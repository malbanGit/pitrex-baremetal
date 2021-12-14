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

#include "array.h"
#include "c-strcase.h"
#include "dict.h"
#include "slist.h"

#include "asm6809.h"
#include "assemble.h"
#include "error.h"
#include "eval.h"
#include "instr.h"
#include "interp.h"
#include "listing.h"
#include "node.h"
#include "opcode.h"
#include "program.h"
#include "register.h"
#include "section.h"
#include "symbol.h"

static struct prog_ctx *defining_macro_ctx = NULL;
static int defining_macro_level = 0;

static unsigned asm_pass;
static unsigned prog_depth = 0;

enum cond_state {
	cond_state_if,
	cond_state_if_done,
	cond_state_else
};

enum translate {
	translate_none,
	translate_basic_keyword,
	translate_vdg,
	translate_inverse_vdg
};

static void set_label(struct node *label, struct node *value, _Bool changeable);
static void args_float_to_int(struct node *args);
static int verify_num_args(struct node *args, int min, int max, const char *op);
static int64_t have_int_optional(struct node *args, int aindex, const char *op, int64_t in);
static int64_t have_int_required(struct node *args, int aindex, const char *op, int64_t in);

/* Pseudo-operations */

static void pseudo_macro(struct prog_line *);
static void pseudo_endm(struct prog_line *);
static void pseudo_export(struct prog_line *);

static void pseudo_equ(struct prog_line *);
static void pseudo_set(struct prog_line *);
static void pseudo_org(struct prog_line *);
static void pseudo_section(struct prog_line *);
static void pseudo_section_name(struct prog_line *line);

static void pseudo_fcb(struct prog_line *);
static void pseudo_fcc(struct prog_line *);
static void pseudo_fcn(struct prog_line *);
static void pseudo_fcv(struct prog_line *);
static void pseudo_fci(struct prog_line *);
static void pseudo_fcs(struct prog_line *);
static void pseudo_fdb(struct prog_line *);
static void pseudo_fqb(struct prog_line *);
static void pseudo_rzb(struct prog_line *);
static void pseudo_fill(struct prog_line *);
static void pseudo_rmb(struct prog_line *);
static void pseudo_align(struct prog_line *line);

static void pseudo_put(struct prog_line *);
static void pseudo_setdp(struct prog_line *);
static void pseudo_include(struct prog_line *);
static void pseudo_includebin(struct prog_line *);
static void pseudo_end(struct prog_line *);
static void pseudo_nop(struct prog_line *);

struct pseudo_op {
	const char *name;
	void (*handler)(struct prog_line *);
};

/* Pseudo-ops that override any label meaning */

static struct pseudo_op pseudo_label_ops[] = {
	{ .name = "equ", .handler = &pseudo_equ },
	{ .name = "set", .handler = &pseudo_set },
	{ .name = "org", .handler = &pseudo_org },
	{ .name = "section", .handler = &pseudo_section },
	{ .name = "code", .handler = &pseudo_section_name },
	{ .name = "data", .handler = &pseudo_section_name },
	{ .name = "bss", .handler = &pseudo_section_name },
	{ .name = "ram", .handler = &pseudo_section_name },
	{ .name = "auto", .handler = &pseudo_section_name },
};

/* Pseudo-ops that emit data */

static struct pseudo_op pseudo_data_ops[] = {
	{ .name = "fcb", .handler = &pseudo_fcb },
	{ .name = "fcc", .handler = &pseudo_fcc },
	{ .name = "fcn", .handler = &pseudo_fcn },
	{ .name = "fcv", .handler = &pseudo_fcv },
	{ .name = "fci", .handler = &pseudo_fci },
	{ .name = "fcs", .handler = &pseudo_fcs },
	{ .name = "fdb", .handler = &pseudo_fdb },
	{ .name = "fqb", .handler = &pseudo_fqb },
	{ .name = "rzb", .handler = &pseudo_rzb },
	{ .name = "fzb", .handler = &pseudo_rzb },
	{ .name = "zmb", .handler = &pseudo_rzb },  // alias
	{ .name = "bsz", .handler = &pseudo_rzb },  // alias
	{ .name = "fill", .handler = &pseudo_fill },
	{ .name = "rmb", .handler = &pseudo_rmb },
	{ .name = "align", .handler = &pseudo_align },
	{ .name = "includebin", .handler = &pseudo_includebin },
};

