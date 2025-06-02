
/*****************************************************************/
/****************************** LZ 4 *****************************/
/*****************************************************************/


#define MINMATCH 2
#define HASH_SIZE 256  // Small for 1-byte offset

static inline unsigned char hashN(const unsigned char* p) {
    return (unsigned char)(((p[0] << 3) ^ (p[1] << 2)) & 0xFF);
}
int miniLZ4_compress(const unsigned char* src, int srcSize, unsigned char* dst, int dstCapacity) {
    const unsigned char* ip = src;
    const unsigned char* anchor = ip;
    const unsigned char* iend = src + srcSize;
    const unsigned char* mflimit = iend - MINMATCH;

    unsigned char* op = dst;
    unsigned char* oend = dst + dstCapacity;

    const unsigned char* hashTable[HASH_SIZE] = { 0 };

    while (ip < mflimit) {
        unsigned char hash = hashN(ip);
        const unsigned char* match = hashTable[hash];
        hashTable[hash] = ip;

        if (match && (ip - match <= 255) && memcmp(match, ip, MINMATCH) == 0) {
            // Match found
            int literalLength = ip - anchor;

            unsigned char* token = op++;
            *token = (literalLength >= 15 ? 15 << 4 : literalLength << 4);

            // Encode literal length
            if (literalLength >= 15) {
                int len = literalLength - 15;
                while (len >= 255) { *op++ = 255; len -= 255; }
                *op++ = (unsigned char)len;
            }

            // Copy literals
            memcpy(op, anchor, literalLength);
            op += literalLength;

            // Write 1-byte offset
            *op++ = (unsigned char)(ip - match);



            ip += MINMATCH;
            match += MINMATCH;

            while (ip < iend && *ip == *match) 
            {
                ip++;
                match++;
            }

            int matchLength = ip - anchor - literalLength - MINMATCH;

            if (matchLength >= 15) {
                *token |= 15;
                matchLength -= 15;
                while (matchLength >= 255) { *op++ = 255; matchLength -= 255; }
                *op++ = (unsigned char)matchLength;
            } else {
                *token |= matchLength;
            }

            anchor = ip;
        } else {
            ip++;
        }
    }

    // Encode last literals
    int literalLength = iend - anchor;
    if (op + literalLength + 2 >= oend) return 0;

    unsigned char* token = op++;
    *token = (literalLength >= 15 ? 15 << 4 : literalLength << 4);

    if (literalLength >= 15) {
        int len = literalLength - 15;
        while (len >= 255) { *op++ = 255; len -= 255; }
        *op++ = (unsigned char)len;
    }

    memcpy(op, anchor, literalLength);
    op += literalLength;

    return (int)(op - dst);
}
int miniLZ4_decompress(const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity) {
    const unsigned char* ip = src;
    const unsigned char* iend = src + compressedSize;
    unsigned char* op = dst;
    unsigned char* oend = dst + dstCapacity;

    while (ip < iend) {
        int token = *ip++;

        // Literal length
        int literalLength = token >> 4;
        if (literalLength == 15) {
            unsigned char len;
            do { len = *ip++; literalLength += len; } while (len == 255);
        }

        if (op + literalLength > oend || ip + literalLength > iend) return -1;
        memcpy(op, ip, literalLength);
        ip += literalLength;
        op += literalLength;

        if (ip >= iend) break;

        int offset = *ip++;  // 1-byte offset
        if (offset == 0 || op - offset < dst) return -2;

        unsigned char* match = op - offset;

        int matchLength = token & 0x0F;
        if (matchLength == 15) {
            unsigned char len;
            do { len = *ip++; matchLength += len; } while (len == 255);
        }

        matchLength += MINMATCH;

        if (op + matchLength > oend) return -3;

        while (matchLength--) *op++ = *match++;
    }

    return op - dst;
}

/*****************************************************************/
/*************************** HUFFMAN *****************************/
/*****************************************************************/
#define MAX_SYMBOLS 256
#define MAX_TREE_NODES (((MAX_SYMBOLS)*2)-1)
#define MAX_BITS 12
#define GLOBAL_HUF 1 // uses - MUST use - a fixed dictionary!

