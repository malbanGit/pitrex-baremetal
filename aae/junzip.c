// JUnzip library by Joonas Pihlajamaa. See junzip.h for license and details.
// from https://github.com/jokkebk/JUnzip

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//#include "junzip.h"

// MAXIMUM "file load buffer"
#define MAX_ZIP_LOAD_BUF (1024*1024) // 1 meg

unsigned char zipLoadBuffer[MAX_ZIP_LOAD_BUF];


unsigned char jzBuffer[JZ_BUFFER_SIZE]; // limits maximum zip descriptor size

// Read ZIP file end record. Will move within file.
int jzReadEndRecord(JZFile *zip, JZEndRecord *endRecord) {
    long fileSize, readBytes, i;
    JZEndRecord *er;

    if(zip->seek(zip, 0, SEEK_END)) {
        fprintf(stderr, "Couldn't go to end of zip file!");
        return Z_ERRNO;
    }

    if((fileSize = zip->tell(zip)) <= sizeof(JZEndRecord)) {
        fprintf(stderr, "Too small file to be a zip!");
        return Z_ERRNO;
    }

    readBytes = (fileSize < sizeof(jzBuffer)) ? fileSize : sizeof(jzBuffer);

    if(zip->seek(zip, fileSize - readBytes, SEEK_SET)) {
        fprintf(stderr, "Cannot seek in zip file!");
        return Z_ERRNO;
    }

    if(zip->read(zip, jzBuffer, readBytes) < readBytes) {
        fprintf(stderr, "Couldn't read end of zip file!");
        return Z_ERRNO;
    }

    // Naively assume signature can only be found in one place...
    for(i = readBytes - sizeof(JZEndRecord); i >= 0; i--) {
        er = (JZEndRecord *)(jzBuffer + i);
        if(er->signature == 0x06054B50)
//        if(er->signature == 0x504B0506)
            break;
    }

    if(i < 0) {
        fprintf(stderr, "End record signature not found in zip!");
        return Z_ERRNO;
    }

    memcpy(endRecord, er, sizeof(JZEndRecord));

    if(endRecord->diskNumber || endRecord->centralDirectoryDiskNumber ||
            endRecord->numEntries != endRecord->numEntriesThisDisk) {
        fprintf(stderr, "Multifile zips not supported!");
        return Z_ERRNO;
    }

    return Z_OK;
}

// Read ZIP file global directory. Will move within file.
int jzReadCentralDirectory(JZFile *zip, JZEndRecord *endRecord,
        JZRecordCallback callback, void *user_data) {
    JZGlobalFileHeader fileHeader;
    JZFileHeader header;
    int i;

    if(zip->seek(zip, endRecord->centralDirectoryOffset, SEEK_SET)) {
        fprintf(stderr, "Cannot seek in zip file!");
        return Z_ERRNO;
    }
    
    for(i=0; i<endRecord->numEntries; i++) {
        if(zip->read(zip, &fileHeader, sizeof(JZGlobalFileHeader)) <
                sizeof(JZGlobalFileHeader)) {
            fprintf(stderr, "Couldn't read file header %d!", i);
            return Z_ERRNO;
        }
        
        if(fileHeader.signature != 0x02014B50) {
//        if(fileHeader.signature != 0x504B0102) {
            fprintf(stderr, "Invalid file header signature %d!", i);
            return Z_ERRNO;
        }

        if(fileHeader.fileNameLength + 1 >= JZ_BUFFER_SIZE) {
            fprintf(stderr, "Too long file name %d!", i);
            return Z_ERRNO;
        }

        if(zip->read(zip, jzBuffer, fileHeader.fileNameLength) <
                fileHeader.fileNameLength) {
            fprintf(stderr, "Couldn't read filename %d!", i);
            return Z_ERRNO;
        }

        jzBuffer[fileHeader.fileNameLength] = '\0'; // NULL terminate
        
        if(zip->seek(zip, fileHeader.extraFieldLength, SEEK_CUR) ||
                zip->seek(zip, fileHeader.fileCommentLength, SEEK_CUR)) {
            fprintf(stderr, "Couldn't skip extra field or file comment %d", i);
            return Z_ERRNO;
        }

        // Construct JZFileHeader from global file header
        memcpy(&header, &fileHeader.compressionMethod, sizeof(header));
        header.offset = fileHeader.relativeOffsetOflocalHeader;

        if(!callback(zip, i, &header, (char *)jzBuffer, user_data))
            break; // end if callback returns zero
    }

    return Z_OK;
}

// Read local ZIP file header. Silent on errors so optimistic reading possible.
int jzReadLocalFileHeaderRaw(JZFile *zip, JZLocalFileHeader *header,
        char *filename, int len) {

    if(zip->read(zip, header, sizeof(JZLocalFileHeader)) <
            sizeof(JZLocalFileHeader))
        return Z_ERRNO;

    if(header->signature != 0x04034B50)
//    if(header->signature != 0x504B0304 )
        return Z_ERRNO;

    if(len) { // read filename
        if(header->fileNameLength >= len)
            return Z_ERRNO; // filename cannot fit

        if(zip->read(zip, filename, header->fileNameLength) <
                header->fileNameLength)
            return Z_ERRNO; // read fail

        filename[header->fileNameLength] = '\0'; // NULL terminate
    } else { // skip filename
        if(zip->seek(zip, header->fileNameLength, SEEK_CUR))
            return Z_ERRNO;
    }

    if(header->extraFieldLength) {
        if(zip->seek(zip, header->extraFieldLength, SEEK_CUR))
            return Z_ERRNO;
    }

    // For now, silently ignore bit flags and hope ZLIB can uncompress
    // if(header->generalPurposeBitFlag)
    //     return Z_ERRNO; // Flags not supported

    if(header->compressionMethod == 0 &&
            (header->compressedSize != header->uncompressedSize))
        return Z_ERRNO; // Method is "store" but sizes indicate otherwise, abort

    return Z_OK;
}

