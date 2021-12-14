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
#include <stdio.h>
#include <stdlib.h>

#include "xalloc.h"
#include "xvasprintf.h"

#include "asm6809.h"
#include "error.h"
#include "program.h"
#include "slist.h"

/* Highest error level encountered */
enum error_type error_level = error_type_none;

/* Track errors during a pass */
struct error {
	enum error_type type;
	const char *filename;
	unsigned line_number;
	char *message;
};
static struct slist *error_list = NULL;
static struct slist **error_list_next = &error_list;

/*
 * Report an error.
 */

static void verror(enum error_type type, const char *fmt, va_list ap) {
	struct error *err = NULL;
	if (type > error_level) {
		error_level = type;
	}
	if (type == error_type_inconsistent &&
	    error_level == error_type_out_of_range)
		error_level = error_type_inconsistent;
	if (fmt) {
		err = xmalloc(sizeof(*err));
		err->type = type;
		if (prog_ctx_stack) {
			struct prog_ctx *ctx = prog_ctx_stack->data;
			assert(ctx != NULL);
			struct prog *prog = ctx->prog;
			assert(prog != NULL);
			err->filename = prog->name;
			err->line_number = ctx->line_number;
		} else {
			err->filename = NULL;
			err->line_number = 0;
		}
		err->message = xvasprintf(fmt, ap);
	}
	if (err) {
		*error_list_next = slist_append(*error_list_next, err);
		error_list_next = &((*error_list_next)->next);
	}
}

void error(enum error_type type, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	verror(type, fmt, ap);
	va_end(ap);
}

/*
 * Report a fatal error, print all errors and abort.  Used for unexpected
 * failures in memory handling or reference counting.
 */

void error_abort(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	verror(error_type_fatal, fmt, ap);
	va_end(ap);
	error_print_list();
	exit(EXIT_FAILURE);
}

/*
 * Clear all errors reported in the previous pass.
 */

void error_clear_all(void) {
	while (error_list) {
		struct error *err = error_list->data;
		error_list = slist_remove(error_list, err);
		free(err->message);
		free(err);
	}
	error_list_next = &error_list;
	error_level = error_type_none;
}

/*
 * If finishing, this is called to print out the errors found in the last pass.
 * Frees data as it goes.  Resets error_level.
 */

void error_print_list(void) {
	int min_error = error_type_illegal;
	min_error -= asm6809_options.verbosity;
	fflush(stdout);
	if (error_level >= error_type_inconsistent)
		min_error = error_level;
	while (error_list) {
		struct error *err = error_list->data;
		error_list = slist_remove(error_list, err);
		if ((int)err->type >= min_error) {
			switch (err->type) {
			case error_type_none:
				break;
			case error_type_inefficient:
			case error_type_illegal:
				fprintf(stderr, "warning: ");
				break;
			case error_type_syntax:
				fprintf(stderr, "syntax ");
				/* fall through */
			default:
				fprintf(stderr, "error: ");
				break;
			}
			if (err->filename) {
				fprintf(stderr, "%s:", err->filename);
				if (err->line_number > 0)
					fprintf(stderr, "%u:", err->line_number);
				fputc(' ', stderr);
			}
			if (err->message) {
				fprintf(stderr, "%s\n", err->message);
			}
		}
		if (err->message)
			free(err->message);
		free(err);
	}
	error_list_next = &error_list;
	error_level = error_type_none;
}