typedef struct {
    uint32_t bits;
    int bitlen;
} HuffCode;
typedef struct {
    unsigned char* data;
    int bitpos;
    int bytepos;
    int maxlen;
} BitWriter;
typedef struct {
    const unsigned char* data;
    int bitpos;
    int bytepos;
    int length;
} BitReader;
typedef struct {
    int symbol;
    int freq;
} SymbolFreq;
typedef struct Node {
    int freq;
    int depth;
    struct Node* left;
    struct Node* right;
    int symbol;
} Node;
typedef struct {
    int symbol;
    int bitlen;
} SymbolLen;

HuffCode global_codes[256];
uint8_t global_bitlens[256] = {0};
static void bitwriter_init(BitWriter* bw, unsigned char* buf, int maxlen) {
    bw->data = buf;
    bw->bitpos = 0;
    bw->bytepos = 0;
    bw->maxlen = maxlen;
}
static int compare_symbol_len(const void* a, const void* b) {
    const SymbolLen* sa = (const SymbolLen*)a;
    const SymbolLen* sb = (const SymbolLen*)b;
    if (sa->bitlen != sb->bitlen)
        return sa->bitlen - sb->bitlen;
    return sa->symbol - sb->symbol;
}
static void bitwriter_write(BitWriter* bw, uint32_t bits, int count) {
    while (count--) {
        if (bw->bytepos >= bw->maxlen) return;
        if (bw->bitpos == 0) bw->data[bw->bytepos] = 0;

        if (bits & (1 << count)) bw->data[bw->bytepos] |= (1 << (7 - bw->bitpos));
        bw->bitpos++;
        if (bw->bitpos == 8) { bw->bitpos = 0; bw->bytepos++; }
    }
}
static int bitwriter_bytes_written(BitWriter* bw) {
    return bw->bytepos + (bw->bitpos > 0 ? 1 : 0);
}
static void bitreader_init(BitReader* br, const unsigned char* buf, int len) {
    br->data = buf;
    br->bitpos = 0;
    br->bytepos = 0;
    br->length = len;
}
static int bitreader_read(BitReader* br) {
    if (br->bytepos >= br->length) return -1;
    int bit = (br->data[br->bytepos] >> (7 - br->bitpos)) & 1;
    br->bitpos++;
    if (br->bitpos == 8) { br->bitpos = 0; br->bytepos++; }
    return bit;
}
void assign_lengths(Node* n, int depth, uint8_t* bitlens) {
    if (depth > MAX_BITS) depth = MAX_BITS;  // cap to 15 or user-defined limit

    if (!n->left && !n->right) {
        bitlens[n->symbol] = depth;
    } else {
        assign_lengths(n->left, depth + 1, bitlens);
        assign_lengths(n->right, depth + 1, bitlens);
    }
}
void free_tree(Node* n) {
    if (!n) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}