int jzReadLocalFileHeader(JZFile *zip, JZFileHeader *header,
        char *filename, int len) {
    JZLocalFileHeader localHeader;

    if(jzReadLocalFileHeaderRaw(zip, &localHeader, filename, len) != Z_OK)
        return Z_ERRNO;

    memcpy(header, &localHeader.compressionMethod, sizeof(JZFileHeader));
    header->offset = 0; // not used in local context

    return Z_OK;
}

// Read data from file stream, described by header, to preallocated buffer
int jzReadData(JZFile *zip, JZFileHeader *header, void *buffer) {
    if(header->compressionMethod == 0) { // Store - just read it
        if(zip->read(zip, buffer, header->uncompressedSize) <
                header->uncompressedSize || zip->error(zip))
            return Z_ERRNO;
    } else if(header->compressionMethod == 8) { // Deflate - using puff()
        unsigned long destlen = header->uncompressedSize,
                      sourcelen = header->compressedSize;
//        unsigned char *comp = (unsigned char *)malloc(sourcelen);
//        if(comp == NULL) return Z_ERRNO; // couldn't allocate
//        unsigned long read = zip->read(zip, comp, sourcelen);
        unsigned long read = zip->read(zip, zipLoadBuffer, sourcelen);


        if(read != sourcelen) return Z_ERRNO; // TODO: more robust read loop
        int ret = puff((unsigned char *)buffer, &destlen, zipLoadBuffer, &sourcelen);
//        free(comp);
        if(ret) return Z_ERRNO; // something went wrong
    } else {
        return Z_ERRNO;
    }

    return Z_OK;
}

static size_t
stdio_read_file_handle_read(JZFile *file, void *buf, size_t size)
{
    StdioJZFile *handle = (StdioJZFile *)file;
    return fread(buf, 1, size, handle->fp);
}

static size_t
stdio_read_file_handle_tell(JZFile *file)
{
    StdioJZFile *handle = (StdioJZFile *)file;
    return ftell(handle->fp);
}

static int
stdio_read_file_handle_seek(JZFile *file, size_t offset, int whence)
{
    StdioJZFile *handle = (StdioJZFile *)file;
    return fseek(handle->fp, offset, whence);
}

static int
stdio_read_file_handle_error(JZFile *file)
{
    StdioJZFile *handle = (StdioJZFile *)file;
    return ferror(handle->fp);
}

static void
stdio_read_file_handle_close(JZFile *file)
{
    StdioJZFile *handle = (StdioJZFile *)file;
    fclose(handle->fp);
}


void jzfile_from_stdio_file(FILE *fp, StdioJZFile *handle)
{
//    StdioJZFile *handle = (StdioJZFile *)malloc(sizeof(StdioJZFile));

    handle->handle.read = stdio_read_file_handle_read;
    handle->handle.tell = stdio_read_file_handle_tell;
    handle->handle.seek = stdio_read_file_handle_seek;
    handle->handle.error = stdio_read_file_handle_error;
    handle->handle.close = stdio_read_file_handle_close;
    handle->fp = fp;
}













//------------------------------------------------------------------------
int processFile(JZFile *zip, unsigned char *data) {
    JZFileHeader header;
    char filename[1024];

    if(jzReadLocalFileHeader(zip, &header, filename, sizeof(filename))) {
        printf("Couldn't read local file header!\n\r");
        return -1;
    }

//    printf("%s, %d / %d bytes at offset %08X\n\r", filename,  header.compressedSize, header.uncompressedSize, header.offset);

    if(jzReadData(zip, &header, data) != Z_OK) 
    {
        printf("Couldn't read file data!\n\r");
        return -1;
    }
    return 0;
}
char *nameToSeekInZip=0;

int wasFound; 
int recordCallback(JZFile *zip, int idx, JZFileHeader *header, char *filename, void *user_data) 
{
    long offset;

    offset = zip->tell(zip); // store current position

    if(zip->seek(zip, header->offset, SEEK_SET)) 
    {
        printf("Cannot seek in zip file!\n\r");
        return 0; // abort
    }
    if (strcmp(filename,nameToSeekInZip)==0)
    {
      processFile(zip, user_data); // alters file offset
      wasFound = 1;
    }
    zip->seek(zip, offset, SEEK_SET); // return to position

    return 1; // continue
}
// return 0 on Ok
int loadFromZip(char *zipFile, char *name, unsigned char* block)
{
    StdioJZFile zipFileStruct;
    FILE *fp;
    
    if(!(fp = fopen(zipFile, "rb"))) 
    {
        printf("Couldn't open \"%s\"!\n\r",zipFile);
        return -1;
    }
    jzfile_from_stdio_file(fp, &zipFileStruct);
    JZFile* zip = &zipFileStruct.handle;
    
    
    JZEndRecord endRecord;
    if(jzReadEndRecord(zip, &endRecord)) 
    {
        printf("Couldn't read ZIP file end record.\n\r");
        zip->close(zip);
        return -1;
    }
    nameToSeekInZip = name;
    wasFound = 0;
    if(jzReadCentralDirectory(zip, &endRecord, recordCallback, block)) {
        printf("Couldn't read ZIP file central record.\n\r");
        zip->close(zip);
        nameToSeekInZip = 0;
        return -1;
    }
    nameToSeekInZip = 0;
    zip->close(zip);
    if (wasFound == 0)
    {
      printf("File not found in zip: '%s'\n\r", name);
      return -1;
    }
    return 0;
}





