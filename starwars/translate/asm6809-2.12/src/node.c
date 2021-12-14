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
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

#include "error.h"
#include "node.h"
#include "register.h"
#include "slist.h"

#include "grammar.h"

static struct node *node_new_oper_n(int oper, int nargs);

struct node *node_new(int type) {
	struct node *n = xmalloc(sizeof(*n));
	n->ref = 1;
	n->type = type;
	n->attr = node_attr_none;
	return n;
}

void node_free(struct node *n) {
	if (!n)
		return;
	if (n->ref == 0) {
		error_abort("internal: attempt to free node with ref=0");
	}
	n->ref--;
	if (n->ref > 0)
		return;

	switch (n->type) {

	/* Nodes containing string data: */
	case node_type_string:
	case node_type_interp:
		free(n->data.as_string);
		break;

	/* Node array */
	case node_type_array:
		for (int i = 0; i < n->data.as_array.nargs; i++)
			node_free(n->data.as_array.args[i]);
		free(n->data.as_array.args);
		break;

	/* Nodes containing linked lists of other nodes: */
	case node_type_id:
	case node_type_text:
		slist_free_full(n->data.as_list, (slist_free_func)node_free);
		break;

	/* Operator node (operator type plus array of nodes): */
	case node_type_oper:
		for (int i = 0; i < n->data.as_oper.nargs; i++)
			node_free(n->data.as_oper.args[i]);
		free(n->data.as_oper.args);
		break;

	default:
		break;
	}
	free(n);
}

