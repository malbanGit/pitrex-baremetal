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
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "gl_avltree_list.h"
#include "gl_xlist.h"
#include "slist.h"
#include "xalloc.h"

#include "assemble.h"
#include "error.h"
#include "eval.h"
#include "node.h"
#include "section.h"
#include "symbol.h"

/*
 * When asserted, don't raise an error for undefined symbols.
 */

_Bool symbol_ignore_undefined = 0;

/*
 * Record the pass in which each symbol was entered into the table.  This can
 * be used to detect multiple definitions without cycling through a new table
 * each pass.
 */

struct symbol {
	unsigned pass;
	struct node *node;
};

struct symbol_local {
	unsigned line_number;
	struct node *node;
};

static struct dict *symbols = NULL;

static void symbol_free(struct symbol *s) {
	node_free(s->node);
	free(s);
}

static void init_table(void) {
	symbols = dict_new_full(dict_str_hash, dict_str_equal, free, (Hash_data_freer)symbol_free);
}

void symbol_set(const char *key, struct node *value, _Bool changeable, unsigned pass) {
	_Bool is_inconsistent = symbol_force_set(key, value, changeable, pass);
	if (is_inconsistent && !changeable) {
		error(error_type_inconsistent, "value of '%s' unstable", key);
	}
}

_Bool symbol_force_set(const char *key, struct node *value, _Bool changeable, unsigned pass) {
	if (!symbols)
		init_table();
	struct symbol *olds = dict_lookup(symbols, key);
	if (!changeable && olds && olds->pass == pass) {
		error(error_type_syntax, "symbol '%s' redefined", key);
		return 0;
	}
	struct symbol *news = xmalloc(sizeof(*news));
	news->pass = pass;
	news->node = eval_node(value);
	_Bool is_inconsistent = (olds && !node_equal(olds->node, news->node));
	char *key_copy = xstrdup(key);
	dict_insert(symbols, key_copy, news);
	return is_inconsistent;
}

struct node *symbol_try_get(const char *key) {
	if (!symbols)
		init_table();
	struct symbol *s = dict_lookup(symbols, key);
	if (!s)
		return NULL;
	return node_ref(s->node);
}

struct node *symbol_get(const char *key) {
	struct node *n = symbol_try_get(key);
	if (!n) {
		if (symbol_ignore_undefined) {
			return node_new_int(0);
		} else {
			error(error_type_inconsistent, "symbol '%s' not defined", key);
		}
	}
	return n;
}

static void add_to_list(const char *key, struct node *value, struct slist **l) {
	(void)value;
	*l = slist_prepend(*l, (void *)key);
}

struct slist *symbol_get_list(void) {
	struct slist *l = NULL;
	dict_foreach(symbols, (dict_iter_func)add_to_list, &l);
	return l;
}

void symbol_free_all(void) {
	if (!symbols)
		return;
	dict_destroy(symbols);
	symbols = NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void symbol_local_free(struct symbol_local *sym) {
	node_free(sym->node);
	free(sym);
}

static void symbol_local_list_free(gl_list_t sym_list) {
	gl_list_iterator_t iter = gl_list_iterator(sym_list);
	const void *elt;
	gl_list_node_t node;
	while (gl_list_iterator_next(&iter, &elt, &node)) {
		gl_list_remove_node(sym_list, node);
	}
	gl_list_iterator_free(&iter);
	gl_list_free(sym_list);
}

struct dict *symbol_local_table_new(void) {
	return dict_new_full(dict_direct_hash, dict_direct_equal, NULL, (Hash_data_freer)symbol_local_list_free);
}

static struct symbol_local const *sym_found = NULL;

static bool sym_local_equals(const void *a, const void *b) {
	struct symbol_local const *syma = a, *symb = b;
	return syma->line_number == symb->line_number;
}

static int sym_local_compar(const void *a, const void *b) {
	struct symbol_local const *syma = a, *symb = b;
	if (syma->line_number < symb->line_number)
		return -1;
	if (syma->line_number > symb->line_number)
		return 1;
	return 0;
}

static int sym_local_compar_backref(const void *a, const void *b) {
	struct symbol_local const *syma = a, *symb = b;
	if (syma->line_number > symb->line_number)
		return 1;
	if (!sym_found || syma->line_number > sym_found->line_number)
		sym_found = syma;
	if (syma->line_number < symb->line_number)
		return -1;
	return 0;
}

static int sym_local_compar_fwdref(const void *a, const void *b) {
	struct symbol_local const *syma = a, *symb = b;
	if (syma->line_number <= symb->line_number)
		return -1;
	if (!sym_found || syma->line_number < sym_found->line_number)
		sym_found = syma;
	if (syma->line_number > symb->line_number+1)
		return 1;
	return 0;
}

struct node *symbol_local_backref(struct dict *table, intptr_t key, unsigned line_number) {
	gl_list_t sym_list = dict_lookup(table, (void *)key);
	if (sym_list) {
		sym_found = NULL;
		struct symbol_local find = { .line_number = line_number };
		(void)gl_sortedlist_search(sym_list, sym_local_compar_backref, &find);
		if (sym_found) {
			return node_ref(sym_found->node);
		}
	}
	error(error_type_inconsistent, "backref '%ld' not defined", key);
	return NULL;
}

struct node *symbol_local_fwdref(struct dict *table, intptr_t key, unsigned line_number) {
	gl_list_t sym_list = dict_lookup(table, (void *)key);
	if (sym_list) {
		sym_found = NULL;
		struct symbol_local find = { .line_number = line_number };
		(void)gl_sortedlist_search(sym_list, sym_local_compar_fwdref, &find);
		if (sym_found) {
			return node_ref(sym_found->node);
		}
	}
	error(error_type_inconsistent, "fwdref '%ld' not defined", key);
	return NULL;
}

void symbol_local_set(struct dict *table, intptr_t key, unsigned line_number,
		      struct node *value, unsigned pass) {
	(void)pass;
	gl_list_t sym_list = dict_lookup(table, (void *)key);
	gl_list_node_t old_lnode = NULL;
	struct symbol_local const *old_sym = NULL;
	if (!sym_list) {
		sym_list = gl_list_create_empty(GL_AVLTREE_LIST, sym_local_equals, NULL, (gl_listelement_dispose_fn)symbol_local_free, 0);
		dict_insert(table, (void *)key, sym_list);
	} else {
		struct symbol_local find = { .line_number = line_number };
		old_lnode = gl_list_search(sym_list, &find);
		if (old_lnode)
			old_sym = gl_list_node_value(sym_list, old_lnode);
	}
	struct node *newn = eval_node(value);
	struct symbol_local *new_sym = xmalloc(sizeof(*new_sym));
	new_sym->line_number = line_number;
	new_sym->node = newn;
	if (old_sym) {
		if (!node_equal(old_sym->node, newn))
			error(error_type_inconsistent,
			      "value of local label '%ld' unstable", key);
	}
	if (old_lnode) {
		gl_list_remove_node(sym_list, old_lnode);
	}
	gl_sortedlist_add(sym_list, sym_local_compar, new_sym);
}