/* Other pseudo-ops */

static struct pseudo_op pseudo_ops[] = {
	{ .name = "put", .handler = &pseudo_put },
	{ .name = "setdp", .handler = &pseudo_setdp },
	{ .name = "include", .handler = &pseudo_include },
	{ .name = "end", .handler = &pseudo_end },
	{ .name = "page", .handler = &pseudo_nop },
	{ .name = "opt", .handler = &pseudo_nop },
	{ .name = "spc", .handler = &pseudo_nop },
	{ .name = "ttl", .handler = &pseudo_nop },
	{ .name = "nam", .handler = &pseudo_nop },
	{ .name = "name", .handler = &pseudo_nop },
};

/* Speed psuedo-op lookups using dictionaries */

static struct dict *pseudo_label_dict = NULL;
static struct dict *pseudo_data_dict = NULL;
static struct dict *pseudo_dict = NULL;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void assemble_init(void) {
	if (!pseudo_label_dict) {
		pseudo_label_dict = dict_new(dict_str_hash_case, dict_str_equal_case);
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(pseudo_label_ops); i++) {
			dict_insert(pseudo_label_dict, (void *)pseudo_label_ops[i].name, (void *)pseudo_label_ops[i].handler);
		}
	}
	if (!pseudo_data_dict) {
		pseudo_data_dict = dict_new(dict_str_hash_case, dict_str_equal_case);
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(pseudo_data_ops); i++) {
			dict_insert(pseudo_data_dict, (void *)pseudo_data_ops[i].name, (void *)pseudo_data_ops[i].handler);
		}
	}
	if (!pseudo_dict) {
		pseudo_dict = dict_new(dict_str_hash_case, dict_str_equal_case);
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(pseudo_ops); i++) {
			dict_insert(pseudo_dict, (void *)pseudo_ops[i].name, (void *)pseudo_ops[i].handler);
		}
	}
}

