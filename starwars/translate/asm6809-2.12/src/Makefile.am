bin_PROGRAMS = asm6809

AM_CPPFLAGS = \
	-I$(top_builddir)/dt101 \
	-I$(top_srcdir)/dt101 \
	-I$(top_builddir)/gnulib \
	-I$(top_srcdir)/gnulib
AM_YFLAGS = -d

BUILT_SOURCES = \
	grammar.h

asm6809_CFLAGS =
asm6809_LDADD = $(top_builddir)/dt101/libdt101.a $(top_builddir)/gnulib/libgnu.a
asm6809_SOURCES = \
	asm6809.c asm6809.h \
	assemble.c assemble.h \
	error.c error.h \
	eval.c eval.h \
	grammar.y \
	instr.c instr.h \
	interp.c interp.h \
	lex.l \
	listing.c listing.h \
	node.c node.h \
	opcode.c opcode.h \
	output.c output.h \
	program.c program.h \
	register.c register.h \
	section.c section.h \
	symbol.c symbol.h
