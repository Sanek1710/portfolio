%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markstack.h"
#include "defmarkstack.h"
#include "strstack.h"
extern void yyerror(char *s);
extern void yyset_in (FILE *  _in_str );
extern int  yyget_lineno();
extern int  yylex(void);
extern int  yyparse(void);

FILE *out_file;
int   PC = 0;
int MEM_STACK = 0;
fpos_t zero_fpos;

void write_code (unsigned short byte_code);
void write_codeh(unsigned char h1, unsigned char h2);
void write_codeq(unsigned char q1, unsigned char q2, unsigned char q3, unsigned char q4);
void write_code_jmp(unsigned char q1, unsigned short addr);

STR_STACK Vars;

MARK_STACK Undef;
DEF_MARK_STACK Defined;

int check_mark(const char *str);
int overwrite_marks();

%}

%union { int num; const char *str; }

%start SRC_ASM

%token Mov Je Jne Jl Jle Jg Jge Jmp Ret Cmp Tst Call
%token Add Sub Mlt Div Shl Shr And Or Xor Neg Not
%token Code Data Mem Dw CurPC
%token <num> Num Eax Ebx 
%token <str> Mark 

%type <num> REG MEM POINT CurPC
%type <str> VAR

%right Data

%%

SRC_ASM : Data DATA Mem ':' 		{ MEM_STACK = Vars.count * 4; }
		  Code CODE  				{ }
		;

DATA 	: VAR						{ str_stack_push(&Vars, $1); }
		| DATA VAR 					{ str_stack_push(&Vars, $2); }
		;

VAR 	: Mark ':' Dw Num 			{ $$ = $1; }
		;

CODE    : CMD 						{  }
        | CODE CMD 					
        ;

CMD 	: MOV
		| JMP
		| CMP
		| BINOP
		| UNOOP
		| Mark ':' 					{ def_mark_stack_push(&Defined, $1, PC); }
		;

MOV		: Mov REG ',' REG 			{ write_codeq(0x1, 0x0,  $2,  $4); }
		| Mov REG ',' MEM 			{ write_codeq(0x1, 0x1,  $2, 0x0);
									  write_code($4);
									  }
		| Mov MEM ',' REG 			{ write_codeq(0x1, 0x2, 0x0,  $4); 
									  write_code($2);
									  }
		| Mov REG ',' Num 			{ write_codeq(0x1, 0x3,  $2, 0x0);
									  write_code($4 >> 16);
									  write_code($4 & 0xFFFF);
									  }
		;

JMP 	: Je   POINT 				{ write_code_jmp(0x8, $2); }
		| Jne  POINT 				{ write_code_jmp(0x9, $2); }
		| Jl   POINT 				{ write_code_jmp(0xA, $2); }
		| Jle  POINT 				{ write_code_jmp(0xB, $2); }
		| Jg   POINT 				{ write_code_jmp(0xC, $2); }
		| Jge  POINT 				{ write_code_jmp(0xD, $2); }
		| Jmp  POINT 				{ write_code_jmp(0xE, $2); }
		| Call Mark 				{ write_codeq(0xF, 0x0, 0x0, 0x0); }
		| Ret       				{ write_code(0x0000); }
		;

POINT   : Mark 						{ $$ = check_mark($1); }
		| CurPC '+' Num  			{ $$ = PC + $3; }
		| CurPC '-' Num 			{ $$ = PC - $3; }
		| CurPC 					{ $$ = PC; }
		;

CMP 	: Cmp REG ',' REG 			{ write_codeq(0x4, 0xF,  $2, $4); }
		| Tst REG 					{ write_codeq(0x5, 0xF, 0x0, $2); }
		;

BINOP   : Add REG ',' REG 			{ write_codeq(0x4, 0x0, $2, $4);  }
		| Sub REG ',' REG 			{ write_codeq(0x4, 0x1, $2, $4);  }
		| Mlt REG ',' REG			{ write_codeq(0x4, 0x2, $2, $4);  }
		| Div REG ',' REG			{ write_codeq(0x4, 0x3, $2, $4);  }
		| Shl REG ',' REG			{ write_codeq(0x4, 0x4, $2, $4);  }
		| Shr REG ',' REG			{ write_codeq(0x4, 0x5, $2, $4);  }
		| And REG ',' REG			{ write_codeq(0x4, 0x8, $2, $4);  }
		| Or  REG ',' REG			{ write_codeq(0x4, 0x9, $2, $4);  }
		| Xor REG ',' REG			{ write_codeq(0x4, 0xB, $2, $4);  }
		;