void assemble_free_all(void) {
	if (pseudo_label_dict) {
		dict_destroy(pseudo_label_dict);
		pseudo_label_dict = NULL;
	}
	if (pseudo_data_dict) {
		dict_destroy(pseudo_data_dict);
		pseudo_data_dict = NULL;
	}
	if (pseudo_dict) {
		dict_destroy(pseudo_dict);
		pseudo_dict = NULL;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static enum node_attr arg_attr(struct node *args, int index) {
	if (!args)
		return node_attr_undef;
	if (index >= args->data.as_array.nargs)
		return node_attr_undef;
	return node_attr_of(args->data.as_array.args[index]);
}

static void args_float_to_int(struct node *args) {
	if (!args)
		return;
	if (node_type_of(args) != node_type_array)
		return;
	int nargs = args->data.as_array.nargs;
	struct node **arga = args->data.as_array.args;
	for (int i = 0; i < nargs; i++) {
		if (node_type_of(arga[i]) == node_type_float)
			arga[i] = eval_int_free(arga[i]);
	}
}

/* Verify size of an array is allowed for op.  Returns actual number of args if
 * in range, or -1 if not.  Specify negative max for no upper bound.  */

static int verify_num_args(struct node *args, int min, int max, const char *op) {
	int nargs = node_array_count(args);
	if (max < 0) {
		if (nargs >= min)
			return nargs;
		error(error_type_syntax, "'%s' requires at least %d argument%s",
		      op, min, (min == 1) ? "" : "s");
		return -1;
	}
	if (nargs >= min && nargs <= max)
		return nargs;
	if (min == max) {
		error(error_type_syntax, "'%s' requires exactly %d argument%s",
		      op, min, (min == 1) ? "" : "s");
	} else {
		error(error_type_syntax, "'%s' requires between %d and %d arguments",
		      op, min, max);
	}
	return -1;
}

/* Get integer value from array.  Returns 0 (failure) if bad type. */

static int64_t have_int_optional(struct node *args, int aindex, const char *op, int64_t in) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (aindex < 0 || aindex >= nargs)
		return in;
	switch (node_type_of(arga[aindex])) {
	default:
		error(error_type_syntax,
		      "argument %d of '%s' invalid: expected integer value", aindex+1, op);
		return in;
	case node_type_undef:
		break;
	case node_type_empty:
		in = 0;
		break;
	case node_type_int:
		in = arga[aindex]->data.as_int;
		break;
	case node_type_float:
		in = arga[aindex]->data.as_float;
		break;
	}
	return in;
}

/* Same, but fail if arg index out of bounds. */

static int64_t have_int_required(struct node *args, int aindex, const char *op, int64_t in) {
	int nargs = node_array_count(args);
	if (aindex < 0 || aindex >= nargs) {
		error(error_type_syntax,
		      "required argument %d of '%s' missing", aindex+1, op);
		return in;
	}
	return have_int_optional(args, aindex, op, in);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* Perform an assembly pass on a program. */

void assemble_prog(struct prog *prog, unsigned pass) {
	if (prog_depth >= asm6809_options.max_program_depth) {
		error(error_type_fatal, "maximum program depth exceeded");
		return;
	}
	asm_pass = pass;
	prog_depth++;
	struct prog_ctx *ctx = prog_ctx_new(prog);

	/* cond_excluded will point to the element in cond_list that started to
	 * exclude code.  ENDIF will stop excluding code if back to that
	 * position. */
	struct slist *cond_list = NULL;
	struct slist *cond_excluded = NULL;

	while (!prog_ctx_end(ctx)) {
		/* Dummy line to be populated with values evaluated or not, as
		 * appropriate. */
		struct prog_line n_line;

		struct prog_line *l = prog_ctx_next_line(ctx);

		assert(l != NULL);

		/* Incremented for every line encountered.  Doesn't correspond
		 * to any file or macro line number.  Must be consistent across
		 * passes - see section.h for details. */
		cur_section->line_number++;

		if (!l->label && !l->opcode && !l->args) {
			listing_add_line(-1, 0, NULL, l->text);
			continue;
		}

		n_line.label = NULL;
		n_line.opcode = eval_string(l->opcode);
		n_line.args = NULL;
		n_line.text = l->text;

		/* Macro handling */

		if (!cond_excluded && n_line.opcode && 0 == c_strcasecmp("macro", n_line.opcode->data.as_string)) {
			defining_macro_level++;
			if (defining_macro_level == 1) {
				n_line.label = eval_string(l->label);
				n_line.args = node_ref(l->args);
				pseudo_macro(&n_line);
				listing_add_line(-1, 0, NULL, l->text);
				goto next_line;
			}
		}

		if (!cond_excluded && n_line.opcode && 0 == c_strcasecmp("endm", n_line.opcode->data.as_string)) {
			if (defining_macro_level == 0) {
				error(error_type_syntax, "ENDM without beginning MACRO");
				goto next_line;
			}
			defining_macro_level--;
			if (defining_macro_level == 0) {
				n_line.args = eval_node(l->args);
				pseudo_endm(&n_line);
				listing_add_line(-1, 0, NULL, l->text);
				goto next_line;
			}
		}

		if (defining_macro_level > 0) {
			if (defining_macro_ctx)
				prog_ctx_add_line(defining_macro_ctx, prog_line_ref(l));
			listing_add_line(-1, 0, NULL, l->text);
			goto next_line;
		}

		/* Conditional assembly */

		if (n_line.opcode && 0 == c_strcasecmp("if", n_line.opcode->data.as_string)) {
			listing_add_line(-1, 0, NULL, l->text);
			if (!cond_excluded) {
				symbol_ignore_undefined = 1;
				n_line.args = eval_node(l->args);
				symbol_ignore_undefined = 0;
				if (verify_num_args(n_line.args, 1, 1, "IF") < 0)
					goto next_line;
			}
			if (!cond_excluded && !have_int_required(n_line.args, 0, "IF", 0)) {
				cond_list = slist_prepend(cond_list, (void *)cond_state_if);
				cond_excluded = cond_list;
			} else {
				cond_list = slist_prepend(cond_list, (void *)cond_state_if_done);
			}
			goto next_line;
		}

		if (n_line.opcode && 0 == c_strcasecmp("elsif", n_line.opcode->data.as_string)) {
			listing_add_line(-1, 0, NULL, l->text);
			if (!cond_list) {
				error(error_type_syntax, "ELSIF without IF");
			} else if ((intptr_t)cond_list->data == cond_state_else) {
				error(error_type_syntax, "repeated ELSE");
			} else {
				if (!cond_excluded) {
					cond_excluded = cond_list;
				} else if (cond_excluded == cond_list &&
					   (intptr_t)cond_list->data != cond_state_if_done) {
					symbol_ignore_undefined = 1;
					n_line.args = eval_node(l->args);
					symbol_ignore_undefined = 0;
					if (verify_num_args(n_line.args, 1, 1, "ELSIF") < 0)
						goto next_line;
					if (have_int_required(n_line.args, 0, "ELSIF", 0)) {
						cond_excluded = NULL;
						cond_list->data = (void *)cond_state_if_done;
					}
				}
			}
			goto next_line;
		}

		if (n_line.opcode && 0 == c_strcasecmp("else", n_line.opcode->data.as_string)) {
			listing_add_line(-1, 0, NULL, l->text);
			if (!cond_list) {
				error(error_type_syntax, "ELSE without IF");
			} else if ((intptr_t)cond_list->data == cond_state_else) {
				error(error_type_syntax, "repeated ELSE");
			} else if (cond_excluded == cond_list) {
				if ((intptr_t)cond_list->data != cond_state_if_done) {
					cond_list->data = (void *)cond_state_else;
					cond_excluded = NULL;
				}
			} else if (!cond_excluded) {
				cond_list->data = (void *)cond_state_else;
				cond_excluded = cond_list;
			}
			goto next_line;
		}

		if (n_line.opcode && 0 == c_strcasecmp("endif", n_line.opcode->data.as_string)) {
			listing_add_line(-1, 0, NULL, l->text);
			if (!cond_list) {
				error(error_type_syntax, "ENDIF without IF");
				goto next_line;
			} else if (cond_excluded == cond_list) {
				cond_excluded = NULL;
			}
			cond_list = slist_remove(cond_list, cond_list->data);
			goto next_line;
		}

		if (cond_excluded) {
			listing_add_line(-1, 0, NULL, l->text);
			goto next_line;
		}

		/* Normal processing */

		n_line.label = eval_int(l->label);
		if (!n_line.label)
			n_line.label = eval_string(l->label);

		/* EXPORT only needs symbol names, not their values */
		if (n_line.opcode && 0 == c_strcasecmp("export", n_line.opcode->data.as_string)) {
			n_line.args = node_ref(l->args);
			pseudo_export(&n_line);
			listing_add_line(-1, 0, NULL, l->text);
			goto next_line;
		}

		/* Anything else needs a fully evaluated list of arguments */
		n_line.args = eval_node(l->args);

		/* Pseudo-ops which determine a label's value */
		void (*op_handler)(struct prog_line *);
		if (n_line.opcode) {
			op_handler = dict_lookup(pseudo_label_dict, n_line.opcode->data.as_string);
			if (op_handler) {
				op_handler(&n_line);
				goto next_line;
			}
		}

		/* Otherwise, any label on the line gets PC as its value */
		if (n_line.label) {
			set_label(n_line.label, node_new_int(cur_section->pc), 0);
		}

		/* No opcode?  Next line. */
		if (!n_line.opcode) {
			if (n_line.label)
				listing_add_line(cur_section->pc & 0xffff, 0, NULL, l->text);
			goto next_line;
		}

		/* Pseudo-ops that emit or reserve data */
		op_handler = dict_lookup(pseudo_data_dict, n_line.opcode->data.as_string);
		if (op_handler) {
			int old_pc = cur_section->pc;
			op_handler(&n_line);
			int nbytes = cur_section->pc - old_pc;
			if (cur_section->span && cur_section->pc == (int)(cur_section->span->put + cur_section->span->size))
				listing_add_line(old_pc & 0xffff, nbytes, cur_section->span, l->text);
			else
				listing_add_line(old_pc & 0xffff, nbytes, NULL, l->text);
			goto next_line;
		}

		/* Other pseudo-ops */
		op_handler = dict_lookup(pseudo_dict, n_line.opcode->data.as_string);
		if (op_handler) {
			listing_add_line(cur_section->pc, 0, NULL, l->text);
			op_handler(&n_line);
			goto next_line;
		}

		/* Real instructions */
		struct opcode const *op = opcode_by_name(n_line.opcode->data.as_string);
		if (op) {
			int old_pc = cur_section->pc;
			int op_ext_type = op->type & OPCODE_EXT_TYPE;
			/* No instruction accepts floats, convert them all to
			 * integer here as a convenience: */
			args_float_to_int(n_line.args);
			if (op->type == OPCODE_INHERENT) {
				instr_inherent(op, n_line.args);
			} else if ((op_ext_type == OPCODE_IMM8 ||
				    op_ext_type == OPCODE_IMM16 ||
				    op_ext_type == OPCODE_IMM32) &&
				   (arg_attr(l->args, 0) == node_attr_immediate)) {
				instr_immediate(op, n_line.args);
			} else if (op_ext_type == OPCODE_IMM8_MEM) {
				/* Important to check this here, as the
				 * OPCODE_MEM bits will also be set. */
				instr_imm8_mem(op, n_line.args);
			} else if (op->type & OPCODE_MEM) {
				instr_address(op, n_line.args, -1);
			} else if (op_ext_type == OPCODE_REL8 ||
				   op_ext_type == OPCODE_REL16) {
				instr_rel(op, n_line.args);
			} else if (op_ext_type == OPCODE_STACKU) {
				instr_stack(op, n_line.args, REG_U);
			} else if (op_ext_type == OPCODE_STACKS) {
				instr_stack(op, n_line.args, REG_S);
			} else if (op_ext_type == OPCODE_PAIR) {
				instr_pair(op, n_line.args);
			} else if (op_ext_type == OPCODE_TFM) {
				instr_tfm(op, n_line.args);
			} else if (op_ext_type == OPCODE_REG_MEM) {
				instr_reg_mem(op, n_line.args);
			} else {
				error(error_type_syntax, "invalid addressing mode");
			}
			int nbytes = cur_section->pc - old_pc;
			listing_add_line(old_pc & 0xffff, nbytes, cur_section->span, l->text);
			goto next_line;
		}

		/* Macro expansion */
		struct prog *macro = prog_macro_by_name(n_line.opcode->data.as_string);
		if (macro) {
			listing_add_line(cur_section->pc & 0xffff, 0, NULL, l->text);
			interp_push(n_line.args);
			assemble_prog(macro, pass);
			interp_pop();
			goto next_line;
		}

		error(error_type_syntax, "unknown instruction '%s'", n_line.opcode->data.as_string);

next_line:
		node_free(n_line.label);
		node_free(n_line.opcode);
		node_free(n_line.args);
	}

	if (cond_list) {
		slist_free(cond_list);
		error(error_type_syntax, "IF not matched with ENDIF");
	}

	assert(prog_depth > 0);
	prog_depth--;
	prog_ctx_free(ctx);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* A disposable node must be passed in as value.  symbol_set() performs an eval
 * and stores the result, not the original node. */

static void set_label(struct node *label, struct node *value, _Bool changeable) {
	switch (node_type_of(label)) {
	default:
		error(error_type_syntax, "invalid label type");
		break;
	case node_type_undef:
		break;
	case node_type_int:
		symbol_local_set(cur_section->local_labels, label->data.as_int, cur_section->line_number, value, asm_pass);
		break;
	case node_type_string:
		symbol_set(label->data.as_string, value, changeable, asm_pass);
		break;
	}
	node_free(value);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* EQU.  A symbol with the name of this line's label is assigned a value. */

/* TODO: would be nice to support EQU arrays */

static void pseudo_equ(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "EQU") < 0)
		return;
	struct node **arga = node_array_of(line->args);
	set_label(line->label, node_ref(arga[0]), 0);
	struct node *n = eval_int(arga[0]);
	if (n) {
		listing_add_line(n->data.as_int & 0xffff, 0, NULL, line->text);
		node_free(n);
	} else {
		listing_add_line(-1, 0, NULL, line->text);
	}
}

/* SET.  As EQU, but symbol is permitted to change value. */

static void pseudo_set(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "SET") < 0)
		return;
	struct node **arga = node_array_of(line->args);
	set_label(line->label, node_ref(arga[0]), 1);
	struct node *n = eval_int(arga[0]);
	if (n) {
		listing_add_line(n->data.as_int & 0xffff, 0, NULL, line->text);
		node_free(n);
	} else {
		listing_add_line(-1, 0, NULL, line->text);
	}
}

