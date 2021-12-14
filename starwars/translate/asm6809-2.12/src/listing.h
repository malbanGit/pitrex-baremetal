/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#ifndef ASM6809_LISTING_H_
#define ASM6809_LISTING_H_

/*
 * Produce source listings annotated with assembled code output bytes and
 * address information.
 *
 * Before each pass, listing_free_all() ensures any previous attempts at a
 * listing are cleared.  listing_add_line() does what it says on the tin.
 * listing_print() dumps the listing as it currently stands to file.
 */

struct section_span;

void listing_add_line(int pc, int nbytes, struct section_span const *span, char const *text);
void listing_print(FILE *f);
void listing_free_all(void);

#endif