static void generate_bitlengths(int *freq, uint8_t* bitlens) {

    SymbolFreq symbols[MAX_SYMBOLS];
    int count = 0;
    for (int i = 0; i < MAX_SYMBOLS; ++i) {
        if (freq[i]) {
            symbols[count].symbol = i;
            symbols[count].freq = freq[i];
            count++;
        }
    }

    if (count == 0) return;
    if (count == 1) {
        bitlens[symbols[0].symbol] = 1;
        return;
    }


    Node* nodes[MAX_SYMBOLS * 2];
    int nodeCount = 0;

    for (int i = 0; i < count; ++i) {
        Node* n = (Node*)malloc(sizeof(Node));
        n->freq = symbols[i].freq;
        n->depth = 0;
        n->left = n->right = NULL;
        n->symbol = symbols[i].symbol;
        nodes[nodeCount++] = n;
    }

    while (nodeCount > 1) {
        // Sort nodes by frequency
        for (int i = 0; i < nodeCount - 1; ++i) {
            for (int j = i + 1; j < nodeCount; ++j) {
                if (nodes[i]->freq > nodes[j]->freq) {
                    Node* tmp = nodes[i];
                    nodes[i] = nodes[j];
                    nodes[j] = tmp;
                }
            }
        }

        Node* left = nodes[0];
        Node* right = nodes[1];

        Node* parent = (Node*)malloc(sizeof(Node));
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;
        parent->depth = 0;
        parent->symbol = -1;

        nodes[0] = parent;
        nodes[1] = nodes[nodeCount - 1];
        nodeCount--;
    }


    assign_lengths(nodes[0], 0, bitlens);

    free_tree(nodes[0]);
}
static void generate_codes(const uint8_t* bitlens, HuffCode* codes) {
    SymbolLen sorted[MAX_SYMBOLS];
    int count = 0;

    for (int i = 0; i < MAX_SYMBOLS; ++i) {
        if (bitlens[i] > 0) {
            sorted[count].symbol = i;
            sorted[count].bitlen = bitlens[i];
            count++;
        }
    }

    qsort(sorted, count, sizeof(SymbolLen), compare_symbol_len);

    uint32_t code = 0;
    int prev_len = 0;

    for (int i = 0; i < count; ++i) {
        int len = sorted[i].bitlen;
        if (len != prev_len) {
            code <<= (len - prev_len);
            prev_len = len;
        }
        codes[sorted[i].symbol].bits = code;
        codes[sorted[i].symbol].bitlen = len;
        code++;
    }
}
int huffman_compress(const unsigned char* input, int length, unsigned char* output, int outMax) {
#if GLOBAL_HUF  
    HuffCode codes[256] = {0};
    uint8_t *local_bitlens=global_bitlens;
    if (fixedHuffman!=NULL)
    {
        local_bitlens = fixedHuffman;
    }

    generate_codes(local_bitlens, codes);

    // Write original length (uint16_t)
    output[0] = (length >> 8) & 0xFF;
    output[1] = length & 0xFF;

    // ✅ Start bitstream after dictionary + 2-byte length
    BitWriter bw;
    bitwriter_init(&bw, output + 2, outMax - 2);



    for (int i = 0; i < length; ++i) {
        HuffCode c = codes[input[i]];
        bitwriter_write(&bw, c.bits, c.bitlen);
    }

    return 2 + bitwriter_bytes_written(&bw);
    
#else
    HuffCode codes[256] = {0};

    int freq[MAX_SYMBOLS] = {0};
    for (int i = 0; i < length; ++i) freq[input[i]]++;
    generate_bitlengths(freq, bitlens);
    
    generate_codes(bitlens, codes);

    if (outMax < 256) return -1;

    memcpy(output, bitlens, 256);

    // Write original length (uint16_t)
    output[256] = (length >> 8) & 0xFF;
    output[257] = length & 0xFF;

    // ✅ Start bitstream after dictionary + 2-byte length
    BitWriter bw;
    bitwriter_init(&bw, output + 258, outMax - 258);



    for (int i = 0; i < length; ++i) {
        HuffCode c = codes[input[i]];
        bitwriter_write(&bw, c.bits, c.bitlen);
    }

    return 258 + bitwriter_bytes_written(&bw);
#endif

}
int huffman_decompress(const unsigned char* input, int length, unsigned char* output, int outMax) {
#if GLOBAL_HUF  
    HuffCode codes[256];
    uint8_t *local_bitlens=global_bitlens;

    if (fixedHuffman!=NULL)
    {
        local_bitlens = fixedHuffman;
    }

    generate_codes(local_bitlens, codes);

    // Read original length
    int expectedLength = (input[0] << 8) | input[1];

    BitReader br;
    bitreader_init(&br, input + 2, length - 2);

    int outPos = 0;
    while (outPos < expectedLength) {
        uint32_t code = 0;
        for (int bits = 1; bits <= MAX_BITS; ++bits) {
            int bit = bitreader_read(&br);
            if (bit < 0) return outPos;
            code = (code << 1) | bit;

            for (int i = 0; i < 256; ++i) {
                if (local_bitlens[i] == bits && codes[i].bits == code) {
                    output[outPos++] = (unsigned char)i;
                    goto found;
                }
            }
        }
    found:;
    }
    return outPos;
#else
    if (length < 256) return -1;

    uint8_t bitlens[256];
    HuffCode codes[256];
    memcpy(bitlens, input, 256);
    generate_codes(bitlens, codes);

    // Read original length
    int expectedLength = (input[256] << 8) | input[257];

    BitReader br;
    bitreader_init(&br, input + 258, length - 258);

    int outPos = 0;
    while (outPos < expectedLength) {
        uint32_t code = 0;
        for (int bits = 1; bits <= MAX_BITS; ++bits) {
            int bit = bitreader_read(&br);
            if (bit < 0) return outPos;
            code = (code << 1) | bit;

            for (int i = 0; i < 256; ++i) {
                if (bitlens[i] == bits && codes[i].bits == code) {
                    output[outPos++] = (unsigned char)i;
                    goto found;
                }
            }
        }
    found:;
    }
    return outPos;
#endif  
}


