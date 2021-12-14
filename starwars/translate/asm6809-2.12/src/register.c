/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <stdlib.h>

#include "c-strcase.h"

#include "array.h"
#include "asm6809.h"
#include "register.h"

struct reg_info {
	const char *name;
	enum reg_id id;
};

static struct reg_info const registers_6809[] = {
	{ .name = "cc", .id = REG_CC },
	{ .name = "a",  .id = REG_A },
	{ .name = "b",  .id = REG_B },
	{ .name = "dp", .id = REG_DP },
	{ .name = "x",  .id = REG_X },
	{ .name = "y",  .id = REG_Y },
	{ .name = "u",  .id = REG_U },
	{ .name = "s",  .id = REG_S },
	{ .name = "pc", .id = REG_PC },
	{ .name = "d",  .id = REG_D },
	/* Program Counter Relative "register" used in indexed addressing: */
	{ .name = "pcr", .id = REG_PCR },
};

static struct reg_info const registers_6309[] = {
	{ .name = "e",  .id = REG_E },
	{ .name = "f",  .id = REG_F },
	{ .name = "w",  .id = REG_W },
	{ .name = "q",  .id = REG_Q },
	{ .name = "v",  .id = REG_V },
};

/* Useful lookup tables.  Indexed by positive reg_id only. */

int8_t const reg_tfr_size[REG_MAX] = {
	8, 8, 8, 8,
	16, 16, 16 ,16, 16, 16,
	-1,
	8, 8, 16, 32, 16
};

int8_t const reg_tfr_nibble[REG_MAX] = {
	0x0a, 0x08, 0x09, 0x0b,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x00,
	-1,
	0x0e, 0x0f, 0x06, -1, 0x07
};

uint8_t const reg_stack_bit[REG_MAX] = {
	0x01, 0x02, 0x04, 0x08,
	0x10, 0x20, 0x40, 0x40, 0x80, 0x06,
	0,
	0, 0, 0, 0, 0
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum reg_id reg_name_to_id(const char *name) {
	switch (asm6809_options.isa) {
	case asm6809_isa_6309:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(registers_6309); i++) {
			if (0 == c_strcasecmp(name, registers_6309[i].name))
				return registers_6309[i].id;
		}
		/* fall through */
	case asm6809_isa_6809:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(registers_6809); i++) {
			if (0 == c_strcasecmp(name, registers_6809[i].name))
				return registers_6809[i].id;
		}
		break;
	default:
		break;
	}
	return REG_INVALID;
}

const char *reg_id_to_name(enum reg_id id) {
	switch (asm6809_options.isa) {
	case asm6809_isa_6309:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(registers_6309); i++) {
			if (id == registers_6309[i].id)
				return registers_6309[i].name;
		}
		/* fall through */
	case asm6809_isa_6809:
		for (unsigned i = 0; i < ARRAY_N_ELEMENTS(registers_6809); i++) {
			if (id == registers_6809[i].id)
				return registers_6809[i].name;
		}
		break;
	default:
		break;
	}
	return NULL;
}
