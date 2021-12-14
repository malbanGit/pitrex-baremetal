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
#include <stdlib.h>

#include "array.h"
#include "asm6809.h"
#include "assemble.h"
#include "error.h"
#include "eval.h"
#include "instr.h"
#include "node.h"
#include "opcode.h"
#include "register.h"
#include "section.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Inherent instructions.  No arguments.
 */

void instr_inherent(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	if (nargs != 0) {
		error(error_type_syntax, "unexpected argument");
	}
	section_emit_op(op->immediate);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Inherent.
 */

void instr_immediate(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (nargs != 1) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}
	section_emit_op(op->immediate);
	struct node *tmp = eval_int(arga[0]);
	uint32_t value = 0;
	if (tmp) {
		value = tmp->data.as_int;
		node_free(tmp);
	}
	if ((op->type & OPCODE_EXT_TYPE) == OPCODE_IMM8)
		section_emit_uint8(value);
	else if ((op->type & OPCODE_EXT_TYPE) == OPCODE_IMM16)
		section_emit_uint16(value);
	else
		section_emit_uint32(value);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Relative addressing.
 */

/* Constrain relative values to 16 bit signed - a wrap-around could cause an
 * 8-bit value to become valid.  Only needs to be called if the result is going
 * to be range tested. */

static int to_rel16(int64_t i) {
	while (i < -32768)
		i += 65536;
	while (i > 32767)
		i -= 65536;
	return i;
}

void instr_rel(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (nargs != 1) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}
	section_emit_op(op->immediate);
	if (node_type_of(arga[0]) != node_type_int) {
		if ((op->type & OPCODE_EXT_TYPE) == OPCODE_REL8)
			section_emit_pad(1);
		else
			section_emit_pad(2);
		return;
	}
	int rel8 = to_rel16(arga[0]->data.as_int - (cur_section->pc + 1));
	_Bool rel8v = (rel8 < -128 || rel8 > 127);
	if ((op->type & OPCODE_EXT_TYPE) == OPCODE_REL8) {
		if (rel8v)
			error(error_type_out_of_range, "8-bit relative value out of range");
		section_emit_uint8(rel8);
	} else {
		if (!rel8v && node_attr_of(arga[0]) != node_attr_16bit)
			error(error_type_inefficient, "16-bit relative value could be represented in 8 bits");
		section_emit_uint16(rel8 - 1);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Indexed addressing.
 */

/* Table of 2-arg indexed addressing modes. */

enum off_type {
	off_type_none,
	off_type_zero,
	off_type_5bit,
	off_type_8bit,
	off_type_16bit,
	off_type_reg_a,
	off_type_reg_b,
	off_type_reg_d,
	off_type_reg_e,
	off_type_reg_f,
	off_type_reg_w,
};

enum idx_type {
	idx_type_xyus,
	idx_type_pcr,
	idx_type_w,
};

enum idx_indirect {
	idx_indirect_ok,
	idx_indirect_ok_6309,  // postbyte += 1, rather than postbyte += 0x10
	idx_indirect_impossible,
	idx_indirect_illegal,
};

static struct {
	enum idx_type idx_type;
	enum node_attr idx_attr;
	enum off_type off_type;
	uint8_t postbyte;
	enum idx_indirect idx_indirect;
} const indexed_modes[] = {
	{ idx_type_xyus, node_attr_none,     off_type_zero,  0x84, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_5bit,  0x00, idx_indirect_impossible },
	{ idx_type_xyus, node_attr_none,     off_type_8bit,  0x88, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_16bit, 0x89, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_reg_a, 0x86, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_reg_b, 0x85, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_reg_d, 0x8b, idx_indirect_ok },
	{ idx_type_xyus, node_attr_postinc,  off_type_none,  0x80, idx_indirect_illegal },
	{ idx_type_xyus, node_attr_postinc2, off_type_none,  0x81, idx_indirect_ok },
	{ idx_type_xyus, node_attr_predec,   off_type_none,  0x82, idx_indirect_illegal },
	{ idx_type_xyus, node_attr_predec2,  off_type_none,  0x83, idx_indirect_ok },
	{ idx_type_pcr,  node_attr_none,     off_type_8bit,  0x8c, idx_indirect_ok },
	{ idx_type_pcr,  node_attr_none,     off_type_16bit, 0x8d, idx_indirect_ok },
	/* 6309 extensions */
	{ idx_type_xyus, node_attr_none,     off_type_reg_e, 0x87, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_reg_f, 0x8a, idx_indirect_ok },
	{ idx_type_xyus, node_attr_none,     off_type_reg_w, 0x8e, idx_indirect_ok },
	{ idx_type_w,    node_attr_none,     off_type_none,  0x8f, idx_indirect_ok_6309 },
	{ idx_type_w,    node_attr_none,     off_type_16bit, 0xaf, idx_indirect_ok_6309 },
	{ idx_type_w,    node_attr_postinc2, off_type_none,  0xcf, idx_indirect_ok_6309 },
	{ idx_type_w,    node_attr_predec2,  off_type_none,  0xef, idx_indirect_ok_6309 },
};
#define NUM_INDEXED_MODES ARRAY_N_ELEMENTS(indexed_modes)

static _Bool off_type_compatible(enum off_type off_type, _Bool pcr, struct node const *n) {
	enum node_type ntype = node_type_of(n);
	enum node_attr nattr = node_attr_of(n);

	switch (nattr) {
	case node_attr_none:
		break;
	case node_attr_5bit:
		return off_type == off_type_5bit;
	case node_attr_8bit:
		return off_type == off_type_8bit;
	case node_attr_16bit:
		return off_type == off_type_16bit;
	default:
		return 0;
	}

	switch (off_type) {
	case off_type_none:
		return ntype == node_type_empty;
	case off_type_zero:
		if (ntype == node_type_int)
			return n->data.as_int == 0;
		return ntype == node_type_empty;
	case off_type_5bit:
		if (ntype == node_type_int) {
			return n->data.as_int >= -16 && n->data.as_int <= 15;
		}
		return ntype == node_type_empty;
	case off_type_8bit:
		if (ntype == node_type_int) {
			int64_t val_int = n->data.as_int;
			if (pcr)
				val_int = to_rel16(val_int - (cur_section->pc + 2));
			return val_int >= -128 && val_int <= 127;
		}
		return ntype == node_type_empty;
	case off_type_16bit:
		return ntype == node_type_int || ntype == node_type_empty;
	case off_type_reg_a:
		return ntype == node_type_reg && n->data.as_reg == REG_A;
	case off_type_reg_b:
		return ntype == node_type_reg && n->data.as_reg == REG_B;
	case off_type_reg_d:
		return ntype == node_type_reg && n->data.as_reg == REG_D;
	case off_type_reg_e:
		return ntype == node_type_reg && n->data.as_reg == REG_E;
	case off_type_reg_f:
		return ntype == node_type_reg && n->data.as_reg == REG_F;
	case off_type_reg_w:
		return ntype == node_type_reg && n->data.as_reg == REG_W;
	default:
		break;
	}

	return 0;
}

static void instr_indexed2(_Bool indirect, struct node const *arg0, struct node const *arg1) {

	enum node_type arg0_type = node_type_of(arg0);
	enum node_attr arg0_attr = node_attr_of(arg0);
	enum node_type arg1_type = node_type_of(arg1);
	enum node_attr arg1_attr = node_attr_of(arg1);

	if (arg0_type == node_type_undef || arg1_type == node_type_undef) {
		section_emit_pad(1);
		return;
	}

	if (arg1_type != node_type_reg)
		goto invalid_mode;

	enum idx_type idx_type;
	uint8_t idx_select = 0;
	_Bool pcr = 0;

	switch (arg1->data.as_reg) {
	case REG_X:
		idx_type = idx_type_xyus;
		idx_select = 0x00;
		break;
	case REG_Y:
		idx_type = idx_type_xyus;
		idx_select = 0x20;
		break;
	case REG_U:
		idx_type = idx_type_xyus;
		idx_select = 0x40;
		break;
	case REG_S:
		idx_type = idx_type_xyus;
		idx_select = 0x60;
		break;
	case REG_PC:
		idx_type = idx_type_pcr;
		pcr = 0;
		break;
	case REG_PCR:
		idx_type = idx_type_pcr;
		pcr = 1;
		break;
	case REG_W:
		idx_type = idx_type_w;
		break;
	default:
		goto invalid_mode;
	}

	if (arg0_type == node_type_int && arg0_attr == node_attr_none
	    && arg0->data.as_int == 0) {
		arg0_type = node_type_empty;
	}

	enum off_type off_type;
	enum idx_indirect idx_indirect;
	int postbyte = -1;
	for (unsigned i = 0; i < NUM_INDEXED_MODES; i++) {
		if (indexed_modes[i].idx_type != idx_type)
			continue;
		if (indexed_modes[i].idx_attr != arg1_attr)
			continue;
		off_type = indexed_modes[i].off_type;
		if (!off_type_compatible(off_type, pcr, arg0))
			continue;
		idx_indirect = indexed_modes[i].idx_indirect;
		if (indirect && idx_indirect == idx_indirect_impossible)
			continue;
		postbyte = indexed_modes[i].postbyte;
		break;
	}

	if (postbyte == -1)
		goto invalid_mode;

	postbyte |= idx_select;

	int64_t off_value = 0;
	if (arg0_type == node_type_int) {
		off_value = arg0->data.as_int;
		if (pcr) {
			if (off_type == off_type_8bit) {
				off_value -= (cur_section->pc + 2);
			} else if (off_type == off_type_16bit) {
				off_value -= (cur_section->pc + 3);
			}
		}
	}

	if (off_type == off_type_5bit)
		postbyte |= (off_value & 0x1f);

	if (indirect) {
		// impossible dealt with above
		if (idx_indirect == idx_indirect_illegal)
			error(error_type_illegal, "illegal indirect indexed mode");
		if (idx_indirect == idx_indirect_ok_6309)
			postbyte++;
		else
			postbyte |= 0x10;
	}

	section_emit_uint8(postbyte);

	switch (off_type) {
	case off_type_8bit:
		section_emit_uint8(off_value);
		break;
	case off_type_16bit:
		section_emit_uint16(off_value);
		break;
	default:
		break;
	}

	return;

invalid_mode:
	error(error_type_syntax, "invalid addressing mode");
	return;
}

void instr_indexed(struct opcode const *op, struct node const *args, int imm8_val) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	int indirect = 0;
	if (nargs == 1 && node_type_of(arga[0]) == node_type_array) {
		indirect = 0x10;
		nargs = node_array_count(arga[0]);
		arga = arga[0]->data.as_array.args;
	}
	if (nargs < 1 || nargs > 2) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}
	section_emit_op(op->indexed);
	if (imm8_val >= 0)
		section_emit_uint8(imm8_val);

	if (node_type_of(arga[0]) == node_type_string) {
		struct node *tmp = eval_int(arga[0]);
		node_free(arga[0]);
		arga[0] = tmp;
	}

	if (nargs == 1) {
		int pbyte = indirect ? 0x9f : 0x8f;
		int addr = 0;
		switch (node_type_of(arga[0])) {
		case node_type_undef:
			break;
		case node_type_int:
			addr = arga[0]->data.as_int;
			break;
		default:
			error(error_type_syntax, "invalid addressing mode");
			return;
		}
		switch (node_attr_of(arga[0])) {
		case node_attr_undef:
		case node_attr_none:
		case node_attr_16bit:
			break;
		default:
			error(error_type_syntax, "invalid addressing mode");
			return;
		}
		if (!indirect)
			error(error_type_illegal, "illegal indexed addressing form");
		section_emit_uint8(pbyte);
		section_emit_uint16(addr);
		return;
	}

	instr_indexed2(indirect, arga[0], arga[1]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * Direct and extended addressing.
 */

void instr_address(struct opcode const *op, struct node const *args, int imm8_val) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (nargs < 1 || nargs > 2 || (!(op->type & OPCODE_INDEXED) && nargs > 1)) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}
	if (nargs == 2 || (nargs == 1 && node_type_of(arga[0]) == node_type_array)) {
		instr_indexed(op, args, imm8_val);
		return;
	}

	struct node *arg = eval_int(arga[0]);
	enum node_attr attr = node_attr_16bit;
	unsigned addr = cur_section->pc;
	if (arg) {
		attr = arg->attr;
		addr = arg->data.as_int & 0xffff;
		node_free(arg);
	}

	if ((op->type & OPCODE_DIRECT)) {
		if (attr == node_attr_8bit ||
		    (attr == node_attr_none && (cur_section->dp == (addr >> 8)))) {
			section_emit_op(op->direct);
			if (imm8_val >= 0)
				section_emit_uint8(imm8_val);
			section_emit_uint8(addr);
			return;
		}
	}

	if ((op->type & OPCODE_EXTENDED)) {
		if (attr == node_attr_16bit || attr == node_attr_none) {
			section_emit_op(op->extended);
			if (imm8_val >= 0)
				section_emit_uint8(imm8_val);
			section_emit_uint16(addr);
			return;
		}
	}

	instr_indexed(op, args, imm8_val);
}

