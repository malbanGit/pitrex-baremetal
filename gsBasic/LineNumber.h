
#ifndef _LINENUMBER_H
#define _LINENUMBER_H

#include <stdint.h>
#include <stddef.h>

// Encode and decode line numbers and file indexes

static inline uint16_t EncodeLineNumber(size_t fileIndex, size_t lineNumber)
{
	return (uint16_t) (fileIndex << 13 | (lineNumber & 0x1fff));
}

static inline uint16_t ExtractLineNumber(uint16_t fileIndexLineNumber)
{
	return (uint16_t) (fileIndexLineNumber & 0x1fff);
}


static inline uint16_t ExtractFileIndex(uint16_t fileIndexLineNumber)
{
	return (uint16_t) (fileIndexLineNumber >> 13);
}

#endif
