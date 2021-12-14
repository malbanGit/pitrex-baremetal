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

#include "error.h"
#include "eval.h"
#include "node.h"
#include "output.h"
#include "section.h"
#include "slist.h"
#include "symbol.h"

/* Helper that dumps a single binary blob to file. */

static void write_single_binary(FILE *f, struct section_span *span) {
	unsigned size = span->size;
	fwrite(span->data, 1, size, f);
}

/* Helper to figure out exec address. */

static int get_exec_addr(void) {
	struct node *n = symbol_try_get(".exec");
	int ret = -1;
	if (n) {
		if (n->data.as_int >= 0)
			ret = n->data.as_int & 0xffff;
		node_free(n);
	}
	return ret;
}

/* Output format: Plain binary.  All coalesced into one big blob. */

void output_binary(const char *filename) {
	FILE *f = fopen(filename, "wb");
	if (!f)
		return;

	struct section *sect = section_coalesce_all(1);
	if (sect->spans) {
		struct section_span *span = sect->spans->data;
		write_single_binary(f, span);
	}

	section_free(sect);
	fclose(f);
}

/* Output format: DragonDOS binary. */

void output_dragondos(const char *filename) {
	int exec_addr = get_exec_addr();

	FILE *f = fopen(filename, "wb");
	if (!f)
		return;

	struct section *sect = section_coalesce_all(1);
	struct section_span *span = NULL;
	unsigned put = 0;
	unsigned size = 0;
	if (sect->spans) {
		span = sect->spans->data;
		put = span->put;
		size = span->size;
	}
	if (exec_addr < 0)
		exec_addr = put & 0xffff;

	fputc(0x55, f);
	fputc(0x02, f);
	fputc((put >> 8) & 0xff, f);
	fputc(put & 0xff, f);
	fputc((size >> 8) & 0xff, f);
	fputc(size & 0xff, f);
	fputc((exec_addr >> 8) & 0xff, f);
	fputc(exec_addr  & 0xff, f);
	fputc(0xaa, f);
	if (span)
		write_single_binary(f, span);

	section_free(sect);
	fclose(f);
}

/* Output format: CoCo RSDOS binary. */

/*
 * TODO: Currently all sections are merged.  Would be preferable to have an
 * ordered list of sections, each of which can be coalesced and dumped in
 * order.
 */

void output_coco(const char *filename) {
	int exec_addr = get_exec_addr();

	FILE *f = fopen(filename, "wb");
	if (!f)
		return;

	struct section *sect = section_coalesce_all(0);

	for (struct slist *l = sect->spans; l; l = l->next) {
		struct section_span *span = l->data;
		unsigned put = span->put;
		unsigned size = span->size;
		if (exec_addr < 0)
			exec_addr = put & 0xffff;
		fputc(0x00, f);
		fputc((size >> 8) & 0xff, f);
		fputc(size & 0xff, f);
		fputc((put >> 8) & 0xff, f);
		fputc(put & 0xff, f);
		fwrite(span->data, 1, size, f);
	}

	fputc(0xff, f);
	fputc(0x00, f);
	fputc(0x00, f);
	fputc((exec_addr >> 8) & 0xff, f);
	fputc(exec_addr  & 0xff, f);

	section_free(sect);
	fclose(f);
}

/* Output format: Motorola SREC. */

void output_motorola_srec(const char *filename) {
	int exec_addr = get_exec_addr();

	FILE *f = fopen(filename, "wb");
	if (!f)
		return;

	struct section *sect = section_coalesce_all(0);

	for (struct slist *l = sect->spans; l; l = l->next) {
		struct section_span *span = l->data;
		unsigned put = span->put;
		unsigned size = span->size;
		unsigned base = 0;
		while (size > 0) {
			unsigned nbytes = (size > 32) ? 32 : size;
			unsigned sum = nbytes + 3 + (put >> 8) + (put & 0xff);
			fprintf(f, "S1%02X%04X", nbytes + 3, put);
			for (unsigned i = 0; i < nbytes; i++) {
				fprintf(f, "%02X", (unsigned)span->data[base + i]);
				sum += span->data[base + i];
			}
			sum = ~sum & 0xff;
			fprintf(f, "%02X\n", sum);
			put += nbytes;
			base += nbytes;
			size -= nbytes;
		}
	}

	if (exec_addr >= 0) {
		unsigned sum = exec_addr + (exec_addr >> 8) + 3;
		fprintf(f, "S903%04X%02X\n", exec_addr, ~sum & 0xff);
	} else {
		fprintf(f, "S9030000FC\n");
        }

	section_free(sect);
	fclose(f);
}

/* Output format: Intel HEX. */

void output_intel_hex(const char *filename) {
	int exec_addr = get_exec_addr();

	FILE *f = fopen(filename, "wb");
	if (!f)
		return;

	struct section *sect = section_coalesce_all(0);

	for (struct slist *l = sect->spans; l; l = l->next) {
		struct section_span *span = l->data;
		unsigned put = span->put;
		unsigned size = span->size;
		unsigned base = 0;
		while (size > 0) {
			unsigned nbytes = (size > 32) ? 32 : size;
			unsigned sum = nbytes + put + (put >> 8);
			fprintf(f, ":%02X%04X00", nbytes, put);
			for (unsigned i = 0; i < nbytes; i++) {
				fprintf(f, "%02X", (unsigned)span->data[base + i]);
				sum += span->data[base + i];
			}
			sum = (~sum + 1) & 0xff;
			fprintf(f, "%02X\n", sum);
			put += nbytes;
			base += nbytes;
			size -= nbytes;
		}
	}

	if (exec_addr < 0) {
		fprintf(f, ":00000001FF\n");
	} else {
		unsigned sum = exec_addr + (exec_addr >> 8) + 1;
		fprintf(f, ":00%04X01%02X\n", exec_addr, (~sum + 1) & 0xff);
	}

	section_free(sect);
	fclose(f);
}
