%{
#include <stdio.h>
#include "y.tab.h"
%}
tip "int"|"float"|"char"|"string"|"double"
nr [1-9][0-9]*
cuvant [_a-zA-Z][_a-zA-Z0-9]*
caracter \'[a-zA-Z]\'
caractere \"[a-zA-Z][a-zA-Z]*\"
include1 \<{cuvant}.h\>
include2 \<{cuvant}\>

%%
struct {return STRUCT;}

"const" {return CONST;}

"print" {return PRINT;}

"printf" {return PRINTF;}

"return" { return RETURN; }

"#include"[ ]*({include1}|{include1}) { return LIBRARIE; }

("void"|"int")[ ]+main\(\) { return MAIN; }

"strcpy" {return STRCPY;}

"strcmp" {return STRCMP;}

"*" { return PRODUS; }

"/" { return IMPARTIRE; }

"+" { return PLUS; }

"-" { return MINUS; }

">" { return MMARE; }

"<" { return MMIC; }

"!=" { return DIFERIT; }

"==" { return EGAL; }

"||" { return SAU; }

"&&" { return SI; }

"if" { return IF; }

"else" {return ELSE;}

"while" { return WHILE; }

"for" { return FOR; }

("int"|"float"|"char"|"string") { strcpy(yylval.strval, yytext); return TIP;}

{cuvant}(\[{nr}\])+ { strcpy(yylval.strval, yytext);  return VECTOR; }

{caracter}|{caractere} { strcpy(yylval.strval, yytext);  return CARACTER; }

0|[1-9][0-9]* { strcpy(yylval.strval, yytext); return NUMAR; }

{cuvant} { strcpy(yylval.strval, yytext); return NUME; }

":=" { return ATRIBUIRE; }

[ \t] ;

\n {yylineno++;}

. {return yytext[0];}