/* ORG.  Following instructions will be assembled to this address. */

static void pseudo_org(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "ORG") < 0)
		return;
	long new_pc = have_int_required(line->args, 0, "ORG", cur_section->pc);
	cur_section->pc = new_pc;
	if (new_pc >= 0)
		cur_section->put = new_pc;
	set_label(line->label, node_new_int(new_pc), 0);
	listing_add_line(new_pc & 0xffff, 0, NULL, line->text);
}

/* SECTION.  Switch sections. */

static void pseudo_section(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "SECTION") < 0)
		return;
	struct node **arga = node_array_of(line->args);
	if (node_type_of(arga[0]) == node_type_undef)
		return;
	struct node *n = eval_string(arga[0]);
	if (!n) {
		error(error_type_syntax, "invalid argument to SECTION");
		return;
	}
	section_set(n->data.as_string, asm_pass);
	node_free(n);
	set_label(line->label, node_new_int(cur_section->pc), 0);
	listing_add_line(cur_section->pc, 0, NULL, line->text);
}

/*
 * Switch to section with opcode field as name.
 */

static void pseudo_section_name(struct prog_line *line) {
	section_set(line->opcode->data.as_string, asm_pass);
	set_label(line->label, node_new_int(cur_section->pc), 0);
	listing_add_line(cur_section->pc, 0, NULL, line->text);
}

