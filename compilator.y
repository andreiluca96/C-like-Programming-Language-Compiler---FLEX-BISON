%{
#include <stdio.h>
#include <stdlib.h>
#include "tabele.h"
#define MAXNUMBEROFVARIABLES 1024
#define MAXNUMBEROFFUNCTIONS 1024
#define ERR 12312312
struct celulaTabelaVariabila tabelaVariabile[MAXNUMBEROFVARIABLES];
struct celulaTabelaFunctii tabelaFunctii[MAXNUMBEROFFUNCTIONS];
int nrVariabile = 0;
int nrFunctii = 0;

extern FILE* yyin;
extern char* yytext;
extern int yylineno;


%}
%union {
char strval[2048];
int intval;
}

%token <strval>STRCPY CONST <strval>STRCMP PRINT PRINTF LIBRARIE MAIN <strval>TIP <strval>NUME VECTOR <strval>CARACTER <strval>NUMAR STRUCT ATRIBUIRE MMARE MMIC DIFERIT EGAL PRODUS IMPARTIRE PLUS MINUS SI SAU IF ELSE FOR WHILE RETURN
%type <strval>valoare
%type <strval>listaParametrii
%type <strval>parametru
%type <strval>listaApelFunctie
%type <intval>expresie
%right IF ELSE
%left '(' ')'
%left PLUS
%left MINUS
%left PRODUS
%left IMPARTIRE
%left ATRIBUIRE
%left SI
%left SAU
%left MMARE MMIC DIFERIT EGAL
%left NUME CARACTER NUMAR
%start progr
%%
progr: listaLibrarii declaratii functieMain definitiiFunctii {printf("\n\nProgram corect sintactic!!!\n");}
     ;

listaLibrarii :  LIBRARIE
           | listaLibrarii LIBRARIE
           ;

declaratii : declaratie ';'
           | declaratii declaratie ';'
           ;

declaratie : TIP NUME  { int rez = adaugaVariabilaInTabela($1, $2, nrVariabile, tabelaVariabile); if (rez == 0) {printf("Variabila \"%s\" a fost declarata deja!\n", $2); } else {nrVariabile++;} } 
           | TIP VECTOR  
           | TIP NUME '('')'  
           | TIP NUME '(' listaParametrii ')'  { adaugaFunctieInTabela($2, $1, $4, nrFunctii++,tabelaFunctii, 0); }
           | STRUCT NUME'{' declaratii '}'
           | STRUCT NUME NUME 
           | CONST TIP NUME  
           | CONST TIP VECTOR  
           | CONST TIP NUME '('')'  
           | CONST TIP NUME '(' listaParametrii ')' 
           | CONST STRUCT NUME'{' declaratii '}'
           | CONST STRUCT NUME NUME 
           ;

listaParametrii : parametru { for (int i = 0; i < 2048; i++) { $$[i] = $1[i];} }
                | listaParametrii ','  parametru { char s[2048]; strcpy(s, $1); strcat(s, " "); strcat(s, $3); for (int i = 0; i < 2048; i++) { $$[i] = s[i];} }
            ;
            
parametru : TIP 
          ; 

        /* bloc */
functieMain : MAIN bloc 
     ;

bloc : '{' listaInstructiuni RETURN expresie ';' '}'
     | '{' listaInstructiuni RETURN valoare ';' '}'
     ;
        /* lista instructiuni */
listaInstructiuni : instructiune ';' 
                  | listaInstructiuni instructiune ';' 
                  | instructiuneControl
                  | listaInstructiuni instructiuneControl
                  ;

        /* instructiuneControl*/
instructiuneControl : IF '(' conditie ')' '{' listaInstructiuni '}'
        | IF '(' conditie ')' '{' listaInstructiuni '}' ELSE '{' listaInstructiuni '}'
        | WHILE '(' conditie ')' '{' listaInstructiuni '}'
        | FOR '(' instructiune ';' conditie ';' listaInstructiuni ')' '{' listaInstructiuni '}'
        ;

        /* conditie*/
conditie : conditie MMARE conditie
         | conditie MMIC conditie
         | conditie DIFERIT conditie
         | conditie EGAL conditie
         | conditie SI conditie
         | conditie SAU conditie
         | '('  conditie ')'
         | NUME { 
            int pozitie = pozitiaVariabileiInTabela($1, tabelaVariabile, nrVariabile);
            if (pozitie == -1) {
              printf("Variabila %s nu a fost declarata! Linia: %d\n", $1, yylineno);
            }
         }
         | CARACTER
         | NUMAR
         ;

        /* instructiune */