/*
 * 6309-specific 8-bit immediate in-memory instructions.
 */

void instr_imm8_mem(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (nargs < 2 || nargs > 3 || (!(op->type & OPCODE_INDEXED) && nargs > 2)) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}
	int imm8_val = 0;
	if (node_type_of(arga[0]) == node_type_int)
		imm8_val = arga[0]->data.as_int;
	struct node *newa = node_new_array();
	newa->data.as_array.nargs = nargs - 1;
	newa->data.as_array.args = arga + 1;
	instr_address(op, newa, (uint8_t)imm8_val);
	newa->data.as_array.nargs = 0;
	newa->data.as_array.args = NULL;
	node_free(newa);
}

/*
 * Stack operations.  Operands are a list of registers.  e.g., PSHS, PULU.
 */

void instr_stack(struct opcode const *op, struct node const *args, enum reg_id stack) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);

	int pbyte = 0;
	for (int i = 0; i < nargs; i++) {
		if (node_type_of(arga[i]) != node_type_reg) {
			error(error_type_syntax, "invalid argument");
			return;
		}
		if (arga[i]->data.as_reg == stack)
			goto invalid_register;
		uint8_t bit = reg_stack_bit[arga[i]->data.as_reg];
		if (bit == 0)
			goto invalid_register;
		pbyte |= bit;
	}

	section_emit_op(op->immediate);
	section_emit_uint8(pbyte);
	return;

