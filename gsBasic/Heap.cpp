/* 
 * File:   Heap.cpp
 * Author: Bob
 * 
 * Created on April 11, 2016, 10:12 AM
 */

#include "Heap.h"

#include <new>
#include <cstdlib> // for malloc() and free()

#ifdef _MSC_VER

void MemoryUsage(size_t *used, size_t *free, size_t *largestFreeBlock)
{
	*used = 0;
	*free = 0;
    *largestFreeBlock = 0;
}

#else

#ifdef RPI0
void MemoryUsage(size_t *used, size_t *free, size_t *largestFreeBlock)
{
    *used = 0;
    *free = 0;
    *largestFreeBlock = 0;
}

#else

extern "C" size_t PortGetFreeHeapSize();
extern "C" size_t PortGetUsedHeapSize();
extern "C" size_t PortGetLargestFreeBlockSize();

void MemoryUsage(size_t *used, size_t *free, size_t *largestFreeBlock)
{
	*used = PortGetUsedHeapSize();
	*free = PortGetFreeHeapSize();
    *largestFreeBlock = PortGetLargestFreeBlockSize();
}

#endif
#endif

