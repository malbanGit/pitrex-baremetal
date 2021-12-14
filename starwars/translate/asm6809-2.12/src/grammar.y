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

#include "c-strcase.h"

#include "error.h"
#include "eval.h"
#include "node.h"
#include "program.h"
#include "register.h"
#include "slist.h"

static void raise_error(void);
static void yyerror(const char *);
void yylex_destroy(void);
int yylex(void);
char *lex_fetch_line(void);
void lex_free_all(void);

extern FILE *yyin;
static struct prog_ctx *cur_ctx = NULL;

struct prog *grammar_parse_file(const char *filename);

static void check_end_opcode(struct prog_line *line);
%}

%union {
	int as_token;
	int64_t as_int;
	double as_float;
	char *as_string;
	enum reg_id as_reg;
	struct node *as_node;
	struct prog_line *as_line;
	struct slist *as_list;
	}

%token WS
%token <as_string> ID INTERP
%token <as_float> FLOAT
%token <as_int> INTEGER BACKREF FWDREF
%token <as_reg> REGISTER
%token <as_string> TEXT
%token <as_token> SHL SHR
%token <as_token> LE GE EQ NE
%token <as_token> LOR LAND
%token DELIM
%token DEC2 INC2

%destructor { free($$); } <as_string>

%type <as_node> label
%type <as_node> id string
%type <as_node> idpart strpart arg
%type <as_node> arglist
%type <as_list> idlist strlist
%type <as_node> expr reg
%type <as_line> line

%right '?' ':'
%left LOR
%left LAND
%left '|'
%left '^'
%left '&'
%left '<' LE '>' GE
%left EQ NE
%left SHL SHR
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS '!' '~'
%left '(' ')'

%%

program	:
	| program line	{ prog_line_set_text($2, lex_fetch_line()); prog_ctx_add_line(cur_ctx, $2); check_end_opcode($2); }
	| program error '\n'	{ raise_error(); yyerrok; }
	;

line	: label WS id WS arglist '\n'	{ $$ = prog_line_new($1, $3, $5); }
	| label WS id WS arglist error '\n'	{ $$ = prog_line_new($1, $3, $5); }
	| label WS id '\n'		{ $$ = prog_line_new($1, $3, NULL); }
	| label WS id error '\n'	{ $$ = prog_line_new($1, $3, NULL); }
	| label '\n'			{ $$ = prog_line_new($1, NULL, NULL); }
	;

label	:			{ $$ = NULL; }
	| INTEGER		{ $$ = node_new_int($1); }
	| id			{ $$ = $1; }
	;

id	: idlist		{ $$ = node_new_id($1); }
	;

idlist	: idpart		{ $$ = slist_append(NULL, $1); }
	| idlist idpart		{ $$ = slist_append($1, $2); }
	;

idpart	: ID			{ $$ = node_new_string($1); }
      	| INTERP		{ $$ = node_new_interp($1); }
	;

arglist	: arg			{ $$ = node_array_push(NULL, $1); }
	| arglist ',' arg	{ $$ = node_array_push($1, $3); }
	;

arg	:			{ $$ = node_new_empty(); }
	| '[' arglist ']'	{ $$ = $2; }
	| '#' expr		{ $$ = node_set_attr($2, node_attr_immediate); }
	| SHL expr		{ $$ = node_set_attr($2, node_attr_5bit); }
	| '<' expr		{ $$ = node_set_attr($2, node_attr_8bit); }
	| '>' expr		{ $$ = node_set_attr($2, node_attr_16bit); }
	| '<'			{ $$ = node_new_backref(0); }
	| '>'			{ $$ = node_new_fwdref(0); }
	| reg '+'		{ $$ = node_set_attr($1, node_attr_postinc); }
	| reg INC2		{ $$ = node_set_attr($1, node_attr_postinc2); }
	| '-' reg		{ $$ = node_set_attr($2, node_attr_predec); }
	| DEC2 reg		{ $$ = node_set_attr($2, node_attr_predec2); }
	| reg '-'		{ $$ = node_set_attr($1, node_attr_postdec); }
	| reg			{ $$ = $1; }
	| expr			{ $$ = $1; }
	;