invalid_register:
	error(error_type_syntax, "invalid register in stack operation");
	return;
}

/*
 * Instructions operating on a pair of registers.  e.g., TFR & EXG.
 */

void instr_pair(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node * const *arga = node_array_of(args);
	if (nargs != 2) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}

	int nibble[2];
	int type[2];
	int size[2];
	for (int i = 0; i < 2; i++) {
		type[i] = node_type_of(arga[i]);
		if (type[i] == node_type_reg) {
			nibble[i] = reg_tfr_nibble[arga[i]->data.as_reg];
			size[i] = reg_tfr_size[arga[i]->data.as_reg];
		} else if (type[i] == node_type_int) {
			int v = arga[i]->data.as_int;
			if (v == 0 && asm6809_options.isa == asm6809_isa_6309) {
				nibble[i] = 0x0c;
				size[i] = 16 | 8;
			} else {
				error(error_type_illegal, "numerical values used in inter-register op");
				if (v < 0 || v > 15) {
					error(error_type_out_of_range, "numeric value for register out of range");
				}
				nibble[i] = v & 15;
				// don't warn about size as well if numeric...
				size[i] = 16 | 8;
			}
		} else if (type[i] != node_type_undef) {
			error(error_type_syntax, "invalid register in inter-register op");
			return;
		}
	}
	if ((size[0] & size[1]) == 0) {
		error(error_type_illegal, "register size mismatch in inter-register op");
	}

	int pbyte = (nibble[0] << 4) | nibble[1];
	section_emit_op(op->immediate);
	section_emit_uint8(pbyte);
	return;
}