/* PUT.  Following instructions will be located at this address.  Allows
 * assembling as if at one address while locating them elsewhere. */

static void pseudo_put(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "PUT") < 0)
		return;
	long new_put = have_int_required(line->args, 0, "PUT", cur_section->put);
	if (new_put < 0) {
		error(error_type_out_of_range, "invalid negative address for PUT");
		return;
	}
	cur_section->put = new_put;
}

/* SETDP.  Set the assumed Direct Page value (8-bit).  Addresses evaluated to
 * exist within this page will be assembled to use direct addressing, if
 * possible. */

static void pseudo_setdp(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "SETDP") < 0)
		return;
	long new_dp = have_int_required(line->args, 0, "SETDP", -1);
	// negative number implies no valid DP
	if (new_dp >= 0)
		cur_section->dp = new_dp & 0xff;
	else
		cur_section->dp = -1;
}

/* EXPORT.  Flag a symbol or macro for exporting in the symbols file. */

static void pseudo_export(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "EXPORT");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		struct node *n = eval_string(arga[i]);
		if (n) {
			prog_export(n->data.as_string);
			node_free(n);
		}
	}
}

static uint8_t ascii_to_vdg(uint8_t av, _Bool invert) {
	uint8_t eor = invert ? 0x40 : 0;
	if (av < 0x20)
		return av ^ eor;
	if (av < 0x40)
		return (av+64) ^ eor;
	if (av < 0x60)
		return av ^ eor;
	if (av == 0x60)
		return 0x20 ^ eor;
	if (av < 0x80)
		return (av-96) ^ eor;
	return av;
}