/*****************************************************************/
/****************************** Huffman Tool *********************/
/*****************************************************************/

static int global_histogram[256] = {0};
/*
static int build_tree(const int histogram[256], int sorted[256]) {
    int count = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > 0) {
            sorted[count++] = i;
        }
    }
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (histogram[sorted[j]] < histogram[sorted[i]]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    return count;
}
*/
void print_bitlength_table(const uint8_t bitlens[256]) {
    printf("const uint8_t huffman_bitlengths[256] = {\n");
    for (int i = 0; i < 256; ++i) {
        printf("%3d%s", bitlens[i], (i % 16 == 15) ? ",\n" : ", ");
    }
    printf("\n};\n\n");
}
void huffman_collect_bitlengths(const unsigned char* src, int srcLen) {
    
    for (int i = 0; i < srcLen; ++i) {
        global_histogram[src[i]]++;
    }
    for (int i=0;i<256;i++)
        if (global_histogram[i]==0) global_histogram[i]=1;

    generate_bitlengths(global_histogram, global_bitlens);
    print_bitlength_table(global_bitlens);
}

/*****************************************************************/
/*************************** FRAME DIFS 256 **********************/
/*****************************************************************/

// REALY BAD!
/*
// Compact and efficient byte-stream diff/undiff implementation using 256-byte chunks
// No temporary buffers or copying. All chunk handling is done via direct pointers.

#include <stdint.h>
#include <string.h>

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02
#define CHUNK_SIZE     256

// Create diff for one 256-byte chunk (respects prev_len and curr_len)
static int encode_chunk(const uint8_t *prev, int prev_len,
                        const uint8_t *curr, int curr_len,
                        uint8_t *diff, int diff_capacity) {
    int dp = 0;
    for (int i = 0; i < curr_len;) {
        int max_match = 0;
        int best_offset = 0;

        for (int off = 0; off < prev_len; ++off) {
            int match_len = 0;
            while (match_len < 255 &&
                   i + match_len < curr_len &&
                   off + match_len < prev_len &&
                   prev[off + match_len] == curr[i + match_len]) {
                match_len++;
            }
            if (match_len > max_match) {
                max_match = match_len;
                best_offset = off;
            }
        }

        if (max_match >= 4) {
            if (dp + 3 > diff_capacity) return -1;
            diff[dp++] = DIFF_OP_COPY;
            diff[dp++] = (uint8_t)best_offset;
            diff[dp++] = (uint8_t)max_match;
            i += max_match;
        } else {
            int insert_len = 1;
            while (insert_len < 255 && i + insert_len < curr_len) {
                int j = 0;
                while (j < 4 &&
                       i + insert_len + j < curr_len &&
                       j < prev_len &&
                       prev[i + insert_len + j] == curr[i + insert_len + j]) j++;
                if (j >= 4) break;
                insert_len++;
            }
            if (dp + 2 + insert_len > diff_capacity) return -2;
            diff[dp++] = DIFF_OP_INSERT;
            diff[dp++] = (uint8_t)insert_len;
            memcpy(&diff[dp], &curr[i], insert_len);
            dp += insert_len;
            i += insert_len;
        }
    }
    if (dp + 1 > diff_capacity) return -3;
    diff[dp++] = DIFF_OP_END;
    return dp;
}


// Apply diff to one 256-byte chunk
static int decode_chunk(const uint8_t *prev, int prevSize,
                        const uint8_t *diff, int diffSize,
                        uint8_t *out, int outCapacity,
                        int *bytes_read, int *bytes_written) {
    int dp = 0, cp = 0;

    while (dp < diffSize) {
        uint8_t op = diff[dp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (dp + 2 > diffSize) 
            {
                printf("Decode Error -1\n");
                return -1;
            }   
            uint8_t offset = diff[dp++];
            uint8_t length = diff[dp++];
            if (offset + length > prevSize || cp + length > outCapacity) 
            {
                printf("Decode Error -2\n");
                return -2;
            }   
            memcpy(&out[cp], &prev[offset], length);
            cp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (dp >= diffSize) 
            {
                printf("Decode Error -3\n");
                return -3;
            }   
            uint8_t length = diff[dp++];
            if (dp + length > diffSize || cp + length > outCapacity) 
            {
                printf("Decode Error -4\n");
                return -4;
            }   
            memcpy(&out[cp], &diff[dp], length);
            dp += length;
            cp += length;
        } else 
        {
            printf("Decode Error -5\n");
            return -5;
        }   
    }

    if (bytes_read) *bytes_read = dp;
    if (bytes_written) *bytes_written = cp;
    return 0;
}

// Top-level diff: process src vs. prev in 256-byte chunks
int dif(const unsigned char* prevBuffer, int prevBufferLength,
        const unsigned char* src, int srcSize,
        unsigned char* dst, int dstCapacity) {
    int processed = 0;
    int total_out = 0;

    while (processed < srcSize) {
        int chunk_len = (srcSize - processed >= CHUNK_SIZE) ? CHUNK_SIZE : (srcSize - processed);
        const uint8_t* currChunk = src + processed;

        const uint8_t* prevChunk = prevBuffer + processed;
        int prev_len = prevBufferLength - processed;
        if (prev_len > CHUNK_SIZE) prev_len = CHUNK_SIZE;
        if (prev_len < 0) prev_len = 0;

        int written = encode_chunk(prevChunk, prev_len, currChunk, chunk_len, dst + total_out, dstCapacity - total_out);
        if (written < 0) return written;

        processed += chunk_len;
        total_out += written;
    }
    return total_out;
}

// Top-level undiff: reconstruct full output from diff stream
int undiff(const unsigned char* prevBuffer, int prevBufferLength,
          const unsigned char* src, int srcSize,
          unsigned char* dst, int dstCapacity) {
    int dp = 0;
    int total_out = 0;
    int chunk_index = 0;

    while (dp < srcSize && total_out < dstCapacity) {
        const uint8_t* prevChunk = prevBuffer + chunk_index * CHUNK_SIZE;
        int prev_len = prevBufferLength - chunk_index * CHUNK_SIZE;
        if (prev_len > CHUNK_SIZE) prev_len = CHUNK_SIZE;
        if (prev_len < 0) prev_len = 0;

        int bytes_read = 0, bytes_written = 0;

        int result = decode_chunk(
            prevChunk, prev_len,
            src + dp, srcSize - dp,
            dst + total_out, dstCapacity - total_out,
            &bytes_read, &bytes_written);

        if (result < 0) return result;

        dp += bytes_read;
        total_out += bytes_written;
        chunk_index++;
    }
    return total_out;
}
*/

