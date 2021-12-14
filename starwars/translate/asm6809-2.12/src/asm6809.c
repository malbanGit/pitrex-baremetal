/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

/* for getopt_long */
#define _GNU_SOURCE

#include "config.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

#include "asm6809.h"
#include "assemble.h"
#include "error.h"
#include "listing.h"
#include "node.h"
#include "opcode.h"
#include "output.h"
#include "program.h"
#include "section.h"
#include "slist.h"
#include "symbol.h"

struct asm6809_options asm6809_options;

#define OUTPUT_BINARY (0)
#define OUTPUT_DRAGONDOS (1)
#define OUTPUT_COCO (2)
#define OUTPUT_MOTOROLA_SREC (3)
#define OUTPUT_INTEL_HEX (4)

static int max_passes = 12;
static int output_format = OUTPUT_BINARY;
static char *exec_option = NULL;
static char *output_filename = NULL;
static char *exports_filename = NULL;
static char *symbol_filename = NULL;
static char *listing_filename = NULL;
static int isa = asm6809_isa_6809;
static int max_program_depth = 8;
static int setdp = -1;
static int verbosity = 0;

static struct option long_options[] = {
	{ "bin", no_argument, &output_format, OUTPUT_BINARY },
	{ "dragondos", no_argument, &output_format, OUTPUT_DRAGONDOS },
	{ "coco", no_argument, &output_format, OUTPUT_COCO },
	{ "srec", no_argument, &output_format, OUTPUT_MOTOROLA_SREC },
	{ "hex", no_argument, &output_format, OUTPUT_INTEL_HEX },
	{ "exec", required_argument, NULL, 'e' },
	{ "6809", no_argument, &isa, asm6809_isa_6809 },
	{ "6309", no_argument, &isa, asm6809_isa_6309 },
	{ "define", required_argument, NULL, 'd' },
	{ "setdp", required_argument, &setdp, 0 },
	{ "max-passes", required_argument, NULL, 'P' },
	{ "output", required_argument, NULL, 'o' },
	{ "listing", required_argument, NULL, 'l' },
	{ "exports", required_argument, NULL, 'E' },
	{ "symbols", required_argument, NULL, 's' },
	{ "quiet", no_argument, NULL, 'q' },
	{ "verbose", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'V' },
	{ NULL, 0, NULL, 0 }
};

static struct slist *files = NULL;

