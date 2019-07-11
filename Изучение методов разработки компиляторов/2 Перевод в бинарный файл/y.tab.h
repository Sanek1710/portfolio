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
    Mov = 258,
    Je = 259,
    Jne = 260,
    Jl = 261,
    Jle = 262,
    Jg = 263,
    Jge = 264,
    Jmp = 265,
    Ret = 266,
    Cmp = 267,
    Tst = 268,
    Call = 269,
    Add = 270,
    Sub = 271,
    Mlt = 272,
    Div = 273,
    Shl = 274,
    Shr = 275,
    And = 276,
    Or = 277,
    Xor = 278,
    Neg = 279,
    Not = 280,
    Code = 281,
    Data = 282,
    Mem = 283,
    Dw = 284,
    CurPC = 285,
    Num = 286,
    Eax = 287,
    Ebx = 288,
    Mark = 289
  };
#endif
/* Tokens.  */
#define Mov 258
#define Je 259
#define Jne 260
#define Jl 261
#define Jle 262
#define Jg 263
#define Jge 264
#define Jmp 265
#define Ret 266
#define Cmp 267
#define Tst 268
#define Call 269
#define Add 270
#define Sub 271
#define Mlt 272
#define Div 273
#define Shl 274
#define Shr 275
#define And 276
#define Or 277
#define Xor 278
#define Neg 279
#define Not 280
#define Code 281
#define Data 282
#define Mem 283
#define Dw 284
#define CurPC 285
#define Num 286
#define Eax 287
#define Ebx 288
#define Mark 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 34 "bincomp.y" /* yacc.c:1909  */
 int num; const char *str; 

#line 125 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
