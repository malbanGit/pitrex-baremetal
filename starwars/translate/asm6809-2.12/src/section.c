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
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

#include "asm6809.h"
#include "dict.h"
#include "error.h"
#include "opcode.h"
#include "section.h"
#include "slist.h"
#include "symbol.h"

static struct dict *sections = NULL;
static unsigned span_sequence = 0;

struct section *cur_section = NULL;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static struct section_span *section_span_new(void) {
	assert(cur_section != NULL);
	struct section_span *new = xmalloc(sizeof(*new));
	new->ref = 1;
	new->sequence = span_sequence++;
	new->org = 0;
	new->put = 0;
	new->size = 0;
	new->allocated = 0;
	new->data = NULL;
	return new;
}

static struct section_span *section_span_ref(struct section_span *span) {
	if (!span)
		return NULL;
	span->ref++;
	return span;
}

static void section_span_free(struct section_span *span) {
	if (!span)
		return;
	if (span->ref == 0) {
		error_abort("internal: attempt to free section_span with ref=0");
	}
	span->ref--;
	if (span->ref > 0)
		return;
	if (span->data)
		free(span->data);
	free(span);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static struct section *section_new(void) {
	struct section *sect = xmalloc(sizeof(*sect));
	sect->spans = NULL;
	sect->span = NULL;
	sect->local_labels = symbol_local_table_new();
	sect->pass = (unsigned)-1;
	sect->line_number = 0;
	sect->pc = 0;
	sect->put = 0;
	sect->dp = asm6809_options.setdp;
	sect->last_pc = 0;
	sect->last_put = 0;
	return sect;
}

void section_free(struct section *sect) {
	if (!sect)
		return;
	dict_destroy(sect->local_labels);
	slist_free_full(sect->spans, (slist_free_func)section_span_free);
	free(sect);
}

void section_free_all(void) {
	if (sections)
		dict_destroy(sections);
	sections = NULL;
	cur_section = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void section_set(const char *name, unsigned pass) {
	if (!sections)
		sections = dict_new_full(dict_str_hash, dict_str_equal, free, (Hash_data_freer)section_free);

	struct section *next_section = dict_lookup(sections, name);
	if (!next_section) {
		next_section = section_new();
		char *key = xstrdup(name);
		dict_insert(sections, key, next_section);
	}

	if (next_section->pass != pass) {
		if (next_section->spans) {
			slist_free_full(next_section->spans, (slist_free_func)section_span_free);
			next_section->spans = NULL;
			next_section->span = NULL;
		}
		if (cur_section && cur_section->pass == pass) {
			next_section->pc = cur_section->last_pc;
			next_section->put = cur_section->last_put;
		} else {
			next_section->pc = 0;
			next_section->put = 0;
		}
		next_section->pass = pass;
		next_section->dp = asm6809_options.setdp;
		next_section->line_number = 0;
	}

	cur_section = next_section;
	return;
}

static void verify_section(void *key, void *value, void *data) {
	(void)key;
	(void)data;
	struct section *sect = value;
	if (!sect)
		return;
	if (sect->last_pc != sect->pc) {
		sect->last_pc = sect->pc;
		sect->last_put = sect->put;
		error(error_type_inconsistent, NULL);
	}
}

void section_finish_pass(void) {
	dict_foreach(sections, verify_section, NULL);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static int span_cmp(struct section_span *a, struct section_span *b) {
	if (a->put < b->put) return -1;
	if (a->put > b->put) return 1;
	if (a->sequence < b->sequence) return -1;
	return 1;
}

/* TODO: When merging spans this should create a new span and free the old
 * ones.  At the moment it breaks reference counting... */

void section_coalesce(struct section *sect, _Bool sort, _Bool pad) {

	struct slist *spans = slist_copy(sect->spans);
	if (sort)
		spans = slist_sort(spans, (slist_cmp_func)span_cmp);
	for (struct slist *l = spans; l && l->next; l = l->next) {
		do {
			struct slist *ln = l->next;
			struct section_span *span = l->data;
			struct section_span *nspan = ln->data;
			unsigned span_end = span->put + span->size;

			if (span_end > nspan->put) {
				error(error_type_data, "data at $%04X overlaps data at $%04X", span->put, nspan->put);
				// truncate earlier span
				span->size -= (span_end - nspan->put);
			} else if (pad && span_end < nspan->put) {
				unsigned npad = nspan->put - span_end;
				if ((span->size + npad) > span->allocated) {
					span->allocated = span->size + npad;
					span->data = xrealloc(span->data, span->allocated);
				}
				memset(span->data + span->size, 0, npad);
				span->size = span->size + npad;
				span_end = span->put + span->size;
			}
			if (span_end == nspan->put) {
				if ((span->size + nspan->size) > span->allocated) {
					span->allocated = span->size + nspan->size;
					span->data = xrealloc(span->data, span->allocated);
				}
				memcpy(span->data + span->size, nspan->data, nspan->size);
				span->size += nspan->size;
				l->next = l->next->next;
				section_span_free(nspan);
				slist_free_1(ln);
			} else {
				break;
			}
		} while (l->next);
	}
	slist_free(sect->spans);
	sect->spans = spans;
}

struct section *section_coalesce_all(_Bool pad) {
	struct section *sect = section_new();

	struct slist *section_list = dict_get_values(sections);
	for (struct slist *l = section_list; l; l = l->next) {
		struct section *s = l->data;
		sect->spans = slist_concat(sect->spans, slist_copy_deep(s->spans, (slist_copy_func)section_span_ref, NULL));
	}
	slist_free(section_list);

	section_coalesce(sect, 1, pad);
	return sect;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Emit data.  Adds bytes to the current span, or creates a new span if
 * appropriate.
 */

#define next_put(s) ((s)->put + (s)->size)
#define next_pc(s) ((int)((s)->org + (s)->size))

static void section_emit(uint8_t const *buf, int nbytes) {
	assert(cur_section != NULL);
	struct section_span *span = cur_section->span;

	if (!span || (cur_section->put != next_put(span)) ||
	    (cur_section->pc != next_pc(span))) {
		if (!span || span->size != 0) {
			span = section_span_new();
			cur_section->spans = slist_append(cur_section->spans, span);
		}
		span->put = cur_section->put;
		span->org = cur_section->pc;
	}
	cur_section->span = span;

	if (cur_section->pc < 0) {
		error(error_type_out_of_range, "assembling to negative address");
	}

	cur_section->put += nbytes;
	cur_section->pc += nbytes;

	if (cur_section->pc >= (int)(span->org + span->allocated)) {
		span->allocated += 128;
		span->data = xrealloc(span->data, span->allocated);
	}

	if (!buf) {
		memset(&span->data[span->size], 0, nbytes);
	} else for (int i = 0; i < nbytes; i++) {
		memcpy(&span->data[span->size], buf, nbytes);
	}
	span->size += nbytes;

	if (cur_section->pc > 0x10000) {
		error(error_type_out_of_range, "assembling beyond addressable memory");
	}
}

void section_emit_pad(int nbytes) {
	section_emit(NULL, nbytes);
}

void section_emit_op(uint16_t op) {
	uint8_t buf[2] = { op >> 8, op & 0xff };
	int nbytes = buf[0] ? 2 : 1;
	section_emit(buf + (2 - nbytes), nbytes);
}

void section_emit_uint8(uint8_t v) {
	section_emit(&v, 1);
}

void section_emit_uint16(uint16_t v) {
	uint8_t buf[2] = { v >> 8, v & 0xff };
	section_emit(buf, 2);
}

void section_emit_uint32(uint32_t v) {
	uint8_t buf[4] = { v >> 24, v >> 16, v >> 8, v & 0xff };
	section_emit(buf, 4);
}

void section_skip(int nbytes) {
	assert(cur_section != NULL);
	cur_section->put += nbytes;
	cur_section->pc += nbytes;
}
