/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    PROGRAM = 258,
    NAME = 259,
    BEGIN = 260,
    END = 261,
    VAR = 262,
    PROCEDURE = 263,
    STRUCT = 264,
    ASSIGN = 265,
    IF = 266,
    THEN = 267,
    ELSE = 268,
    FI = 269,
    WHILE = 270,
    DO = 271,
    OD = 272,
    RETURN = 273,
    NEW = 274,
    REF = 275,
    DEREF = 276,
    IN = 277,
    FOR = 278,
    TO = 279,
    STEP = 280,
    UNTIL = 281,
    INT_LITERAL = 282,
    FLOAT_LITERAL = 283,
    STRING_LITERAL = 284,
    BOOL_LITERAL = 285,
    NULLVAL = 286,
    INT = 287,
    FLOAT = 288,
    STRING = 289,
    BOOL = 290,
    OR = 291,
    AND = 292,
    NOT = 293,
    COMPARISON = 294,
    SUM = 295,
    MINUS = 296,
    MULT = 297,
    DIV = 298,
    EX = 299,
    UNARYMINUS = 300
  };
#endif
/* Tokens.  */
#define PROGRAM 258
#define NAME 259
#define BEGIN 260
#define END 261
#define VAR 262
#define PROCEDURE 263
#define STRUCT 264
#define ASSIGN 265
#define IF 266
#define THEN 267
#define ELSE 268
#define FI 269
#define WHILE 270
#define DO 271
#define OD 272
#define RETURN 273
#define NEW 274
#define REF 275
#define DEREF 276
#define IN 277
#define FOR 278
#define TO 279
#define STEP 280
#define UNTIL 281
#define INT_LITERAL 282
#define FLOAT_LITERAL 283
#define STRING_LITERAL 284
#define BOOL_LITERAL 285
#define NULLVAL 286
#define INT 287
#define FLOAT 288
#define STRING 289
#define BOOL 290
#define OR 291
#define AND 292
#define NOT 293
#define COMPARISON 294
#define SUM 295
#define MINUS 296
#define MULT 297
#define DIV 298
#define EX 299
#define UNARYMINUS 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