struct node *node_ref(struct node *n) {
	if (!n)
		return NULL;
	n->ref++;
	return n;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Utility functions.
 */

int node_array_count(struct node const *n) {
	if (!n)
		return 0;
	if (n->type != node_type_array)
		return 0;
	return n->data.as_array.nargs;
}

struct node **node_array_of(struct node const *n) {
	if (!n)
		return NULL;
	if (n->type != node_type_array)
		return NULL;
	return n->data.as_array.args;
}

struct node *node_set_attr(struct node *n, enum node_attr attr) {
	if (n)
		n->attr = attr;
	return n;
}

/* Allow register attributes to override "none" */

struct node *node_set_attr_if(struct node *n, enum node_attr attr) {
	if (!n)
		return NULL;
	if (attr != node_attr_none) {
		n->attr = attr;
		return n;
	}
	switch (n->attr) {
	case node_attr_postinc:
	case node_attr_postinc2:
	case node_attr_predec:
	case node_attr_predec2:
	case node_attr_postdec:
		break;
	default:
		n->attr = attr;
		break;
	}
	return n;
}

_Bool node_equal(struct node const *n1, struct node const *n2) {
	if (node_type_of(n1) == node_type_undef ||
	    node_type_of(n2) == node_type_undef)
		return 0;
	if (n1->type != n2->type)
		return 0;
	switch (n1->type) {
	case node_type_float:
		return n1->data.as_float == n2->data.as_float;
	case node_type_int:
		return n1->data.as_int == n2->data.as_int;
	case node_type_reg:
		return n1->data.as_reg == n2->data.as_reg;
	case node_type_string:
		return (0 == strcmp(n1->data.as_string, n2->data.as_string));
	default:
		break;
	}
	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Constructor functions.
 */

/* Base types */

struct node *node_new_empty(void) {
	return node_new(node_type_empty);
}

struct node *node_new_int(int64_t v) {
	struct node *n = node_new(node_type_int);
	n->data.as_int = v;
	return n;
}

struct node *node_new_float(double v) {
	struct node *n = node_new(node_type_float);
	n->data.as_float = v;
	return n;
}

struct node *node_new_reg(enum reg_id r) {
	struct node *n = node_new(node_type_reg);
	assert(r > REG_INVALID && r < REG_MAX);
	n->data.as_reg = r;
	return n;
}

struct node *node_new_string(char *v) {
	struct node *n = node_new(node_type_string);
	n->data.as_string = v;
	return n;
}

/* Simple types */

struct node *node_new_pc(void) {
	return node_new(node_type_pc);
}

struct node *node_new_backref(int64_t v) {
	struct node *n = node_new(node_type_backref);
	n->data.as_int = v;
	return n;
}

struct node *node_new_fwdref(int64_t v) {
	struct node *n = node_new(node_type_fwdref);
	n->data.as_int = v;
	return n;
}

struct node *node_new_interp(char *v) {
	struct node *n = node_new(node_type_interp);
	n->data.as_string = v;
	return n;
}

/* Operator types */

struct node *node_new_id(struct slist *v) {
	struct node *n = node_new(node_type_id);
	n->data.as_list = v;
	return n;
}

struct node *node_new_text(struct slist *v) {
	struct node *n = node_new(node_type_text);
	n->data.as_list = v;
	return n;
}

static struct node *node_new_oper_n(int oper, int nargs) {
	struct node *n = node_new(node_type_oper);
	struct node **arga = xmalloc(nargs * sizeof(*arga));
	n->data.as_oper.oper = oper;
	n->data.as_oper.nargs = nargs;
	n->data.as_oper.args = arga;
	return n;
}

struct node *node_new_oper_1(int oper, struct node *a1) {
	struct node *n = node_new_oper_n(oper, 1);
	n->data.as_oper.args[0] = a1;
	return n;
}

struct node *node_new_oper_2(int oper, struct node *a1, struct node *a2) {
	struct node *n = node_new_oper_n(oper, 2);
	n->data.as_oper.args[0] = a1;
	n->data.as_oper.args[1] = a2;
	return n;
}

struct node *node_new_oper_3(int oper, struct node *a1, struct node *a2, struct node *a3) {
	struct node *n = node_new_oper_n(oper, 3);
	n->data.as_oper.args[0] = a1;
	n->data.as_oper.args[1] = a2;
	n->data.as_oper.args[2] = a3;
	return n;
}

/* Array type */

struct node *node_new_array(void) {
	struct node *n = node_new(node_type_array);
	n->data.as_array.nargs = 0;
	n->data.as_array.args = NULL;
	return n;
}

struct node *node_array_push(struct node *a, struct node *n) {
	struct node *ret = a ? a : node_new_array();
	struct node **arga = ret->data.as_array.args;
	int nargs = ++ret->data.as_array.nargs;
	arga = xrealloc(arga, nargs * sizeof(*arga));
	arga[nargs-1] = n;
	ret->data.as_array.args = arga;
	return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Code exporting.
 */

static const char *opstr(int op) {
	static char str[2];
	switch (op) {
	case SHL: return "<<";
	case SHR: return ">>";
	default: str[0] = op; str[1] = 0; break;
	}
	return str;
}

void node_print(FILE *f, struct node const *n) {
	struct slist *l;
	if (!n) {
		return;
	}

	switch (n->attr) {
	case node_attr_5bit:
		fprintf(f, "<<");
		break;
	case node_attr_8bit:
		fprintf(f, "<");
		break;
	case node_attr_16bit:
		fprintf(f, ">");
		break;
	case node_attr_immediate:
		fprintf(f, "#");
		break;
	case node_attr_predec:
		fprintf(f, "-");
		break;
	case node_attr_predec2:
		fprintf(f, "--");
		break;
	default:
		break;
	}

	switch (n->type) {

	/* Undefined */
	case node_type_undef:
		error_abort("internal: unexpected node_type_undef in node_print");

	/* Base types */
	case node_type_empty:
		break;
	case node_type_int:
		fprintf(f, "%"PRId64, n->data.as_int);
		break;
	case node_type_float:
		fprintf(f, "%f", n->data.as_float);
		break;
	case node_type_reg:
		fprintf(f, "%s", reg_id_to_name(n->data.as_reg));
		break;
	case node_type_string:
		fprintf(f, "%s", n->data.as_string);
		break;

	/* Simple types */
	case node_type_pc:
		fprintf(f, "*");
		break;
	case node_type_backref:
		fprintf(f, "%"PRId64"B", n->data.as_int);
		break;
	case node_type_fwdref:
		fprintf(f, "%"PRId64"F", n->data.as_int);
		break;
	case node_type_interp:
		fprintf(f, "&{%s}", n->data.as_string);
		break;

	/* Operator types */
	case node_type_id:
		for (l = n->data.as_list; l; l = l->next) {
			node_print(f, (struct node *)l->data);
		}
		break;
	case node_type_text:
		fprintf(f, "/");
		for (l = n->data.as_list; l; l = l->next) {
			node_print(f, (struct node *)l->data);
		}
		fprintf(f, "/");
		break;
	case node_type_oper:
		fprintf(f, "(");
		if (n->data.as_oper.nargs == 1) {
			fprintf(f, "%s", opstr(n->data.as_oper.oper));
			node_print(f, n->data.as_oper.args[0]);
		} else if (n->data.as_oper.nargs == 2) {
			node_print(f, n->data.as_oper.args[0]);
			fprintf(f, "%s", opstr(n->data.as_oper.oper));
			node_print(f, n->data.as_oper.args[1]);
		}
		fprintf(f, ")");
		break;

	/* Array type */
	case node_type_array:
		fprintf(f, "[");
		node_print_array(f, n);
		fprintf(f, "]");
		break;

	default:
		error_abort("internal: unhandled node type (%d) in node_print", n->type);
		break;

	}

	switch (n->attr) {
	case node_attr_postinc:
		fprintf(f, "+");
		break;
	case node_attr_postinc2:
		fprintf(f, "++");
		break;
	case node_attr_postdec:
		fprintf(f, "-");
		break;
	default:
		break;
	}
}

void node_print_array(FILE *f, struct node const *n) {
	if (!n)
		return;
	if (n->type != node_type_array)
		return;
	for (int i = 0; i < n->data.as_array.nargs; i++) {
		node_print(f, n->data.as_array.args[i]);
		if ((i+1) < n->data.as_array.nargs)
			fprintf(f, ",");
	}
}
