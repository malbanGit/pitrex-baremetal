/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#ifndef ASM6809_ERROR_H_
#define ASM6809_ERROR_H_

enum error_type {

	error_type_none,

	/*
	 * Warning only.  Inefficient code has been explicitly requested, e.g.,
	 * a long branch used where a short branch would do.  Won't be printed
	 * unless verbosity is increased from default.
	 */
	error_type_inefficient,

	/*
	 * Warning only.  Certain illegal instruction forms known to work on
	 * real hardware are accepted, but the user should know they're
	 * considered bad form.
	 */
	error_type_illegal,

	/*
	 * An inconsistency was found.  This will trigger another pass, up to
	 * the requested maximum, after which they are considered fatal.
	 * - A currently-undefined symbol was referenced.
	 * - The value assigned to a symbol differs from the previous pass.
	 * - A section span (portion of code beginning at ORG) begins or ends
	 *   at a different address to the previous pass.
	 */
	error_type_inconsistent,

	/*
	 * A value was out of range.  This is considered fatal at the end of a
	 * pass only if no inconsistencies were found.
	 */
	error_type_out_of_range,

	/*
	 * Syntax errors are considered fatal.
	 * - Invalid characters in label, opcode or operand.
	 * - Unknown opcode.
	 * - Badly formed expression.
	 * - Invalid or indeterminate addressing mode.
	 * - Invalid register used in instruction.
	 */
	error_type_syntax,

	/*
	 * Data errors found following assembly.
	 * - Overlapping data spans.
	 */
	error_type_data,

	/*
	 * Higher level fatal errors.
	 * - File not found.
	 */
	error_type_fatal,

};

/*
 * The highest level of error found.  Used to decide whether to finish assembly
 * or trigger another pass.
 */

extern enum error_type error_level;

/*
 * Report an error.
 */
void error(enum error_type type, const char *fmt, ...);

/*
 * Report a fatal error, print all errors and abort.
 */

_Noreturn void error_abort(const char *fmt, ...);

/*
 * Clear all errors reported in the previous pass.
 */
void error_clear_all(void);

/*
 * If finishing, this is called to print out the errors found in the last pass.
 */
void error_print_list(void);

#endif
