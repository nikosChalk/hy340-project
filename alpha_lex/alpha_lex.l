/* ~~~ user macros  and forward declarations ~~~ */

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include "alpha_token.h"
	#include "alpha_token_generator.h"
	#define MY_EOF 0
	#define YY_DECL int alpha_yylex(void *ylval)
%}

/* ~~~ FLEX parameters ~~~ */

%option header-file="alpha_lex.h"
%option outfile = "alpha_lex.cpp"
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
	alpha_lex::alpha_token_t *ptr;
	//list<alp_alpha_token_t*> l;

	yyin = stdin;
	while((x = alpha_yylex(ptr)) != MY_EOF) {
		printf("yylex returned: %d\n", x);
		//l.append(ptr);
	}
	printf("EOF reached\n");

	return 0;
}