static void emit_formatted(char const *ins, int argn, struct node *n,
			   enum translate mode) {
	uint8_t or_last = (mode == translate_basic_keyword) ? 0x80 : 0;
	_Bool invert = (mode == translate_inverse_vdg);
	_Bool to_vdg = invert || (mode == translate_vdg);
	switch (node_type_of(n)) {
	default:
		error(error_type_syntax,
		      "argument %d of '%s' invalid: expected string or integer value",
		      argn, ins);
		break;
	case node_type_undef:
		section_emit_pad(1);
		break;
	case node_type_empty:
		section_emit_uint8(0);
		break;
	case node_type_int:
		section_emit_uint8(n->data.as_int | or_last);
		break;
	case node_type_float:
		section_emit_uint8((int32_t)n->data.as_float | or_last);
		break;
	case node_type_string:
		for (char const *c = n->data.as_string; *c; c++) {
			uint8_t cc = *c;
			if (!*(c+1))
				cc |= or_last;
			if (to_vdg)
				cc = ascii_to_vdg(cc, invert);
			section_emit_uint8(cc);
		}
		break;
	}
}

/* FCB, FCC.  Embed string and byte constants. */

static void pseudo_fcb(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FCB");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCB", i, arga[i], 0);
	}
}

static void pseudo_fcc(struct prog_line *line) {
	// exactly the same as FCB, just different reporting
	int nargs = verify_num_args(line->args, 1, -1, "FCC");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCC", i, arga[i], 0);
	}
}

