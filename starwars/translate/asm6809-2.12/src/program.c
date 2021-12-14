/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2018 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xalloc.h"

#include "asm6809.h"
#include "dict.h"
#include "error.h"
#include "eval.h"
#include "node.h"
#include "program.h"
#include "register.h"
#include "slist.h"
#include "symbol.h"

#include "grammar.h"

struct prog *grammar_parse_file(const char *filename);

static struct slist *files = NULL;
static struct slist *macros = NULL;

struct slist *prog_ctx_stack = NULL;

static struct dict *exports = NULL;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct prog *prog_new(enum prog_type type, const char *name) {
	struct prog *new = xmalloc(sizeof(*new));
	new->type = type;
	new->name = xstrdup(name);
	new->lines = NULL;
	new->next_new_line = &new->lines;
	return new;
}

struct prog *prog_new_file(const char *filename) {
	for (struct slist *l = files; l; l = l->next) {
		struct prog *f = l->data;
		if (0 == strcmp(filename, f->name)) {
			return f;
		}
	}
	struct prog *file = grammar_parse_file(filename);
	if (!file)
		return NULL;
	files = slist_prepend(files, file);
	return file;
}

struct prog *prog_new_macro(const char *name) {
	if (prog_macro_by_name(name)) {
		error(error_type_syntax, "attempt to redefined macro '%s'", name);
		return NULL;
	}
	struct prog *macro = prog_new(prog_type_macro, name);
	macros = slist_prepend(macros, macro);
	return macro;
}

void prog_free(struct prog *f) {
	slist_free_full(f->lines, (slist_free_func)prog_line_free);
	free(f->name);
	free(f);
}

void prog_free_all(void) {
	slist_free_full(macros, (slist_free_func)prog_free);
	slist_free_full(files, (slist_free_func)prog_free);
	prog_free_exports();
}

struct prog *prog_macro_by_name(const char *name) {
	for (struct slist *l = macros; l; l = l->next) {
		struct prog *f = l->data;
		if (0 == strcmp(name, f->name)) {
			return f;
		}
	}
	return NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct prog_line *prog_line_new(struct node *label, struct node *opcode, struct node *args) {
	struct prog_line *l;
	l = xmalloc(sizeof(*l));
	l->ref = 1;
	l->label = label;
	l->opcode = opcode;
	l->args = args;
	l->text = NULL;
	return l;
}

void prog_line_free(struct prog_line *line) {
	if (!line)
		return;
	if (line->ref == 0) {
		error_abort("internal: attempt to free prog_line with ref=0");
	}
	line->ref--;
	if (line->ref > 0)
		return;
	node_free(line->label);
	node_free(line->opcode);
	node_free(line->args);
	if (line->text)
		free(line->text);
	free(line);
}

struct prog_line *prog_line_ref(struct prog_line *line) {
	if (!line)
		return NULL;
	line->ref++;
	return line;
}

void prog_line_set_text(struct prog_line *line, char *text) {
	if (!asm6809_options.listing_required) {
		free(text);
		return;
	}
	if (line) {
		line->text = text;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct prog_ctx *prog_ctx_new(struct prog *prog) {
	struct prog_ctx *new = xmalloc(sizeof(*new));
	new->prog = prog;
	new->line = NULL;
	new->line_number = 0;
	prog_ctx_stack = slist_prepend(prog_ctx_stack, new);
	return new;
}

void prog_ctx_free(struct prog_ctx *ctx) {
	/* It doesn't make sense to free a context partway up the stack */
	assert(prog_ctx_stack != NULL);
	if (prog_ctx_stack->data != ctx) {
		struct prog_ctx *last_ctx = prog_ctx_stack->data;
		if (last_ctx->prog->type == prog_type_macro) {
			error(error_type_syntax, "unfinished macro");
			prog_ctx_free(last_ctx);
		}
	}
	assert(prog_ctx_stack->data == ctx);
	prog_ctx_stack = slist_remove(prog_ctx_stack, ctx);
	free(ctx);
}

void prog_ctx_add_line(struct prog_ctx *ctx, struct prog_line *line) {
	assert(ctx != NULL);
	struct prog *prog = ctx->prog;
	assert(prog != NULL);
	assert(prog->next_new_line != NULL);
	*(prog->next_new_line) = slist_append(*prog->next_new_line, line);
	prog->next_new_line = &(*prog->next_new_line)->next;
	ctx->line_number++;
}

struct prog_line *prog_ctx_next_line(struct prog_ctx *ctx) {
	assert(ctx != NULL);
	assert(ctx->prog != NULL);
	if (ctx->line_number == 0) {
		ctx->line = ctx->prog->lines;
		ctx->line_number++;
	} else if (ctx->line) {
		ctx->line = ctx->line->next;
		ctx->line_number++;
	}
	return ctx->line->data;
}

_Bool prog_ctx_end(struct prog_ctx *ctx) {
	assert(ctx != NULL);
	if (!ctx->prog || !ctx->prog->lines)
		return 1;
	if (ctx->line_number == 0 || ctx->line->next)
		return 0;
	return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Export a macro or symbol.
 */

void prog_export(const char *name) {
	if (!exports) {
		exports = dict_new_full(dict_str_hash, dict_str_equal, free, NULL);
	}
	dict_add(exports, xstrdup(name));
}

void prog_free_exports(void) {
	if (exports) {
		dict_destroy(exports);
		exports = NULL;
	}
}

static void print_symbol(const char *key, FILE *f) {
	struct node *sym = symbol_get(key);
	if (sym) {
		struct node *n = eval_node(sym);
		node_free(sym);
		if (!n)
			return;
		int delim = 0;
		enum node_attr old_attr = node_attr_of(n);
		n = node_set_attr(n, node_attr_none);
		switch (node_type_of(n)) {
		default:
			error(error_type_syntax, "can't export symbol type");
			node_free(n);
			return;
		case node_type_int:
		case node_type_float:
		case node_type_reg:
			break;
		case node_type_string:
			delim = '/';
			break;
		}
		fprintf(f, "%s\tequ\t", key);
		if (delim) fputc(delim, f);
		node_print(f, n);
		if (delim) fputc(delim, f);
		fprintf(f, "\n");
		n = node_set_attr(n, old_attr);
		node_free(n);
	}
}

static void print_exported(const char *key, void *value, FILE *f) {
	(void)value;
	struct prog *macro = prog_macro_by_name(key);
	if (macro) {
		fprintf(f, "%s\tmacro\n", key);
		for (struct slist *l = macro->lines; l; l = l->next) {
			struct prog_line *line = l->data;
			if (!line)
				continue;
			node_print(f, line->label);
			fprintf(f, "\t");
			node_print(f, line->opcode);
			fprintf(f, "\t");
			node_print_array(f, line->args);
			fprintf(f, "\n");
		}
		fprintf(f, "\tendm\n");
		return;
	}
	print_symbol(key, f);
}

void prog_print_exports(FILE *f) {
	if (!exports)
		return;
	dict_foreach(exports, (dict_iter_func)print_exported, f);
}

void prog_print_symbols(FILE *f) {
	struct slist *symbols = symbol_get_list();
	slist_foreach(symbols, (slist_iter_func)print_symbol, f);
	slist_free(symbols);
}