UNOOP   : Neg REG 			 		{ write_codeq(0x5, 0xC, 0x0, $2); }
		| Not REG 					{ write_codeq(0x5, 0xD, 0x0, $2); }
		;

MEM 	: '[' Mem '+' Num ']' 		{ $$ = MEM_STACK + $4; }
		| '[' Mem '-' Num ']' 		{ $$ = MEM_STACK - $4; }
		| '[' Mark ']' 				{ $$ = str_stack_fnd(&Vars, $2) * 4; }
		;

REG     : Eax						{ $$ = $1; }
		| Ebx						{ $$ = $1; }
		;

%%


int
main(int argc, const char *argv[])
{
	char out_file_name[257];

	if (argc > 1)
	{
		FILE *f = fopen(argv[1], "r");
		if (f == NULL)
		{
		  printf("No such file\n");
		  return -1;
		}

		int len = strlen(argv[1]);
		strcpy(out_file_name, argv[1]);

		if (len < 4) return -1;

		if (strcmp(out_file_name + len - 4, ".asm"))
		{
		  printf("incorrect file extension\n");
		  return -1;
		}
		else
		{
		  out_file_name[len - 4] = '\0';
		  strcat(out_file_name + len - 4, ".bin");
		  printf("%s\n", out_file_name);
		}

		yyset_in (f);
	}
	else strcpy(out_file_name, "o.bin");

	out_file = fopen(out_file_name, "wb+");
	Vars = str_stack_new();
	Undef = mark_stack_new();
	Defined  = def_mark_stack_new();

	int Res = yyparse();
	overwrite_marks();

	fclose(out_file);

	if (Res == 0) 
	{
	    printf("OK \n");

	}
	else 
	{
	    printf("HE OK\n");
	    remove(out_file_name);
	}

	str_stack_delete(&Vars);
	mark_stack_delete(&Undef);
	def_mark_stack_delete(&Defined );

	return(Res);
}

void 
write_code(unsigned short byte_code)
{
	PC += 2;
	fwrite((void*)&byte_code, 1, 2, out_file);
}

void 
write_codeh(unsigned char h1, unsigned char h2)
{
	unsigned short byte_code = 0;

	PC += 2;
	byte_code = (h1 << 8) | (h2 << 0);
	fwrite((void*)&byte_code, 1, 2, out_file);
}

void 
write_codeq(unsigned char q1, unsigned char q2, unsigned char q3, unsigned char q4)
{
	unsigned short byte_code = 0;

	PC += 2;
	byte_code = (q1 << 12) | (q2 << 8) | (q3 << 4) | (q4 << 0);
	fwrite((void*)&byte_code, 1, 2, out_file);
}

void 
write_code_jmp(unsigned char q1, unsigned short addr)
{
	unsigned short byte_code = 0;

	PC += 2;
	byte_code = (q1 << 12) | (addr << 0);
	fwrite((void*)&byte_code, 1, 2, out_file);
}

void
yyerror(char *s)
{
  fprintf(stderr, "%s\n" , s);
}

int
yywrap()
{
  return(1);
}


int check_mark(const char *str)
{
	int idx;
	fpos_t fpos;

	idx = def_mark_stack_fnd(&Defined, str);

	if (idx < 0) 
	{
		fgetpos(out_file, &fpos);
		mark_stack_push(&Undef, str, PC, fpos);
		return 0x000;
	}

	return Defined.list[idx].point;
}

int overwrite_marks()
{
	unsigned short jmp_code;
	int idx;

	for (int i = 0; i < Undef.count; ++i)
	{
		fsetpos(out_file, &Undef.list[i].fpos);
		fread(&jmp_code, 2, 1, out_file);
		idx = def_mark_stack_fnd(&Defined, Undef.list[i].name);

		jmp_code = (jmp_code & 0xF000) | (Defined.list[idx].point);

		fsetpos(out_file, &Undef.list[i].fpos);
		fwrite(&jmp_code, 2, 1, out_file);
	}
}