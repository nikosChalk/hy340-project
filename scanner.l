/* ~~~ CODE ~~~ */

%{
	#include <stdio.h>
	#include <stdlib.h>
	#define MY_EOF 0
	#define YY_DECL int alpha_yylex(void *ylval)

//d	#define MAKE_TOKEN (new Token(yyline, yytext, type)(type))
%}

/* ~~~ FLEX parameters ~~~ */

%option header-file="scanner.h"
%option outfile = "scanner.cpp"
%option noyywrap
%option yylineno

/* ~~~ regex definitions ~~~ */

num		[0-9]+


/* ~~~ actions section ~~~ */
%%

{num}	{ printf("num found: %s in line %d\n", yytext, yylineno); return atoi(yytext); /* telesths new gia to struct */}
.	{ printf("No match found for %s\n", yytext); return MY_EOF; }
\n	{ }	/* Work-arround because yylineno was stuck at 1. */


%%

/* ~~~ user code ~~~ */

int main () {
	int x;
	alpha_token_t *ptr;
	//list<alp_alpha_token_t*> l;

	yyin = stdin;
	while((x = alpha_yylex(ptr)) != MY_EOF) {
		printf("yylex returned: %d\n", x);
		//l.append(ptr);
	}
	printf("EOF reached\n");

	return 0;
}
