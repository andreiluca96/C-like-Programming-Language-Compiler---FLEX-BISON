#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXVARIABLELENGTH 100
#define MAXTYPELENGTH 16
#define MAXNUMBERPARAMETERS 246
#define MAXSTRINGLENGTH 2048
#define MAXNUMBERVARIABLE 1024
#define MAXNUMBERFUNCTIONS 1024
#define ERRNOFUNCTION -1
#define ERRINVALIDNUMBEROFPARAMETERS -2
#define ERRVARIABLENOTFOUND -4
#define ERRWRONGTYPE 0

union tipVariabila {
int intval;
float floatval;
char* strval;
char charval;
};

struct variabila {
	char nume[MAXVARIABLELENGTH];
	char tip[MAXTYPELENGTH];
};

struct celulaTabelaVariabila {
	struct variabila variabilaCurenta;
	int initializata;
	void *adresa;
};

struct celulaTabelaFunctii {
	char nume[MAXVARIABLELENGTH];
	char tipReturnat[MAXTYPELENGTH];
	int numarParametrii;
	char tipParametrii[MAXNUMBERPARAMETERS][MAXTYPELENGTH];
	int rezervata;
};

int pozitiaVariabileiInTabela(char *numeleVariabilei, struct celulaTabelaVariabila *tabela, int numarVariabile) {
	int pozitie = -1;
	struct variabila v;
	for (int i = 0; i < numarVariabile; i++) {
		v = tabela[i].variabilaCurenta;
		if (strcmp(numeleVariabilei, v.nume) == 0) {
			pozitie = i;
		}
	}

	return pozitie;
};

void adaugaFunctieInTabela(char *numeFunctie, char *tipReturnat, char *parametrii, int pozitie, 
	struct celulaTabelaFunctii *tabela, int rezervata) {
	struct celulaTabelaFunctii c;
	strcpy(c.nume, numeFunctie);
	strcpy(c.tipReturnat, tipReturnat);
	c.numarParametrii = 0;
	char *parametriiCopy = (char *)malloc(sizeof(char) * MAXNUMBERPARAMETERS);
	strcpy(parametriiCopy, parametrii);
	char *p = strtok(parametriiCopy, " ");
	while (p) {
		strcpy(c.tipParametrii[c.numarParametrii++], p);
		p = strtok(NULL, " ");
	}
	c.rezervata = rezervata;
	tabela[pozitie] = c;
}

int verificaFunctie(char *numeFunctie, char *parametrii, struct celulaTabelaFunctii *tabelaFunctii, 
	struct celulaTabelaVariabila *tabelaVariabile, int numarFunctii, int numarVariabile) {
	int pozitie = -1;
	for (int i = 0; i < numarFunctii; i++) {
		if (strcmp(tabelaFunctii[i].nume, numeFunctie) == 0) {
			pozitie = i;
			break;
		}
	}
	if (pozitie == -1) {
		printf("Nu am gasit functia %s\n", numeFunctie);
		return ERRNOFUNCTION;
	}
	int nrP = 0;
	int ok = 1;
	struct variabila v;
	int pozitieVariabila = -1;
	char *parametriiCopy = (char *)malloc(sizeof(char) * MAXNUMBERPARAMETERS);
	strcpy(parametriiCopy, parametrii);
	char *p = strtok(parametriiCopy, " ");

	while (p) {

		if (nrP > tabelaFunctii[pozitie].numarParametrii) {
			return ERRINVALIDNUMBEROFPARAMETERS;
		}
		pozitieVariabila = pozitiaVariabileiInTabela(p, tabelaVariabile, numarVariabile);
		if (pozitieVariabila != -1) {
			v = tabelaVariabile[pozitieVariabila].variabilaCurenta;
			if (strcmp(v.tip, tabelaFunctii[pozitie].tipParametrii[nrP]) != 0) {
				ok = 0;
			}
		} else {
			ok = ERRVARIABLENOTFOUND;
		}
		p = strtok(NULL, " ");
		nrP++;
	}

	if (nrP < tabelaFunctii[pozitie].numarParametrii) {
		return ERRINVALIDNUMBEROFPARAMETERS - 1;
	}

	return ok;
}

int verificaVariabilaInitializata(char *numeVariabila, struct celulaTabelaVariabila *tabela, int numarVariabile) {
	int pozitie = pozitiaVariabileiInTabela(numeVariabila, tabela, numarVariabile);
	if (pozitie == -1) {
		return -1;
	}

	return tabela[pozitie].initializata;
}

int adaugaVariabilaInTabela(char *tipulVariabilei, char *numeleVariabilei, int pozitie, struct celulaTabelaVariabila *tabela) {
	struct variabila v;
	if (pozitiaVariabileiInTabela(numeleVariabilei, tabela, pozitie) != -1) {
		return 0;
	}
	struct celulaTabelaVariabila c;
	strcpy(v.nume, numeleVariabilei);
	strcpy(v.tip, tipulVariabilei);
	void *adresaVariabila;
	void *adresa;
	if (strcmp(tipulVariabilei, "int") == 0) {
		adresa = malloc(sizeof(int));
	}
	if (strcmp(tipulVariabilei, "char") == 0) {
		adresa = malloc(sizeof(char));
	}
	if (strcmp(tipulVariabilei, "float") == 0) {
		adresa = malloc(sizeof(float));
	}
	if (strcmp(tipulVariabilei, "string") == 0) {
		adresa = malloc(MAXSTRINGLENGTH * sizeof(char));
	}
	c.variabilaCurenta = v;
	c.adresa = adresa;
	c.initializata = 0;

	tabela[pozitie] = c;
	fflush(stdout);

	return 1;
};