reg	: REGISTER		{ $$ = node_new_reg($1); }
	;

expr	: '(' expr ')'		{ $$ = $2; }
	| '-' expr %prec UMINUS	{ $$ = node_new_oper_1('-', $2); }
	| '+' expr %prec UMINUS	{ $$ = node_new_oper_1('+', $2); }
	| '~' expr		{ $$ = node_new_oper_1('~', $2); }
	| '!' expr		{ $$ = node_new_oper_1('!', $2); }
	| expr '*' expr		{ $$ = node_new_oper_2('*', $1, $3); }
	| expr '/' expr		{ $$ = node_new_oper_2('/', $1, $3); }
	| expr '%' expr		{ $$ = node_new_oper_2('%', $1, $3); }
	| expr '+' expr		{ $$ = node_new_oper_2('+', $1, $3); }
	| expr '-' expr		{ $$ = node_new_oper_2('-', $1, $3); }
	| expr SHL expr		{ $$ = node_new_oper_2(SHL, $1, $3); }
	| expr SHR expr		{ $$ = node_new_oper_2(SHR, $1, $3); }
	| expr '<' expr		{ $$ = node_new_oper_2('<', $1, $3); }
	| expr LE expr		{ $$ = node_new_oper_2(LE, $1, $3); }
	| expr '>' expr		{ $$ = node_new_oper_2('>', $1, $3); }
	| expr GE expr		{ $$ = node_new_oper_2(GE, $1, $3); }
	| expr EQ expr		{ $$ = node_new_oper_2(EQ, $1, $3); }
	| expr NE expr		{ $$ = node_new_oper_2(NE, $1, $3); }
	| expr '&' expr		{ $$ = node_new_oper_2('&', $1, $3); }
	| expr '^' expr		{ $$ = node_new_oper_2('^', $1, $3); }
	| expr '|' expr		{ $$ = node_new_oper_2('|', $1, $3); }
	| expr LAND expr	{ $$ = node_new_oper_2(LAND, $1, $3); }
	| expr LOR expr		{ $$ = node_new_oper_2(LOR, $1, $3); }
	| expr '?' expr ':' expr	{ $$ = node_new_oper_3('?', $1, $3, $5); }
	| INTEGER		{ $$ = node_new_int($1); }
	| FLOAT			{ $$ = node_new_float($1); }
	| BACKREF		{ $$ = node_new_backref($1); }
	| FWDREF		{ $$ = node_new_fwdref($1); }
	| '*'			{ $$ = node_new_pc(); }
	| string		{ $$ = $1; }
	| id			{ $$ = $1; }
	;

string	: DELIM strlist DELIM	{ $$ = node_new_text($2); }
	;

strlist	: strpart		{ $$ = slist_append(NULL, $1); }
	| strlist strpart	{ $$ = slist_append($1, $2); }
	;

strpart	: TEXT			{ $$ = node_new_string($1); }
	| INTERP		{ $$ = node_new_interp($1); }
	;

%%

static void raise_error(void) {
	// discard line with error - going to fail anyway
	char *l = lex_fetch_line();
	free(l);
	cur_ctx->line_number++;
	error(error_type_syntax, "");
}

static void yyerror(const char *s) {
	(void)s;
}

struct prog *grammar_parse_file(const char *filename) {
	yyin = fopen(filename, "r");
	if (!yyin) {
		error(error_type_fatal, "file not found: %s", filename);
		return NULL;
	}
	struct prog *prog = prog_new(prog_type_file, filename);
	cur_ctx = prog_ctx_new(prog);
	yyparse();
	prog_ctx_free(cur_ctx);
	cur_ctx = NULL;
	if (yyin)
		fclose(yyin);
	lex_free_all();
	return prog;
}

static void check_end_opcode(struct prog_line *line) {
	struct node *n = eval_string(line->opcode);
	if (n) {
		if (0 == c_strcasecmp("end", n->data.as_string)) {
			fclose(yyin);
			yyin = NULL;
		}
		node_free(n);
	}
}