/* FCV.  As FCC, but translates characters to VDG equivalents. */

static void pseudo_fcv(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FCV");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCV", i, arga[i], translate_vdg);
	}
}

/* FCI.  As FCV, but inverse video. */

static void pseudo_fci(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FCI");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCI", i, arga[i], translate_inverse_vdg);
	}
}

/* FCN.  As FCC, but zero-terminate. */

static void pseudo_fcn(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FCN");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCN", i, arga[i], translate_none);
	}
	section_emit_uint8(0);
}

/* FCS.  As FCC, but last byte gets top bit set. */

static void pseudo_fcs(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FCS");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	for (int i = 0; i < nargs; i++) {
		emit_formatted("FCS", i, arga[i], translate_basic_keyword);
	}
}

/* FDB.  Embed 16-bit constants. */

static void pseudo_fdb(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FDB");
	if (nargs < 0)
		return;
	for (int i = 0; i < nargs; i++) {
		long word = have_int_optional(line->args, i, "FDB", 0);
		section_emit_uint16(word);
	}
}

/* FQB.  Embed 32-bit constants. */

static void pseudo_fqb(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "FQB");
	if (nargs < 0)
		return;
	for (int i = 0; i < nargs; i++) {
		long word = have_int_optional(line->args, i, "FQB", 0);
		section_emit_uint32(word);
	}
}

/* RZB.  Reserve zero bytes.  Additional argument specifies a non-zero fill
 * value. */

static void pseudo_rzb(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 2, "RZB") < 0)
		return;
	long count = have_int_required(line->args, 0, "RZB", 0);
	long fill = have_int_optional(line->args, 1, "RZB", 0);
	if (count < 0) {
		error(error_type_out_of_range, "negative count for RZB");
	}
	for (long i = 0; i < count; i++)
		section_emit_uint8(fill);
}

/* FILL.  Effectively an arg-swapped version of the two-arg form of RZB. */

static void pseudo_fill(struct prog_line *line) {
	if (verify_num_args(line->args, 2, 2, "FILL") < 0)
		return;
	long fill = have_int_required(line->args, 0, "FILL", 0);
	long count = have_int_required(line->args, 1, "FILL", 0);
	if (count < 0) {
		error(error_type_out_of_range, "negative count for FILL");
	}
	for (long i = 0; i < count; i++)
		section_emit_uint8(fill);
}

/* RMB.  Reserve memory. */

static void pseudo_rmb(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "RMB") < 0)
		return;
	long count = have_int_required(line->args, 0, "RMB", 0);
	if (count < 0) {
		error(error_type_out_of_range, "negative argument to RMB");
		return;
	}
	section_skip(count);
}

