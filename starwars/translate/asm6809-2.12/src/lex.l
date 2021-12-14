%{
/*

asm6809, a Motorola 6809 cross assembler
Copyright 2013-2017 Ciaran Anscomb

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

*/

#include "config.h"

#include <stdio.h>
#include <string.h>

#include "xalloc.h"

#include "error.h"
#include "register.h"
#include "slist.h"

#include "grammar.h"

static int delim;

static int id_or_reg(void);
static int read_line(char *buf, int max_size);

char *lex_fetch_line(void);
void lex_free_all(void);

#define YY_INPUT(buf,result,max_size) \
	{ result = read_line(buf, max_size); }

%}

%option noyywrap
%option noinput
%option nounput

%s opcode
%s arg
%s argnostr
%s argnostrnum
%s string

nzdigit	[1-9]
digit	[0-9]
octdigit	[0-7]
bindigit	[01]
hexdigit	[0-9A-Fa-f]
ws	[ \t]

decimal	{digit}+
word	[A-Za-z_][A-Za-z0-9_\.]*
rword	[A-Za-z0-9_\.]+

%%

<INITIAL>{

\%{bindigit}+	{ yylval.as_int = strtoimax(yytext+1, NULL, 2); return INTEGER; }
0b{bindigit}+	{ yylval.as_int = strtoimax(yytext+2, NULL, 2); return INTEGER; }
@{octdigit}+	{ yylval.as_int = strtoimax(yytext+1, NULL, 8); return INTEGER; }
0{octdigit}+	{ yylval.as_int = strtoimax(yytext, NULL, 8); return INTEGER; }
{decimal}	{ yylval.as_int = strtoimax(yytext, NULL, 10); return INTEGER; }
${hexdigit}+	{ yylval.as_int = strtoimax(yytext+1, NULL, 16); return INTEGER; }
0x{hexdigit}+	{ yylval.as_int = strtoimax(yytext+2, NULL, 16); return INTEGER; }
'.'		{ yylval.as_int = *(yytext+1); return INTEGER; }
'.		{ yylval.as_int = *(yytext+1); return INTEGER; }
\!		{ yylval.as_int = 0; return INTEGER; }

{word}		{ return id_or_reg(); }
&{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
&\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }
\\{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
\\\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }

{ws}*[;\*].*	/* ";" or "*" introduces comment to end of line */
{ws}+		{ BEGIN(opcode); return WS; }
\r		/* skip CR */
{ws}*\n		{ return '\n'; }
.		{ return *yytext; }

}

<opcode>{

{word}		{ return id_or_reg(); }
&{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
&\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }
\\{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
\\\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }


{ws}*;.*	/* ";" introduces comment to end of line */
{ws}+		{ BEGIN(arg); return WS; }
{ws}*\n		{ BEGIN(INITIAL); return '\n'; }
\r		/* skip CR */
.		{ BEGIN(INITIAL); return *yytext; }

}

<arg>{

[/"]		{
			delim = *yytext;
			BEGIN(string);
			return DELIM;
		}

}

<argnostr,argnostrnum>{

\/		{ BEGIN(argnostr); return *yytext; }

}

<arg,argnostr>{

{word}		{ BEGIN(argnostr); return id_or_reg(); }

}

<argnostrnum>{

{rword}		{ BEGIN(argnostrnum); yylval.as_string = strndup(yytext, yyleng); return ID; }

}

<arg,argnostr,argnostrnum>{

&\{{decimal}\}	{ BEGIN(argnostrnum); yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }
\\{digit}	{ BEGIN(argnostrnum); yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
\\\{{decimal}\}	{ BEGIN(argnostrnum); yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }

\+\+		{ BEGIN(argnostr); return INC2; }
\-\-		{ BEGIN(argnostr); return DEC2; }

{decimal}[bB]	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext, NULL, 10); return BACKREF; }
{decimal}[fF]	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext, NULL, 10); return FWDREF; }

\%{bindigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext+1, NULL, 2); return INTEGER; }
0b{bindigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext+2, NULL, 2); return INTEGER; }
@{octdigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext+1, NULL, 8); return INTEGER; }
0{octdigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext, NULL, 8); return INTEGER; }
{decimal}	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext, NULL, 10); return INTEGER; }
${hexdigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext+1, NULL, 16); return INTEGER; }
0x{hexdigit}+	{ BEGIN(argnostr); yylval.as_int = strtoimax(yytext+2, NULL, 16); return INTEGER; }
'.'		{ BEGIN(argnostr); yylval.as_int = *(yytext+1); return INTEGER; }
'.		{ BEGIN(argnostr); yylval.as_int = *(yytext+1); return INTEGER; }
[0-9]*\.[0-9]+	|
{decimal}\.	{ BEGIN(argnostr); yylval.as_float = strtod(yytext, NULL); return FLOAT; }

