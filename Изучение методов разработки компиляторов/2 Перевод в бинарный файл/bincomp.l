%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

extern void yyerror(char *s);

int atoui_dec(const char *str);
int atoui_hex(const char *str);

%}

%%
[0-9]+							{ yylval.num = atoui_dec(yytext); return Num; }
0[xX][0-9A-Fa-f]+			    { yylval.num = atoui_hex(yytext); return Num; }
[0-9A-Fa-f]+h			    	{ yylval.num = atoui_hex(yytext); return Num; }

"mov" 							{ return Mov; }
"je" 							{ return Je ; }
"jne" 							{ return Jne; }
"jl" 							{ return Jl ; }
"jle" 							{ return Jle; }
"jg" 							{ return Jg ; }
"jge" 							{ return Jge; }
"jmp" 							{ return Jmp; }
"ret" 							{ return Ret; }
"cmp" 							{ return Cmp; }
"tst" 							{ return Tst; }
"add" 							{ return Add; }
"sub" 							{ return Sub; }
"mlt" 							{ return Mlt; }
"div" 							{ return Div; }
"shl" 							{ return Shl; }
"shr" 							{ return Shr; }
"and" 							{ return And; }
"or" 							{ return Or ; }
"xor" 							{ return Xor; }
"neg" 							{ return Neg; }
"not" 							{ return Not; }
"call"							{ return Call;}	

"eax"							{ yylval.num = 0; return Eax; }
"ebx"							{ yylval.num = 1; return Ebx; }

[.]CODE 						{ return Code; }
[.]DATA 						{ return Data; }
[.]mem  						{ return Mem;  }
[.]dw 							{ return Dw;  }

[.] 							{ return CurPC; }
[a-zA-Z_][0-9a-zA-Z_]* 		    { yylval.str = strdup(yytext); return Mark; }

[-+,\[\]:]						{ return yytext[0]; }
[\n]							{ yylineno++; }
[ \t\r]							{  }
.								{ yyerror("unexpected char"); printf("%c", *yytext); return 0; }
%%

int
atoui_dec(const char *str)
{
	int num = 0;

	while (*str)
	{
		num = num * 10 + (*str - '0');
		str++;
	}

	return num;
}

int
atoui_hex(const char *str)
{
	int num = 0;
	if (str[1] == 'x' || str[1] == 'X') str += 2;

	while (*str && *str != 'h')
	{
		num = num * 16;
		if ('0' <= *str && *str <= '9') num += *str - '0';
		else if ('a' <= *str && *str <= 'f') num += *str - 'a' + 10;
		else if ('A' <= *str && *str <= 'F') num += *str - 'A' + 10;

		str++;
	}

	return num;
}