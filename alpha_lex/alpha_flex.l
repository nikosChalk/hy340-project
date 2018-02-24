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
	alpha_lex::alpha_token_generator gen;
%}

/* ~~~ FLEX parameters ~~~ */

%option header-file="alpha_flex.h"
%option outfile = "alpha_flex.cpp"
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

{const_int}				{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_const_int_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return CONST_INT; 
						}
{digit}+"."{digit}*     { 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_const_real_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return CONST_REAL; 
						}

if						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno,yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return IF; 
                        }
else					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return ELSE; 
						}
while					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return WHILE; 
						}
for						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return FOR; 
}
function				{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return FUNCTION; 
						}
return					{ 
						    alpha_lex::alpha_token_t *ptr;
						    ptr = gen.gen_keyword_token(yylineno, yytext);
						    memcpy(ylval, ptr, sizeof(alpha_token_t));
						    free(ptr);
						    return RETURN; 
						}
break					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return BREAK; 
						}
continue				{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return CONTINUE; 
                        }
and						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return AND; 
                        }
not						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return NOT; 
						}
or						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OR; 
						}
local					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return LOCAL; 
						}
true					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return BOOL_TRUE; 
						}
false					{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return BOOL_FALSE; 
						}
nil						{ 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_keyword_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return NIL; 
						}

{id}                    { 
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_identifier_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return IDENTIFIER; /* id MUST be bellow all the above enumerated rules. */
						} 

"="						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_ASSIGN; 
						}
"+"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_PLUS; 
						}
"-"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_MINUS; 
						}
"*"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_MUL; 
						}
"/"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_DIV; 
						}
"%"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_MOD; 
						}
"=="					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_EQ; 
						}
"!="					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_NE; 
						}
"++"					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_PLUS_PLUS; 
						}
"--"					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_MINUS_MINUS; 
						}
">"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_GT; 
						}
"<"						{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_LT; 
						}
">="					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_GE; 
						}
"<="					{
							alpha_lex::alpha_token_t *ptr;
							ptr = gen.gen_operator_token(yylineno, yytext);
							memcpy(ylval, ptr, sizeof(alpha_token_t));
							free(ptr);
							return OP_LE; 
						}

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