/*****************************************************************/
/*************************** FRAME DIFS 16 bit *******************/
/*****************************************************************/
/* working good!

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#define MIN_MATCH      4
#define HASH_SIZE2      4096
#define HASH_MASK      (HASH_SIZE2 - 1)

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 4) ^ (p[1] << 2) ^ p[2] ^ (p[3] << 1)) & HASH_MASK;
}

int dif(const uint8_t* prev, int prevSize, const uint8_t* curr, int currSize,
        uint8_t* out, int outCapacity) {
    int op = 0;
    int ip = 0;

    uint16_t hash_table[HASH_SIZE2];
    memset(hash_table, 0xFF, sizeof(hash_table));

    // Index hash table for prevBuffer
    for (int i = 0; i < prevSize - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prev[i]);
        hash_table[h] = i;
    }

    while (ip < currSize) {
        int match_len = 0;
        int match_offset = 0;

        if (ip < currSize - MIN_MATCH) {
            uint16_t h = hash4(&curr[ip]);
            int ref = hash_table[h];
            if (ref != 0xFFFF && ref < prevSize) {
                while (match_len < 255 &&
                       ip + match_len < currSize &&
                       ref + match_len < prevSize &&
                       curr[ip + match_len] == prev[ref + match_len]) {
                    match_len++;
                }
                if (match_len >= MIN_MATCH) {
                    match_offset = ref;
                }
            }
        }

        if (match_len >= MIN_MATCH) {
            if (op + 4 > outCapacity) return -1;
            out[op++] = DIFF_OP_COPY;
            out[op++] = (uint8_t)(match_offset >> 8);
            out[op++] = (uint8_t)(match_offset & 0xFF);
            out[op++] = (uint8_t)match_len;
            ip += match_len;
        } else {
            // INSERT single byte or literal run
            int lit_start = ip;
            ip++;
            while (ip < currSize) {
                uint16_t h = hash4(&curr[ip]);
                int ref = hash_table[h];
                int len = 0;
                if (ref != 0xFFFF && ref < prevSize) {
                    while (len < 255 &&
                           ip + len < currSize &&
                           ref + len < prevSize &&
                           curr[ip + len] == prev[ref + len]) {
                        len++;
                    }
                }
                if (len >= MIN_MATCH) break;
                ip++;
            }
            int lit_len = ip - lit_start;
            if (op + 2 + lit_len > outCapacity) return -2;
            out[op++] = DIFF_OP_INSERT;
            out[op++] = (uint8_t)lit_len;
            memcpy(&out[op], &curr[lit_start], lit_len);
            op += lit_len;
        }
    }

    if (op + 1 > outCapacity) return -3;
    out[op++] = DIFF_OP_END;
    return op;
}

int undiff(const uint8_t* prev, int prevSize,
           const uint8_t* src, int srcSize,
           uint8_t* dst, int dstCapacity) {
    int ip = 0, op = 0;

    while (ip < srcSize) {
        uint8_t opcode = src[ip++];
        if (opcode == DIFF_OP_END) break;

        if (opcode == DIFF_OP_COPY) {
            if (ip + 3 > srcSize) return -1;
            int offset = (src[ip++] << 8);
            offset |= src[ip++];
            int length = src[ip++];
            if (offset + length > prevSize || op + length > dstCapacity) return -2;
            memcpy(&dst[op], &prev[offset], length);
            op += length;
        } else if (opcode == DIFF_OP_INSERT) {
            if (ip >= srcSize) return -3;
            int length = src[ip++];
            if (ip + length > srcSize || op + length > dstCapacity) return -4;
            memcpy(&dst[op], &src[ip], length);
            ip += length;
            op += length;
        } else {
            return -5;
        }
    }
    return op;
} // end

*/

/*****************************************************************/
/*************************** FRAME DIFS + LZ *********************/
/*****************************************************************/
/*
Working good

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#include <string.h>
#include <stdint.h>

#define MIN_MATCH       4
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH      8  // Max matches per hash bucket

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;

    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH && sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= MIN_MATCH) {
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = best_len;
            sp += best_len;
        } else {
            int lit_start = sp++;
            while (sp < srcLen) {
                int lz_len = 0;
                if (sp <= srcLen - MIN_MATCH) {
                    uint16_t h = hash4(&src[sp]);
                    HashBucket* b = &hashTable[h];
                    for (int k = 0; k < b->count; ++k) {
                        int off = b->pos[k];
                        int ml = 0;
                        while (ml < MAX_LENGTH && sp + ml < srcLen &&
                               off + ml < prevLen &&
                               src[sp + ml] == prevBuffer[off + ml]) {
                            ml++;
                        }
                        if (ml >= MIN_MATCH) {
                            lz_len = ml;
                            break;
                        }
                    }
                }
                if (lz_len >= MIN_MATCH) break;
                sp++;
            }

            int lit_len = sp - lit_start;
            if (dp + 2 + lit_len > dstCap) return -2;
            dst[dp++] = DIFF_OP_INSERT;
            dst[dp++] = lit_len;
            memcpy(&dst[dp], &src[lit_start], lit_len);
            dp += lit_len;
        }
    }

    if (dp + 1 > dstCap) return -3;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
          const unsigned char* src, int srcLen,
          unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
            int high = src[sp++];
            int low  = src[sp++];
            int offset = (high << 8) | low;
            
            uint8_t length = src[sp++];
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;
        } else {
            return -5;
        }
    }
    return dp;
}
*/




