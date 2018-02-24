/* ~~~ user macros  and forward declarations ~~~ */

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string>
	#include "alpha_token.h"
	#include "alpha_token_generator.h"
	#define YY_DECL int alpha_yylex(void *ylval)

	enum identified_token{UNKNOWN=0, IF, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, AND,
			NOT, OR, LOCAL, BOOL_TRUE, BOOL_FALSE, NIL,
			OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD, OP_EQ, OP_NE, OP_PLUS_PLUS, OP_MINUS_MINUS,
			OP_GT, OP_LT, OP_GE, OP_LE,
			CONST_INT, CONST_REAL, CONST_STR, PUNCTUATION, IDENTIFIER, COMMENT
	};
%}

/* ~~~ FLEX parameters ~~~ */

%option header-file="alpha_lex.h"
%option outfile = "alpha_lex.cpp"
%option noyywrap
%option yylineno

/* ~~~ regex definitions ~~~ */

delim			[\t\n ]
ws				{delim}+
letter			[a-zA-Z]
digit			[0-9]
underscore		[_]
id				{letter}({letter}|{digit}|_)*
const_int  		{digit}+

/* ~~~ actions section ~~~ */
%%

.	        { return UNKNOWN; }
\n	        { }	/* Work-arround because yylineno was stuck at 1. */

{const_int}				{ return CONST_INT; }
{digit}+"."{digit}*     { return CONST_REAL; }

if						{ return IF; }
else					{ return ELSE; }
while					{ return WHILE; }
for						{ return FOR; }
function				{ return FUNCTION; }
return					{ return RETURN; }
break					{ return BREAK; }
continue				{ return CONTINUE; }
and						{ return AND; }
not						{ return NOT; }
or						{ return OR; }
local					{ return LOCAL; }
true					{ return BOOL_TRUE; }
false					{ return BOOL_FALSE; }
nil						{ return NIL; }

{id}                    { return IDENTIFIER; } /* id MUST be bellow all the above enumerated rules. */

%%

/* ~~~ user code ~~~ */



int main () {
	int x;
	alpha_lex::alpha_token_t *ptr;
	//list<alp_alpha_token_t*> l;

	yyin = stdin;
	while((x = alpha_yylex(ptr)) != EOF && x != UNKNOWN) {
		printf("yylex returned: %d\n", x);
		//l.append(ptr);
	}
	printf("EOF reached\n");

	return 0;
}
