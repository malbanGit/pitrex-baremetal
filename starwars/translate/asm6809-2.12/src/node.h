/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#ifndef ASM6809_NODE_H_
#define ASM6809_NODE_H_

#include <stdint.h>
#include <stdio.h>

#include "register.h"

/*
 * Abstract syntax trees are used to represent expressions to be evaluated.
 * The tree is formed of nodes, each having a type, an attribute, associated
 * data and a reference count.  Types are either base types, operators or
 * lists.
 *
 * Evaluating a node returns a new node with the attribute copied.  Base types
 * evaluate to (a reference to) themselves, operators evaluate to a base type
 * (or undefined), and lists evaluate to a new list, with each element
 * evaluated according to these rules.
 *
 * In each line of assembly, the label, instruction and arguments fields are
 * all represented by the root node of an abstract syntax tree.  The permitted
 * types for label and instruction are constrained to those that make sense.
 * The arguments field is always a list, the elements of which may be of any
 * type.
 *
 * The attribute of a node is only used for elements of the arguments list.  It
 * indicates some source code annotation indicating things like immediate
 * values ("#") or forced direct addressing ("<").
 */

enum node_attr {
	node_attr_undef = -1,
	node_attr_none = 0,
	/* force operand size / addressing mode */
	node_attr_5bit,
	node_attr_8bit,
	node_attr_16bit,
	node_attr_immediate,
	/* index register pre/post dec/inc */
	node_attr_postinc,
	node_attr_postinc2,
	node_attr_predec,
	node_attr_predec2,
	/* postdec possible as TFM operand */
	node_attr_postdec,
};

enum node_type {
	/* Undefined */
	node_type_undef = -1,

	/* Base types */
	node_type_empty = 0,
	node_type_int,
	node_type_float,
	node_type_reg,
	node_type_string,

	/* Simple types */
	node_type_pc,  // no data, evaluates to current PC
	node_type_backref,  // int data, numeric label
	node_type_fwdref,  // int data, numeric label
	node_type_interp,  // string data, variable name to interpolate

	/* Operator types */
	node_type_id,  // linked list of string & interp
	node_type_text,  // linked list of string & interp
	node_type_oper,  // operator and array of nodes

	/* Array type */
	node_type_array,
};

struct node;

struct node_oper {
	int oper;
	int nargs;
	struct node **args;
};

struct node_array {
	int nargs;
	struct node **args;
};

struct node {
	enum node_type type;
	unsigned ref;
	enum node_attr attr;
	union {
		struct node_oper as_oper;
		int64_t as_int;
		double as_float;
		char *as_string;
		enum reg_id as_reg;
		struct slist *as_list;
		struct node_array as_array;
	} data;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Memory management.
 */

/* Allocate & free nodes */

struct node *node_new(int type);
void node_free(struct node *n);

/* Create a new reference to a node */

struct node *node_ref(struct node *n);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Utility functions.
 */

/* Access simple data safely.  Avoids NULL dereferences for undefined nodes. */

static inline enum node_type node_type_of(struct node const *n) {
	return n ? n->type : node_type_undef;
}

static inline enum node_attr node_attr_of(struct node const *n) {
	return n ? n->attr : node_attr_undef;
}

int node_array_count(struct node const *n);
struct node **node_array_of(struct node const *n);

/* Overwrite attribute on supplied node and return same node.  No new reference
 * is created. */

struct node *node_set_attr(struct node *n, enum node_attr attr);

/* Only override if new attribute is not node_attr_none and old attribute is
 * not pre/post inc/dec.  This allows registers with pre/post inc/dec to be
 * associated with a symbol or passed in a macro expansion.  */

struct node *node_set_attr_if(struct node *n, enum node_attr attr);

/* Compare two nodes.  Both must be defined and of a base type, or they will
 * compare unequal. */

_Bool node_equal(struct node const *n1, struct node const *n2);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Constructor functions.
 */

/* Base types */

struct node *node_new_empty(void);
struct node *node_new_int(int64_t v);
struct node *node_new_float(double v);
struct node *node_new_reg(enum reg_id r);
struct node *node_new_string(char *v);

/* Simple types */

struct node *node_new_pc(void);
struct node *node_new_backref(int64_t v);
struct node *node_new_fwdref(int64_t v);
struct node *node_new_interp(char *v);

/* Operator types */

struct node *node_new_id(struct slist *v);
struct node *node_new_text(struct slist *v);
struct node *node_new_oper_1(int oper, struct node *a1);
struct node *node_new_oper_2(int oper, struct node *a1, struct node *a2);
struct node *node_new_oper_3(int oper, struct node *a1, struct node *a2, struct node *a3);

/* Array type */

struct node *node_new_array(void);
struct node *node_array_push(struct node *a, struct node *n);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Code exporting.
 */

void node_print(FILE *f, struct node const *n);
void node_print_array(FILE *f, struct node const *n);  // print array without []

#endif
