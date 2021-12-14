/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_GRAMMAR_H_INCLUDED
# define YY_YY_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    WS = 258,
    ID = 259,
    INTERP = 260,
    FLOAT = 261,
    INTEGER = 262,
    BACKREF = 263,
    FWDREF = 264,
    REGISTER = 265,
    TEXT = 266,
    SHL = 267,
    SHR = 268,
    LE = 269,
    GE = 270,
    EQ = 271,
    NE = 272,
    LOR = 273,
    LAND = 274,
    DELIM = 275,
    DEC2 = 276,
    INC2 = 277,
    UMINUS = 278
  };
#endif
/* Tokens.  */
#define WS 258
#define ID 259
#define INTERP 260
#define FLOAT 261
#define INTEGER 262
#define BACKREF 263
#define FWDREF 264
#define REGISTER 265
#define TEXT 266
#define SHL 267
#define SHR 268
#define LE 269
#define GE 270
#define EQ 271
#define NE 272
#define LOR 273
#define LAND 274
#define DELIM 275
#define DEC2 276
#define INC2 277
#define UMINUS 278

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 43 "grammar.y" /* yacc.c:1909  */

	int as_token;
	int64_t as_int;
	double as_float;
	char *as_string;
	enum reg_id as_reg;
	struct node *as_node;
	struct prog_line *as_line;
	struct slist *as_list;
	

#line 112 "grammar.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */
