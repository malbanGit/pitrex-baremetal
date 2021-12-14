/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#ifndef ASM6809_EVAL_H_
#define ASM6809_EVAL_H_

struct node;

/* Evaluate a node. */
struct node *eval_node(struct node *n);

/*
 * Cast a node to another of a specific type.
 */

struct node *eval_string(struct node *n);
struct node *eval_float(struct node *n);
struct node *eval_int(struct node *n);

/*
 * These variants free the old node before returning the new.
 */

struct node *eval_float_free(struct node *n);
struct node *eval_int_free(struct node *n);

#endif