/*
 * 6309 extension TFM.
 */

void instr_tfm(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node * const *arga = node_array_of(args);
	if (nargs != 2) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}

	int nibble[2];
	int type[2];
	int attr[2];
	for (int i = 0; i < 2; i++) {
		type[i] = node_type_of(arga[i]);
		attr[i] = node_attr_of(arga[i]);
		if (type[i] == node_type_reg) {
			int v = reg_tfr_nibble[arga[i]->data.as_reg];
			if (v < 0 || v > 4) {
				error(error_type_syntax, "invalid register in TFM op");
				return;
			}
			nibble[i] = v;
		} else if (type[i] == node_type_int) {
			int v = arga[i]->data.as_int;
			error(error_type_illegal, "numerical values used in TFM op");
			if (v < 0 || v > 4) {
				error(error_type_out_of_range, "numeric value for register out of range");
			}
			nibble[i] = v & 15;
		} else if (type[i] != node_type_undef) {
			error(error_type_syntax, "invalid register in inter-register op");
			return;
		}
	}

	unsigned pbyte = (nibble[0] << 4) | nibble[1];
	unsigned mod = 0;
	if (attr[0] == node_attr_postinc && attr[1] == node_attr_postinc) {
		mod = 0;
	} else if (attr[0] == node_attr_postdec && attr[1] == node_attr_postdec) {
		mod = 1;
	} else if (attr[0] == node_attr_postinc && attr[1] == node_attr_none) {
		mod = 2;
	} else if (attr[0] == node_attr_none && attr[1] == node_attr_postinc) {
		mod = 3;
	} else {
		error(error_type_syntax, "invalid TFM mode");
		return;
	}

	section_emit_op(op->immediate + mod);
	section_emit_uint8(pbyte);
	return;
}