instructiune: NUME ATRIBUIRE valoare { if (seteazaValoare($1, $3, tabelaVariabile, nrVariabile) == 1) printf("Setarea valorii de la linia %d a avut succes!\n", yylineno); else printf("Eroare semantica la linia %d\n", yylineno); }
            | NUME ATRIBUIRE expresie { 
                char buff[2048];
                int rezultat = $3;
                sprintf(buff, "%d", rezultat);
                if (seteazaValoare($1, buff, tabelaVariabile, nrVariabile) && $3 != ERR) 
                    printf("Setarea valorii de la linia %d a avut succes!\n", yylineno); 
                else printf("Eroare semantica la linia %d\n", yylineno);
            }
            | STRCPY '(' NUME ',' NUME ')'
            | STRCPY '(' NUME ',' CARACTER ')'
            | STRCPY '(' CARACTER ',' CARACTER ')'
            | STRCPY '(' CARACTER ',' NUME ')'
            | STRCMP '(' NUME ',' NUME ')'
            | STRCMP '(' NUME ',' CARACTER ')'
            | STRCMP '(' CARACTER ',' CARACTER ')'
            | STRCMP '(' CARACTER ',' NUME ')'
            | NUME '(' listaApelFunctie ')' {int rez; 
                if ((rez = verificaFunctie($1, $3, tabelaFunctii, tabelaVariabile, nrFunctii, nrVariabile)) == 1) {  printf("Functia de la linia %d este valida.\n", yylineno); } 
                else { printf("Functia de la linia %d nu este valida. Cod eroare: %d\n", yylineno, rez); } 
                if (rez == 1) { 
                        int pozitie = -1;
                        for (int i = 0; i < nrFunctii; i++) {
                                if (strcmp(tabelaFunctii[i].nume, $1) == 0) {
                                        pozitie = i;
                                        break;
                                }
                        }
                        if (tabelaFunctii[pozitie].rezervata == 1) {
                                printf("%d\n", executaFunctieRezervata(pozitie,$3 ,tabelaVariabile, nrVariabile, tabelaFunctii, nrFunctii));
                        }
                } }
            | NUME '.' NUME ATRIBUIRE valoare
            | NUME '.' NUME ATRIBUIRE NUME '.' NUME
            | NUME '.' NUME ATRIBUIRE NUME
            | NUME ATRIBUIRE NUME '.' NUME
            | declaratie
            | PRINT '(' expresie ')' { printf("PRINT linia %d: %d\n", yylineno,$3); }
            | PRINTF '(' NUME ')' { print($3, tabelaVariabile, nrVariabile); printf("\n");}
            ;

expresie: expresie PLUS expresie { $$ = $1 + $3; }
        | expresie PRODUS  expresie { $$ = $1 * $3; }
        | expresie MINUS expresie { $$ = $1 - $3; }
        | expresie IMPARTIRE expresie { $$ = $1 / $3; }
        | '(' expresie ')' { $$ = $2; }
        | NUMAR { $$ = atoi($1); }
        | NUME { int poz = pozitiaVariabileiInTabela($1, tabelaVariabile, nrVariabile); 
                if (poz == -1) { printf("Variabila %s nu exista! Linia: %d\n", $1, yylineno); $$ = ERR;}
                else {
                  if (tabelaVariabile[poz].initializata == 0) { printf("Variabila %s nu a fost intializata!\n", $1); $$ = ERR; }
                  if (strcmp(tabelaVariabile[poz].variabilaCurenta.tip, "int") != 0) {printf("Eroare! Expresie invalida semantic! Linia: %d\n",yylineno); $$ = ERR;}
                  else { $$ = *((int *)(tabelaVariabile[poz].adresa)); } }
                }
        | NUME '(' listaApelFunctie ')' {int rez; 
                if ((rez = verificaFunctie($1, $3, tabelaFunctii, tabelaVariabile, nrFunctii, nrVariabile)) == 1) {  printf("Functia de la linia %d este valida.\n", yylineno); } 
                else { printf("Functia de la linia %d nu este valida. Cod eroare: %d\n", yylineno, rez); } 
                if (rez == 1) { 
                        int pozitie = -1;
                        for (int i = 0; i < nrFunctii; i++) {
                                if (strcmp(tabelaFunctii[i].nume, $1) == 0) {
                                        pozitie = i;
                                        break;
                                }
                        }
                        if (tabelaFunctii[pozitie].rezervata == 1) {
                                $$ = executaFunctieRezervata(pozitie,$3 ,tabelaVariabile, nrVariabile, tabelaFunctii, nrFunctii);
                        }
                } }  
        ;

/*definitiiFunctii*/
definitiiFunctii : TIP NUME '(' listaParametrii ')' bloc
           ;
        
listaApelFunctie : NUME { for (int i = 0; i < 2048; i++) { $$[i] = $1[i];} }  
                 | listaApelFunctie ',' NUME { char s[2048]; strcpy(s, $1); strcat(s, " "); strcat(s , $3);  for (int i = 0; i < 2048; i++) { $$[i] = s[i];}}
                 | NUMAR
     | listaApelFunctie ',' NUMAR
                 | CARACTER
     | listaApelFunctie ',' CARACTER
                 | NUME '(' listaApelFunctie ')'
     | listaApelFunctie ',' NUME '(' listaApelFunctie ')'
;
        /* valori*/
valoare :  CARACTER { for (int i = 0; i < 2048; i++) { $$[i] = $1[i];} }
         ;

%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){

        adaugaFunctieInTabela("maximum", "int", "int int", nrFunctii++, tabelaFunctii, 1);
        adaugaFunctieInTabela("minimum", "int", "int int", nrFunctii++, tabelaFunctii, 1);
        yyin=fopen(argv[1],"r");
        yyparse();

        freopen("stareFinala.txt", "w", stdout);
        printf("Functiile declarate in program: \n\n");
        for (int i = 0; i < nrFunctii; i++) {
                printf("Functia %d\n", i);
                printf("Nume: %s\n", tabelaFunctii[i].nume);
                printf("Tipul returnat: %s\n", tabelaFunctii[i].tipReturnat);
                printf("Functia rezervata: %d\n", tabelaFunctii[i].rezervata);
                for (int j = 0; j < tabelaFunctii[i].numarParametrii; j++) {
                        printf("Tip parametru %d: %s\n", j, tabelaFunctii[i].tipParametrii[j]);
                }
                printf("\n");
        }

        printf("\n");

        printf("Situatia variabilelor declarate dupa executia programului:\n");
        for (int i = 0; i < nrVariabile; i++) {
                struct variabila v;
                v = tabelaVariabile[i].variabilaCurenta;
                printf("Nume: %s\n", v.nume);
                printf("Tip: %s\n", v.tip);
                print(v.nume, tabelaVariabile, nrVariabile);
                printf("\n");
        }
}