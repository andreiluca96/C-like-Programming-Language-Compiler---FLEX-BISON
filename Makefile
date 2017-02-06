all:
	lex compilator.l
	yacc -d compilator.y
	gcc lex.yy.c y.tab.c -ll -ly
clean: