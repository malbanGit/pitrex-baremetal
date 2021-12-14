/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#ifndef ASM6809_ASM6809_H_
#define ASM6809_ASM6809_H_

enum asm6809_isa {
	asm6809_isa_6809,
	asm6809_isa_6309,
};

struct asm6809_options {
	/* Instruction Set Architecture */
	enum asm6809_isa isa;

	/* Maximum program depth (include files, macros) */
	unsigned max_program_depth;

	/* Initial SETDP */
	int setdp;

	/* Can be positive or negative.  Affects error reporting. */
	int verbosity;

	/* If no listing file is required, don't keep a copy in memory. */
	_Bool listing_required;
};

extern struct asm6809_options asm6809_options;

#endif
