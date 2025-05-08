CC=gcc
LEX=win_flex
YACC=win_bison
RM=del

all: program2.exe

program2.exe: lex.yy.c y.tab.c y.tab.h
	$(CC) lex.yy.c y.tab.c -o program2

lex.yy.c: Scanner_lex2.l
	$(LEX) Scanner_lex2.l

y.tab.c: Parser_yacc2.y
	$(YACC) -y Parser_yacc2.y

y.tab.h: Parser_yacc2.y
	$(YACC) -y -d Parser_yacc2.y

clean:
	$(RM) program2.exe lex.yy.c y.tab.c y.tab.h