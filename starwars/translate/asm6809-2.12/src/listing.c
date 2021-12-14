/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

#include "asm6809.h"
#include "listing.h"
#include "program.h"
#include "section.h"
#include "slist.h"

struct listing_line {
	int pc;
	int nbytes;
	struct section_span const *span;
	char const *text;
};

static struct slist *listing_lines = NULL;
static struct slist **listing_next = &listing_lines;

void listing_add_line(int pc, int nbytes, struct section_span const *span, char const *text) {
	if (!asm6809_options.listing_required)
		return;
	struct listing_line *l = xmalloc(sizeof(*l));
	l->pc = pc;
	l->nbytes = nbytes;
	l->span = span;
	l->text = text;
	*listing_next = slist_append(*listing_next, l);
	listing_next = &((*listing_next)->next);
}

void listing_print(FILE *f) {
	for (struct slist *ll = listing_lines; ll; ll = ll->next) {
		struct listing_line *l = ll->data;
		int col = 0;
		if (l->pc >= 0) {
			fprintf(f, "%04X  ", l->pc & 0xffff);
			col += 6;
		}
		if (l->nbytes > 0 && l->span && l->span->data) {
			int offset = l->pc - l->span->org;
			for (int i = 0; i < l->nbytes; i++) {
				fprintf(f, "%02X", l->span->data[i+offset] & 0xff);
				col += 2;
			}
		}
		do {
			fputc(' ', f);
			col++;
		} while (col < 22);
		col = 0;
		for (int i = 0; l->text[i]; i++) {
			if (l->text[i] == '\t') {
				do {
					fputc(' ', f);
					col++;
				} while ((col % 8) != 0);
			} else {
				fputc(l->text[i], f);
				col++;
			}
		}
		fputc('\n', f);
	}
}

void listing_free_all(void) {
	while (listing_lines) {
		struct listing_line *l = listing_lines->data;
		listing_lines = slist_remove(listing_lines, l);
		free(l);
	}
	listing_next = &listing_lines;
}
