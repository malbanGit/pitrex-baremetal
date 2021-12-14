/*

DJB2 string hash function implementation

*/

#include "config.h"

#include <stddef.h>

#include "c-ctype.h"
#include "c-strcase.h"

#include "hash-djb2.h"

size_t hash_djb2(void const *x, size_t tablesize) {
	size_t h = 5381;
	for (const char *s = x; *s; s++)
		h = ((h << 5) + h) + *s;
	return h % tablesize;
}

size_t hash_djb2_case(void const *x, size_t tablesize) {
	size_t h = 5381;
	for (const char *s = x; *s; s++)
		h = ((h << 5) + h) + c_tolower(*s);
	return h % tablesize;
}
