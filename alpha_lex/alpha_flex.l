/* ~~~ user macros  and forward declarations ~~~ */

%{
	#include <iostream>
	#include <string>
	#include <memory>
	#include <vector>
	#include "alpha_token.h"
	#include "alpha_token_generator.h"
	#define YY_DECL int alpha_yylex(void *ylval)

	enum identified_token{UNKNOWN=0, IF, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, AND,
			NOT, OR, LOCAL, BOOL_TRUE, BOOL_FALSE, NIL,
			OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD, OP_EQ, OP_NE, OP_PLUS_PLUS, OP_MINUS_MINUS,
			OP_GT, OP_LT, OP_GE, OP_LE,
			CONST_INT, CONST_REAL, CONST_STR, PUNCTUATION, IDENTIFIER, COMMENT
	};

	alpha_lex::alpha_token_generator token_generator;
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

if						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return IF; }
else					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return ELSE; }
while					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return WHILE; }
for						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return FOR;}
function				{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return FUNCTION; }
return					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return RETURN; }
break					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BREAK; }
continue				{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return CONTINUE; }
and						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return AND; }
not						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return NOT; }
or						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return OR; }
local					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return LOCAL; }
true					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BOOL_TRUE; }
false					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BOOL_FALSE; }
nil						{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return NIL; }

"="						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_ASSIGN; }
"+"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_PLUS; }
"-"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_MINUS; }
"*"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_MUL; }
"/"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_DIV; }
"%"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_MOD; }
"=="					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_EQ; }
"!="					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_NE; }
"++"					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_PLUS_PLUS; }
"--"					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_MINUS_MINUS; }
">"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_GT; }
"<"						{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_LT; }
">="					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_GE; }
"<="					{ token_generator.gen_operator_token(yylineno, yytext, ylval); return OP_LE; }

{const_int}				{ token_generator.gen_const_int_token(yylineno, yytext, ylval); return CONST_INT; }
{digit}*"."{digit}+     { token_generator.gen_const_real_token(yylineno, yytext, ylval); return CONST_REAL; }
{id}                    { token_generator.gen_identifier_token(yylineno, yytext, ylval); return IDENTIFIER; /* id MUST be bellow all the above enumerated rules. */ }

.	        			{ return UNKNOWN; }
\n	        			{ }	/* Work-arround because yylineno was stuck at 1. */

%%

/* ~~~ user code ~~~ */

int main (int argc, char *argv[]) {
    int identified;
    std::vector<alpha_lex::alpha_token_t*> token_vector;
    std::allocator<alpha_lex::alpha_token_t> allocator;
    alpha_lex::alpha_token_t *token_ptr;


	yyin = stdin;
    token_ptr = 0;  /* ignore any compiler warnings */
    while(true) {
        token_ptr = allocator.allocate(1, token_ptr);
        identified = alpha_yylex(token_ptr);
        if(identified == EOF || identified == UNKNOWN) {
            if(identified == UNKNOWN)
                std::cout << "UNIDENTIFIED TOKEN" << std::endl;
			else
				std::cout << "EOF reached" << std::endl;
            allocator.deallocate(token_ptr, 1);
            break;
        }

        /* Valid token here */
		std::cout << "yylex returned: "	<< identified << std::endl;
		std::cout << *token_ptr << std::endl;
        token_vector.push_back(token_ptr);
    }

    for(int i=0; i<token_vector.size(); i++) {
        token_vector[i]->~alpha_token_t();      /* Destructor call since placement new operator was used */
        allocator.deallocate(token_vector[i], 1);
    }	

	return 0;
}