/*****************************************************************/
/*************************** FRAME DIFS 16 bit *******************/
/*****************************************************************/
/*
COPY-Kommandos werden erst ab einer Mindestlänge (COPY_MIN_LEN) ausgeführt, um ineffiziente kurze COPYs zu vermeiden.

Literale werden nun gebündelt und erst beim nächsten COPY oder am Ende geschrieben („Literal Buffer“).

Das verbessert die Kompression merklich bei kleinen Veränderungen.

Ein paar kleine byts besser! (Lunar Lander, star wars ist schlechter

*/

/*

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#include <string.h>
#include <stdint.h>

#define MIN_MATCH       4
#define COPY_MIN_LEN    6  // Avoid short COPYs that waste space
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH      8  // Max matches per hash bucket

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;
    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    uint8_t lit_buffer[256];
    int lit_len = 0;

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH && sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= COPY_MIN_LEN) {
            // Flush any pending literals
            if (lit_len > 0) {
                if (dp + 2 + lit_len > dstCap) return -2;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = best_len;
            sp += best_len;
        } else {
            if (lit_len < sizeof(lit_buffer)) {
                lit_buffer[lit_len++] = src[sp++];
            } else {
                // Flush buffer
                if (dp + 2 + lit_len > dstCap) return -3;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
        }
    }

    // Final flush
    if (lit_len > 0) {
        if (dp + 2 + lit_len > dstCap) return -4;
        dst[dp++] = DIFF_OP_INSERT;
        dst[dp++] = lit_len;
        memcpy(&dst[dp], lit_buffer, lit_len);
        dp += lit_len;
    }

    if (dp + 1 > dstCap) return -5;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
          const unsigned char* src, int srcLen,
          unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
            int high = src[sp++];
            int low  = src[sp++];
            int offset = (high << 8) | low;

            uint8_t length = src[sp++];
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;
        } else {
            return -5;
        }
    }
    return dp;
}
*/


#include <string.h>
#include <stdint.h>

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#define MIN_MATCH       4
#define COPY_MIN_LEN    6
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH       8

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;
    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    uint8_t lit_buffer[255];
    int lit_len = 0;

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH &&
                       sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= COPY_MIN_LEN) {
            if (lit_len > 0) {
                if (dp + 2 + lit_len > dstCap) return -2;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = (uint8_t)lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = (uint8_t)best_len;
            sp += best_len;
        } else {
            lit_buffer[lit_len++] = src[sp++];
            if (lit_len == 255) {
                if (dp + 2 + lit_len > dstCap) return -3;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = (uint8_t)lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
        }
    }

    if (lit_len > 0) {
        if (dp + 2 + lit_len > dstCap) return -4;
        dst[dp++] = DIFF_OP_INSERT;
        dst[dp++] = (uint8_t)lit_len;
        memcpy(&dst[dp], lit_buffer, lit_len);
        dp += lit_len;
    }

    if (dp + 1 > dstCap) return -5;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
           const unsigned char* src, int srcLen,
           unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
            int offset = (src[sp] << 8) | src[sp + 1];
            uint8_t length = src[sp + 2];
            sp += 3;
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;

        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;

        } else {
            return -5;  // unknown opcode
        }
    }
    return dp;
}