static struct node *simple_parse_int(const char *);
static void define_symbol(const char *);
static void helptext(void);
static void versiontext(void);
static _Noreturn void tidy_up_and_exit(int status);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char **argv) {

	int c;
	while ((c = getopt_long(argc, argv, "BDCSHe:893d:P:o:l:E:s:qv",
				long_options, NULL)) != -1) {
		switch (c) {
		case 0:
			break;
		case 'B':
			output_format = OUTPUT_BINARY;
			break;
		case 'D':
			output_format = OUTPUT_DRAGONDOS;
			break;
		case 'C':
			output_format = OUTPUT_COCO;
			break;
		case 'S':
			output_format = OUTPUT_MOTOROLA_SREC;
			break;
		case 'H':
			output_format = OUTPUT_INTEL_HEX;
			break;
		case 'e':
			exec_option = optarg;
			break;
		case '8': case '9':
			isa = asm6809_isa_6809;
			break;
		case '3':
			isa = asm6809_isa_6309;
			break;
		case 'd':
			define_symbol(optarg);
			break;
		case 'P':
			{
				long v = strtol(optarg, NULL, 0);
				if (errno != 0 || v < 2 || v > 255) {
					error(error_type_fatal, "invalid value for max-passes");
					error_print_list();
					tidy_up_and_exit(EXIT_FAILURE);
				}
				max_passes = v;
			}
			break;
		case 'o':
			output_filename = optarg;
			break;
		case 'l':
			listing_filename = optarg;
			break;
		case 'E':
			exports_filename = optarg;
			break;
		case 's':
			symbol_filename = optarg;
			break;
		case 'q':
			verbosity = -1;
			break;
		case 'v':
			verbosity = 1;
			break;
		case 'h':
			helptext();
			tidy_up_and_exit(EXIT_SUCCESS);
		case 'V':
			versiontext();
			tidy_up_and_exit(EXIT_SUCCESS);
		default:
			tidy_up_and_exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) {
		error(error_type_fatal, "no input files");
		error_print_list();
		tidy_up_and_exit(EXIT_FAILURE);
	}

	asm6809_options.isa = isa;
	asm6809_options.max_program_depth = max_program_depth;
	asm6809_options.setdp = setdp;
	asm6809_options.verbosity = verbosity;
	asm6809_options.listing_required = listing_filename ? 1 : 0;

	opcode_init();
	assemble_init();

	/* Read in each file */
	for (int i = optind; i < argc; i++) {
		struct prog *f = prog_new_file(argv[i]);
		files = slist_append(files, f);
	}

	/* Fatal errors? */
	if (error_level >= error_type_syntax) {
		error_print_list();
		tidy_up_and_exit(EXIT_FAILURE);
	}

	/* Attempt to assemble files until consistent */
	for (unsigned pass = 0; pass < max_passes; pass++) {
		error_clear_all();
		listing_free_all();
		section_set("CODE", pass);
		for (struct slist *l = files; l; l = l->next) {
			struct prog *f = l->data;
			assemble_prog(f, pass);
		}
		section_finish_pass();
		/* Only inconsistencies trigger another pass */
		if (error_level != error_type_inconsistent)
			break;
	}

	/* Fatal errors? */
	if (error_level >= error_type_inconsistent) {
		error_print_list();
		tidy_up_and_exit(EXIT_FAILURE);
	}
	/* Otherwise print any warnings */
	error_print_list();

	/* Generate listing file */
	if (listing_filename) {
		FILE *listf = fopen(listing_filename, "wb");
		if (listf) {
			listing_print(listf);
			fclose(listf);
		} else {
			error(error_type_fatal, "%s: %s", listing_filename, strerror(errno));
		}
	}

	/* Special parsing of option exec address option.  Overrides any use of
	 * the END pseudo-op. */
	if (exec_option) {
		struct node *n = simple_parse_int(exec_option);
		if (!n) {
			unsigned v = 0;
			struct node *tmp = symbol_get(exec_option);
			if (tmp) {
				v = tmp->data.as_int & 0xffff;
				node_free(tmp);
			} else {
				error(error_type_fatal, "exec symbol '%s' not defined", exec_option);
			}
			n = node_new_int(v);
		}
		symbol_force_set(".exec", n, 0, max_passes);
	}

	// XXX At the moment listing generation must precede output, as
	// coelescing spans might screw with the span data to which the listing
	// refers.  Not a big deal, but needs fixing.

	/* Generate output file */
	if (output_filename) {
		switch (output_format) {
		case OUTPUT_BINARY:
			output_binary(output_filename);
			break;
		case OUTPUT_DRAGONDOS:
			output_dragondos(output_filename);
			break;
		case OUTPUT_COCO:
			output_coco(output_filename);
			break;
		case OUTPUT_MOTOROLA_SREC:
			output_motorola_srec(output_filename);
			break;
		case OUTPUT_INTEL_HEX:
			output_intel_hex(output_filename);
			break;
		default:
			error(error_type_fatal, "internal: unexpected output format");
			break;
		}
	}

	/* Generate exports file */
	if (exports_filename) {
		FILE *expf = fopen(exports_filename, "wb");
		if (expf) {
			prog_print_exports(expf);
			fclose(expf);
		} else {
			error(error_type_fatal, "%s: %s", exports_filename, strerror(errno));
		}
	}

	/* Generate symbols file */
	if (symbol_filename) {
		FILE *symf = fopen(symbol_filename, "wb");
		if (symf) {
			prog_print_symbols(symf);
			fclose(symf);
		} else {
			error(error_type_fatal, "%s: %s", symbol_filename, strerror(errno));
		}
	}

	/* Any errors in all that? */
	if (error_level >= error_type_syntax) {
		error_print_list();
		tidy_up_and_exit(EXIT_FAILURE);
	}

	error_print_list();
	tidy_up_and_exit(EXIT_SUCCESS);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* Special parsing of arguments.  Integers only for now. */
static struct node *simple_parse_int(const char *str) {
	struct node *n = NULL;
	if (str[0] == '$') {
		n = node_new_int(strtol(str+1, NULL, 16));
	} else if (str[0] == '@') {
		n = node_new_int(strtol(str+1, NULL, 8));
	} else if (str[0] == '%') {
		n = node_new_int(strtol(str+1, NULL, 2));
	} else if (str[0] == '0') {
		if (str[1] == 'x')
			n = node_new_int(strtol(str+2, NULL, 16));
		else if (str[1] == 'b')
			n = node_new_int(strtol(str+2, NULL, 2));
		else
			n = node_new_int(strtol(str, NULL, 10));
	} else if (str[0] >= '0' && str[0] <= '9') {
		n = node_new_int(strtol(str, NULL, 10));
	}
	return n;
}

static void define_symbol(const char *str) {
	char *key = xstrdup(str);
	char *tmp = strchr(key, '=');
	struct node *value;
	if (tmp) {
		*(tmp++) = 0;
		value = simple_parse_int(tmp);
	} else {
		value = node_new_int(1);
	}
	// TODO: check that key is a valid symbol name
	symbol_set(key, value, 0, 0);
	free(key);
	node_free(value);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void helptext(void) {
	puts(
"Usage: asm6809 [OPTION]... SOURCE-FILE...\n"
"Assembles 6809/6309 source code.\n"
"\n"
"  -B, --bin         output to binary file (default)\n"
"  -D, --dragondos   output to DragonDOS binary file\n"
"  -C, --coco        output to CoCo segmented binary file\n"
"  -S, --srec        output to Motorola SREC file\n"
"  -H, --hex         output to Intel hex record file\n"
"  -e, --exec=ADDR   EXEC address (for output formats that support one)\n"
"\n"
"  -8,\n"
"  -9, --6809                  use 6809 ISA (default)\n"
"  -3, --6309                  use 6309 ISA (6809 with extensions)\n"
"  -d, --define=SYM[=NUMBER]   define a symbol\n"
"      --setdp=VALUE           initial value assumed for DP [undefined]\n"
"\n"
"  -o, --output=FILE    set output filename\n"
"  -l, --listing=FILE   create listing file\n"
"  -E, --exports=FILE   create exports table\n"
"  -s, --symbols=FILE   create symbol table\n"
"\n"
"  -q, --quiet     don't warn about illegal (but working) code\n"
"  -v, --verbose   warn about explicitly inefficient code\n"
"\n"
"      --help      show this help\n"
"      --version   show program version\n"
"\n"
"If more than one SOURCE-FILE is specified, they are assembled as though\n"
"they were all in one file."
	    );
}

static void versiontext(void) {
	puts(
"asm6809 " PACKAGE_VERSION "\n"
"Copyright (C) 2018 Ciaran Anscomb\n"
"License: GNU GPL version 3 or later <http://www.gnu.org/licenses/gpl-3.0.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law."
	);
}

/* Call the various free_all routines to tidy up memory, then exit.  Allows us
 * to see what's been missed with valgrind. */

static _Noreturn void tidy_up_and_exit(int status) {
	if (files) {
		slist_free(files);
		files = NULL;
	}
	listing_free_all();
	prog_free_all();
	symbol_free_all();
	section_free_all();
	opcode_free_all();
	assemble_free_all();
	exit(status);
}
