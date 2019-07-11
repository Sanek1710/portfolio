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
    Num = 258,
    While = 259,
    If = 260,
    Else = 261,
    Print = 262,
    Return = 263,
    Var = 264,
    Inc = 265,
    Dec = 266,
    Mvadd = 267,
    Mvsub = 268,
    Mvmlt = 269,
    Mvdiv = 270,
    Mvmod = 271,
    Mvor = 272,
    Mvand = 273,
    Mvxor = 274,
    Mvlsh = 275,
    Mvrsh = 276,
    Add = 277,
    Sub = 278,
    Mlt = 279,
    Div = 280,
    Mod = 281,
    Lsh = 282,
    Rsh = 283,
    Moreq = 284,
    Leseq = 285,
    Equal = 286,
    Noteq = 287,
    More = 288,
    Less = 289,
    Move = 290,
    Or = 291,
    And = 292,
    Not = 293,
    Bitor = 294,
    Bitand = 295,
    Bitxor = 296,
    Bitnot = 297
  };
#endif
/* Tokens.  */
#define Num 258
#define While 259
#define If 260
#define Else 261
#define Print 262
#define Return 263
#define Var 264
#define Inc 265
#define Dec 266
#define Mvadd 267
#define Mvsub 268
#define Mvmlt 269
#define Mvdiv 270
#define Mvmod 271
#define Mvor 272
#define Mvand 273
#define Mvxor 274
#define Mvlsh 275
#define Mvrsh 276
#define Add 277
#define Sub 278
#define Mlt 279
#define Div 280
#define Mod 281
#define Lsh 282
#define Rsh 283
#define Moreq 284
#define Leseq 285
#define Equal 286
#define Noteq 287
#define More 288
#define Less 289
#define Move 290
#define Or 291
#define And 292
#define Not 293
#define Bitor 294
#define Bitand 295
#define Bitxor 296
#define Bitnot 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 5 "comp.y" /* yacc.c:1909  */
 int num; char id; const char *str; 

#line 141 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