"<<"		{ BEGIN(arg); return SHL; }
">>"		{ BEGIN(arg); return SHR; }

"<="		{ BEGIN(arg); return LE; }
">="		{ BEGIN(arg); return GE; }
"=="		{ BEGIN(arg); return EQ; }
"!="		{ BEGIN(arg); return NE; }
"&&"		{ BEGIN(arg); return LAND; }
"||"		{ BEGIN(arg); return LOR; }

[\!#\%&(\*+\-\:<>\?\[\^\|\~]	{ BEGIN(arg); return *yytext; }
[)\]]		{ BEGIN(argnostr); return *yytext; }

,		{ BEGIN(arg); return *yytext; }

{ws}*;.*	/* ";" introduces comment to end of line */
{ws}+		/* skip whitespace */
\r		/* skip CR */
\n		{ BEGIN(INITIAL); return *yytext; }
.		{ return *yytext; }

}

<string>{

[/"]		{
			if (*yytext == delim) {
				BEGIN(argnostr);
				return DELIM;
			} else {
				yylval.as_string = strndup(yytext, yyleng);
				return TEXT;
			}
		}

&{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
&\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }
&&		|
&		{ yylval.as_string = strndup("&", 1); return TEXT; }
\\{digit}	{ yylval.as_string = strndup(yytext+1, yyleng-1); return INTERP; }
\\\{{decimal}\}	{ yylval.as_string = strndup(yytext+2, yyleng-3); return INTERP; }

\\n		{ yylval.as_string = strndup("\n", 1); return TEXT; }
\\r		{ yylval.as_string = strndup("\r", 1); return TEXT; }
\\.		{ yylval.as_string = strndup(yytext+1, 1); return TEXT; }

[^\n\r"&\/\\]+	{ yylval.as_string = strndup(yytext, yyleng); return TEXT; }

\r		/* skip CR */
\n		{ BEGIN(INITIAL); return '\n'; }

}

%%

static int id_or_reg(void) {
	enum reg_id r = reg_name_to_id(yytext);
	if (r != REG_INVALID) {
		yylval.as_reg = r;
		return REGISTER;
	}
	yylval.as_string = strndup(yytext, yyleng);
	return ID;
}

/*
 * Redefining YY_INPUT above, flex reads a line at a time.  Copies of lines are
 * cached, to be fetched by the grammar parser and associated with the parsed
 * data.
 */

static struct slist *lines = NULL;
static char *line = NULL;
static int line_length = 0;
static _Bool need_eol = 1;

static int read_line(char *buf, int max_size) {
	if (!yyin)
		return YY_NULL;
	char *r = fgets(buf, max_size, yyin);
	if (!r) {
		if (!feof(yyin)) {
			error(error_type_fatal, "scanner read failed");
		}
		if (need_eol) {
			buf[0] = '\n';
			buf[1] = 0;
		} else {
			if (line) {
				free(line);
				line = NULL;
				line_length = 0;
			}
			return YY_NULL;
		}
	}

	int len = strlen(buf);
	if (len > 1 && buf[len-1] == '\n' && buf[len-2] == '\r') {
		buf[len-2] = '\n';
		len--;
	}
	line = xrealloc(line, line_length + len + 1);
	memcpy(line + line_length, buf, len + 1);
	line_length = line_length + len;
	need_eol = 1;

	if (line_length > 0 && line[line_length-1] == '\n') {
		need_eol = 0;
		line[line_length-1] = 0;
		lines = slist_append(lines, line);
		line = NULL;
		line_length = 0;
	}

	return len;
}

char *lex_fetch_line(void) {
	if (!lines) {
		error(error_type_fatal, "internal: line copy fetched before ready");
		return NULL;
	}
	char *l = lines->data;
	lines = slist_remove(lines, l);
	return l;
}

void lex_free_all(void) {
	while (lines) {
		char *l = lines->data;
		lines = slist_remove(lines, l);
		free(l);
	}
	yylex_destroy();
}
