/* ~~~ CODE ~~~ */

%{
	#define YY_DECL int alpha_yylex(void *ylval)
%}

/* ~~~ FLEX parameters ~~~ */

%option header-file="scanner.h"
%option outfile = "scanner.cpp"
%option noyywrap
%option yylineno

/* ~~~ regex definitions ~~~ */


/* ~~~ actions section ~~~ */
%%

%%

/* ~~~ user code ~~~ */

int main () {


}