/* ALIGN.  Pad to aligned address. */

static void pseudo_align(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, 2, "ALIGN");
	if (nargs < 0)
		return;
	long align = have_int_required(line->args, 0, "ALIGN", 0);
	long fill = have_int_optional(line->args, 1, "ALIGN", 0);
	if (align < 1) {
		error(error_type_out_of_range, "invalid argument to ALIGN");
		return;
	}
	long count = (align - (cur_section->pc % align)) % align;
	if (nargs < 2) {
		section_skip(count);
	} else {
		for (long i = 0; i < count; i++)
			section_emit_uint8(fill);
	}
}

/* INCLUDE.  Nested inclusion of source files. */

/* TODO: extra arguments should become available as positional variables. */

static void pseudo_include(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 1, -1, "INCLUDE");
	if (nargs < 0)
		return;
	struct node **arga = node_array_of(line->args);
	if (node_type_of(arga[0]) != node_type_string) {
		error(error_type_syntax, "invalid argument to INCLUDE");
		return;
	}
	struct section *old_section = cur_section;
	struct prog *file = prog_new_file(arga[0]->data.as_string);
	if (!file)
		return;
	assemble_prog(file, asm_pass);
	cur_section = old_section;
}

/* INCLUDEBIN.  Include a binary object in-place.  Unlike INCLUDE, the filename
 * may be a forward reference, as binary objects cannot introduce new local
 * labels. */

static void pseudo_includebin(struct prog_line *line) {
	if (verify_num_args(line->args, 1, 1, "INCLUDE") < 0)
		return;
	struct node **arga = node_array_of(line->args);
	if (node_type_of(arga[0]) != node_type_string) {
		error(error_type_syntax, "invalid argument to INCLUDEBIN");
		return;
	}
	FILE *f = fopen(arga[0]->data.as_string, "rb");
	if (!f) {
		error(error_type_fatal, "file not found: %s", arga[0]->data.as_string);
		return;
	}
	int c;
	while ((c = fgetc(f)) != EOF) {
		section_emit_uint8(c);
	}
	fclose(f);
}

/* MACRO.  Start defining a named macro.  The line's label field is used as the
 * macro name.  Arguments are ignored. */

static void pseudo_macro(struct prog_line *line) {
	const char *name;
	switch (node_type_of(line->label)) {
	case node_type_string:
		name = line->label->data.as_string;
		break;
	default:
		error(error_type_syntax, "missing or invalid macro name");
		return;
	}
	struct prog *macro = prog_macro_by_name(name);
	if (macro) {
		if (macro->pass == asm_pass)
			error(error_type_syntax, "macro '%s' redefined", name);
		return;
	}
	macro = prog_new_macro(name);
	macro->pass = asm_pass;
	defining_macro_ctx = prog_ctx_new(macro);
}

/* ENDM.  Finish a macro definition.  If an argument appears, it must match the
 * name of the macro being defined. */

/* TODO: argument not actually checked yet */

static void pseudo_endm(struct prog_line *line) {
	int nargs = node_array_count(line->args);
	struct node **arga = node_array_of(line->args);
	if (nargs > 1) {
		error(error_type_syntax, "invalid number of arguments to ENDM");
		return;
	}
	if (nargs > 0 && node_type_of(arga[0]) != node_type_string) {
		error(error_type_syntax, "invalid argument to ENDM");
		return;
	}
	if (!defining_macro_ctx)
		return;
	prog_ctx_free(defining_macro_ctx);
	defining_macro_ctx = NULL;
}

/* END.  Specify EXEC address. */

static void pseudo_end(struct prog_line *line) {
	int nargs = verify_num_args(line->args, 0, 1, "END");
	if (nargs < 1)
		return;
	struct node **arga = node_array_of(line->args);
	symbol_set(".exec", arga[0], asm_pass, 0);
}

/* Ignore certain historical pseudo-ops */

static void pseudo_nop(struct prog_line *line) {
	(void)line;
}
