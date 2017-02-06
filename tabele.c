%{
#include <stdio.h>
#define MAXNUMBEROFVARIABLES 1024
#define MAXNUMBEROFFUNCTIONS 1024
//struct celulaTabelaVariabila variabile[MAXNUMBEROFVARIABLES];
//struct celulaTabelaFunctii functii[MAXNUMBEROFFUNCTIONS];
int nrVariabile = 0;
int nrFunctii = 0;

extern FILE* yyin;
extern char* yytext;
extern int yylineno;


%}
%union {
int intval;
char* strval;
char charval;
}

%token LIBRARIE MAIN TIP NUME VECTOR CARACTER NUMAR STRUCT ATRIBUIRE MMARE MMIC DIFERIT EGAL PRODUS IMPARTIRE PLUS MINUS SI SAU IF ELSE FOR WHILE
%right IF ELSE
%left PRODUS
%left IMPARTIRE
%left PLUS
%left MINUS
%left ATRIBUIRE
%left SI
%left SAU
%left MMARE MMIC DIFERIT EGAL
%left NUME CARACTER NUMAR
%start progr
%%
progr: listaLibrarii declaratii functieMain {printf("program corect sintactic\n");}
     ;

listaLibrarii :  LIBRARIE
	   | listaLibrarii LIBRARIE
	   ;

declaratii : declaratie 
           | declaratii declaratie
	   ;

declaratie : TIP NUME ';'
	   | TIP VECTOR ';' 
           | TIP NUME '('')' ';'
           | TIP NUME '(' listaParametrii ')' ';'
	   | STRUCT NUME'{' declaratii '}'';'
           ;

listaParametrii : parametru
            | listaParametrii ','  parametru 
            ;
            
parametru : TIP NUME
	  | TIP VECTOR
          ; 

	/* bloc */
functieMain : MAIN bloc 
     ;

bloc : '{' listaInstructiuni '}'
     ;
     
	/* lista instructiuni */
listaInstructiuni :  instructiune ';' 
                  | listaInstructiuni instructiune ';' 
		  | instructiuneControl
		  | listaInstructiuni instructiuneControl
                  ;

	/* instructiuneControl*/
instructiuneControl : IF '(' conditie ')' bloc
		    | IF '(' conditie ')' bloc ELSE bloc
		    | WHILE '(' conditie ')' bloc
 		    | FOR '(' instructiune ';' conditie ';' listaInstructiuni ')' bloc
		    ;

	/* conditie*/
conditie : conditie MMARE conditie
	 | conditie MMIC conditie
	 | conditie DIFERIT conditie
	 | conditie EGAL conditie
	 | conditie SI conditie
	 | conditie SAU conditie
	 | NUME
	 | CARACTER
	 | NUMAR
	 ;

	/* instructiune */
instructiune: NUME ATRIBUIRE expresii	 
            | NUME '(' listaApelFunctie ')'
	    | declaratie
            ;
        
listaApelFunctie : CARACTER
		 | NUMAR
		 | listaApelFunctie ',' NUMAR
           	 | listaApelFunctie ',' CARACTER
		 | listaApelFunctie ',' NUME
		 | NUME	 
           ;

	/* expresii*/
expresii : expresii PRODUS expresii
	 | expresii IMPARTIRE expresii
	 | expresii PLUS expresii
	 | expresii MINUS expresii
	 | NUME
	 | CARACTER
	 | NUMAR
	 ;

%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
}