/*
 * Logical operations between specified bits from a register and a direct
 * address.  6309 extended instructions.
 */

void instr_reg_mem(struct opcode const *op, struct node const *args) {
	int nargs = node_array_count(args);
	struct node **arga = node_array_of(args);
	if (nargs != 4) {
		error(error_type_syntax, "invalid number of arguments");
		return;
	}

	int pbyte = 0xc0;
	switch (node_type_of(arga[0])) {
	case node_type_undef:
		break;
	case node_type_int:
		pbyte |= (arga[0]->data.as_int & 3) << 6;
		error(error_type_illegal, "numerical value used in place of register");
		break;
	case node_type_reg:
		switch (arga[0]->data.as_reg) {
		case REG_CC:
			pbyte = 0;
			break;
		case REG_A:
			pbyte = 0x40;
			break;
		case REG_B:
			pbyte = 0x80;
			break;
		default:
			goto invalid_register;
		}
		break;
	default:
		goto invalid_register;
	}

	if (node_type_of(arga[1]) == node_type_int) {
		if (arga[1]->data.as_int < 0 || arga[1]->data.as_int > 7) {
			error(error_type_out_of_range, "bad source bit");
		} else {
			pbyte |= (arga[1]->data.as_int << 3);
		}
	}
	if (node_type_of(arga[2]) == node_type_int) {
		if (arga[2]->data.as_int < 0 || arga[2]->data.as_int > 7) {
			error(error_type_out_of_range, "bad destination bit");
		} else {
			pbyte |= arga[2]->data.as_int;
		}
	}

	section_emit_op(op->direct);
	section_emit_uint8(pbyte);
	if (node_type_of(arga[3]) == node_type_int) {
		section_emit_uint8(arga[3]->data.as_int);
	} else {
		section_emit_pad(1);
	}
	return;

invalid_register:
	error(error_type_syntax, "invalid register in register-memory operation");
	return;
}