void print(char *numeVariabila, struct celulaTabelaVariabila *tabela, int numarVariabile) {
	int pozitie = pozitiaVariabileiInTabela(numeVariabila, tabela, numarVariabile);

	if (pozitie == -1) {
		return;
	}
	if (tabela[pozitie].initializata == 0) {
		printf("Variabila nu este Initializata!\n");
		return;
	}
	struct variabila v = tabela[pozitie].variabilaCurenta;

	if (strcmp(v.tip, "int") == 0) {
		int valoareInt = *((int *)tabela[pozitie].adresa);
		printf("%s = %d\n", numeVariabila, valoareInt);

		return ;
	}
	if (strcmp(v.tip, "char") == 0) {
		char valoareChar = *((char *)tabela[pozitie].adresa);
		printf("%s = \'%c\'\n",numeVariabila , valoareChar);

		return ;
	}
	if (strcmp(v.tip, "float") == 0) {
		float valoareFloat = *((float *)tabela[pozitie].adresa);
		printf("%s = %f\n", numeVariabila, valoareFloat);

		return ;	
	}
	if (strcmp(v.tip, "string") == 0) {
		printf("%s = \"%s\"\n", numeVariabila, (char *)tabela[pozitie].adresa);

		return ;
	}

};

int seteazaValoare(char *numeleVariabilei, char *valoare, struct celulaTabelaVariabila *tabela, int numarVariabile) {

	int pozitie = pozitiaVariabileiInTabela(numeleVariabilei, tabela, numarVariabile);

	if (pozitie == -1) {
		return 0;
	}
	struct variabila v = tabela[pozitie].variabilaCurenta;
	if ((valoare[0] >= 'a' && valoare[0] <='z') || (valoare[0] >= 'A' && valoare[0] <= 'Z')) {
		int pozitieVariabila2 = -1;
		pozitieVariabila2 = pozitiaVariabileiInTabela(valoare, tabela, numarVariabile);
		struct variabila v2 = tabela[pozitieVariabila2].variabilaCurenta;
		if (strcmp(v.tip, v2.tip) == 0) {
			int dim;
			if (strcmp(v.tip, "int") == 0) {
				dim = sizeof(int);
			}
			if (strcmp(v.tip, "char") == 0) {
				dim = sizeof(char);
			}
			if (strcmp(v.tip, "float") == 0) {
				dim = sizeof(float);
			}
			if (strcmp(v.tip, "string") == 0) {
				strcpy(tabela[pozitie].adresa, tabela[pozitieVariabila2].adresa);
			} else {
				int i;
				memcpy(&i, tabela[pozitieVariabila2].adresa, dim);
				memcpy(tabela[pozitie].adresa, tabela[pozitieVariabila2].adresa, dim);
				tabela[pozitie].initializata = 1;
			}

			return 1;
		} else {

			return 0;
		}
	}
	if (strcmp(v.tip, "int") == 0 && valoare[0] >= '0' && valoare[0] <= '9') {
		int valoareInt = atoi(valoare);
		memcpy(tabela[pozitie].adresa, &valoareInt, sizeof(int));

		tabela[pozitie].initializata = 1;
		return 1;
	}
	if (strcmp(v.tip, "char") == 0 && valoare[0] == '\'') {
		char valoareChar = valoare[1];
		memcpy(tabela[pozitie].adresa, &valoareChar, sizeof(char));

		tabela[pozitie].initializata = 1;
		return 1;
	}
	if (strcmp(v.tip, "float") == 0  && valoare[0] >= '0' && valoare[0] <= '9') {
		float valoareFloat = atof(valoare);
		memcpy(tabela[pozitie].adresa, &valoareFloat, sizeof(float));

		tabela[pozitie].initializata = 1;
		return 1;	
	}
	if (strcmp(v.tip, "string") == 0 && valoare[0] == '\"') {
		char *p = (char *)malloc(MAXSTRINGLENGTH * sizeof(char));
		for (int i = 0; i < strlen(valoare) - 2; i++) {
			p[i] = valoare[i+1];
		}
		strcpy(tabela[pozitie].adresa, p);

		tabela[pozitie].initializata = 1;
		return 1;
	}

	return 0;
};

int maximum(int a, int b) {
	if (a < b) {
		return b;
	}

	return a;
};

int minimum(int a, int b) {
	if (a < b) {
		return a;
	}

	return b;
};


int executaFunctieRezervata(int pozitie, char *parametrii, struct celulaTabelaVariabila *tabelaVariabile, int numarVariabile,
	struct celulaTabelaFunctii *tabelaFunctii, int numarFunctii) {
	printf("Executa functie rezervata!\n");
	if (strcmp(tabelaFunctii[pozitie].nume, "maximum") == 0) {
		int a, b;
		char *p = strtok(parametrii, " ");
		int pozitie = pozitiaVariabileiInTabela(p, tabelaVariabile, numarVariabile);
		a = *((int *)tabelaVariabile[pozitie].adresa);
		p = strtok(NULL, " ");
		pozitie = pozitiaVariabileiInTabela(p, tabelaVariabile, numarVariabile);
		b = *((int *)tabelaVariabile[pozitie].adresa);

		return maximum(a, b);
	}
	if (strcmp(tabelaFunctii[pozitie].nume, "minimum") == 0) {
		int a, b;
		char *p = strtok(parametrii, " ");
		int pozitie = pozitiaVariabileiInTabela(p, tabelaVariabile, numarVariabile);
		a = *((int *)tabelaVariabile[pozitie].adresa);
		p = strtok(NULL, " ");
		pozitie = pozitiaVariabileiInTabela(p, tabelaVariabile, numarVariabile);
		b = *((int *)tabelaVariabile[pozitie].adresa);

		return minimum(a, b);
	}
};

