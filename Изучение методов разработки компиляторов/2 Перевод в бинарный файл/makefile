all: bincomp

bincomp: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c markstack.c defmarkstack.c strstack.c -o bincomp

y.tab.c: bincomp.y
	yacc -d bincomp.y

lex.yy.c: bincomp.l
	lex bincomp.l

clean:
	rm bincomp lex.yy.c y.tab.h y.tab.c


rebuild: clean all
