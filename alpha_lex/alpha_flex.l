 /* ~~~ user macros  and forward declarations ~~~ */

%{
	#include <stdio.h>
	#include <iostream>
	#include <string>
	#include <memory>
	#include <vector>
	#include <sstream>
	#include <stack>
	#include "alpha_token.h"
	#include "../utilities.h"
	#include "alpha_token_generator.h"
	#define YY_DECL int alpha_yylex(void *ylval)

	enum identified_token{IF=100, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, AND,
			NOT, OR, LOCAL, BOOL_TRUE, BOOL_FALSE, NIL,
			OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD, OP_EQ, OP_NE, OP_PLUS_PLUS, OP_MINUS_MINUS,
			OP_GT, OP_LT, OP_GE, OP_LE,
			CONST_INT, CONST_REAL, CONST_STR, PUNCT_LEFT_BRACE, PUNCT_RIGHT_BRACE, PUNCT_LEFT_BRACKET, PUNCT_RIGHT_BRACKET, PUNCT_LEFT_PARENTHESIS, PUNCT_RIGHT_PARENTHESIS, 		
			PUNCT_SEMICOLON, PUNCT_COMMA, PUNCT_COLON, PUNCT_DOUBLE_COLON, PUNCT_DOT, PUNCT_ELIPSIS, IDENTIFIER, LINE_COMMENT, BLOCK_COMMENT, NESTED_COMMENT
	};

	/* Global variables within alpha_flex.cpp */
	alpha_lex::alpha_token_generator token_generator;
	std::stack<int> stack_comment = std::stack<int>();
%}

 /* ~~~ FLEX parameters ~~~ */

%option header-file="alpha_flex.h"
%option outfile = "alpha_flex.cpp"
%option noyywrap
%option yylineno
%option stack

 /* ~~~ start conditions */
%x X_STR
%x X_BLOCK_COMMENT
%x X_LINE_COMMENT
%x X_NESTED_COMMENT

 /* ~~~ regex definitions ~~~ */

ws				[ \t\n\v\f\r]+
letter			[a-zA-Z]
digit			[0-9]
underscore		[_]
id				{letter}({letter}|{digit}|_)*
const_int  		{digit}+

 /* ~~~ actions section ~~~ */
%%

%{
	int str_open_lineno;		/* Local variable within yylex function. Used for holding the line where current_str opened */
	std::string current_str; 	/* Local variable within yylex function. Used in string identification */
%}
"if"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return IF; }
"else"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return ELSE; }
"while"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return WHILE; }
"for"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return FOR;}
"function"				{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return FUNCTION; }
"return"				{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return RETURN; }
"break"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BREAK; }
"continue"				{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return CONTINUE; }
"and"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return AND; }
"not"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return NOT; }
"or"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return OR; }
"local"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return LOCAL; }
"true"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BOOL_TRUE; }
"false"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return BOOL_FALSE; }
"nil"					{ token_generator.gen_keyword_token(yylineno, yytext, ylval); return NIL; }

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

"{"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_LEFT_BRACE; }
"}"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_RIGHT_BRACE; }
"["						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_LEFT_BRACKET; }
"]"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_RIGHT_BRACKET; }
"("						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_LEFT_PARENTHESIS; }
")"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_RIGHT_PARENTHESIS; }
";"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_SEMICOLON; }
","						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_COMMA; }
":"						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_COLON; }
"::"					{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_DOUBLE_COLON; }
"."						{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_DOT; }
".."					{ token_generator.gen_punctuation_token(yylineno, yytext, ylval); return PUNCT_ELIPSIS; }

{const_int}				{ token_generator.gen_const_int_token(yylineno, yytext, ylval); return CONST_INT; }
{digit}*"."{digit}+     { token_generator.gen_const_real_token(yylineno, yytext, ylval); return CONST_REAL; }
{id}                    { token_generator.gen_identifier_token(yylineno, yytext, ylval); return IDENTIFIER; /* id MUST be bellow all the above enumerated rules. */ }

{ws}					{ /*ignore white spaces */ }
<<EOF>>					{ 	if(YY_START == X_BLOCK_COMMENT)
								throw std::runtime_error("EOF reached while block comment is open. Opened in line: " + std::to_string(stack_comment.top()));
							if(YY_START == X_NESTED_COMMENT)
								throw std::runtime_error("EOF reached while nested comment is open. Opened in line: " + std::to_string(stack_comment.top()));
							if(YY_START == X_STR)
								throw std::runtime_error("EOF reached while string is open. Opened in line: " + std::to_string(str_open_lineno));
							return EOF; /* Everything was OK */
						}

 /* CONST_STRING token identification */
"\""					{ BEGIN(X_STR); str_open_lineno = yylineno; }

<X_STR>\a							|
<X_STR>\b							|
<X_STR>\f							|
<X_STR>\n							|
<X_STR>\r							|
<X_STR>\t							|
<X_STR>\v				{ current_str.append(utilities::unescape_ch(yytext[0])); }
<X_STR>[^\\]\"			{	/* Identify anything that is not a backslash, and then it is followed by double-quotes. e.g. abcd" */
							current_str.append(utilities::unescape_ch(yytext[0])); /* Do not append the double quote character */
							token_generator.gen_const_str_token(str_open_lineno, current_str, ylval);
							BEGIN(INITIAL);
							str_open_lineno = -1; /* For debugging reasons */
							return CONST_STR;
						}
<X_STR>.			{ current_str.append(yytext); /* Append any character found */ }

 /* deal with unknown characters */
.	        			{ throw std::runtime_error("UNDEFINED input " + std::string(yytext) + " in line: " + std::to_string(yylineno)); }


 /* BLOCK COMMENTS */
"/*"							{ yy_push_state(X_BLOCK_COMMENT); stack_comment.push(yylineno); }

<X_BLOCK_COMMENT,X_NESTED_COMMENT>"/"+"*"		{ yy_push_state(X_NESTED_COMMENT); stack_comment.push(yylineno); }
<X_BLOCK_COMMENT,X_NESTED_COMMENT>[^/*]*		{ }
<X_BLOCK_COMMENT,X_NESTED_COMMENT>"/"+[^*]*		{ }
<X_BLOCK_COMMENT,X_NESTED_COMMENT>"*"+[^/]*		{ }
<X_NESTED_COMMENT>"*"+"/"		{ 	int start_line = stack_comment.top();
									yy_pop_state();
									stack_comment.pop();
									
									std::stringstream ss;
									ss << start_line << " - " << yylineno;

									token_generator.gen_comment_token(start_line, ss.str(), "NESTED_COMMENT", ylval);
									return NESTED_COMMENT;
								}
<X_BLOCK_COMMENT>"*"+"/" 		{ 	int start_line = stack_comment.top();
									yy_pop_state(); 
									stack_comment.pop();
									
									std::stringstream ss;
									ss << start_line << " - " << yylineno;
									token_generator.gen_comment_token(start_line, ss.str(), "BLOCK_COMMENT", ylval);

									return BLOCK_COMMENT;
								}

 /* LINE COMMENTS*/
"//"					{ BEGIN(X_LINE_COMMENT); }

<X_LINE_COMMENT>[^\n]*	{ }
<X_LINE_COMMENT>"\n"	{ BEGIN(INITIAL); token_generator.gen_comment_token(yylineno-1, "", "LINE_COMMENT", ylval); return LINE_COMMENT; }

%%

 /* ~~~ user code ~~~ */

int main (int argc, char *argv[]) {
    int identified;
    std::vector<analyzer::alpha_token_t*> token_vector;
    std::allocator<analyzer::alpha_token_t> allocator;
    analyzer::alpha_token_t *token_ptr;
	FILE *tmp;


	/* Check command line arguments and open I/O files */
	if(argc < 2) {
		std::cerr << "Too few arguments. Expected input file." << std::endl;
		exit(EXIT_FAILURE);
	}
	if(argc >= 2) {
		if(!(yyin=fopen(argv[1], "r"))) {
			perror("Error while opening input file. Execution aborted");
			exit(EXIT_FAILURE);
		}
		std::cout << "Using input file: " << argv[1] << std::endl;
	}
	if(argc>=3) {
		if(!(yyout=fopen(argv[2], "w"))) {
			perror("Error while opening output file. Using stdout instead.");
			yyout = stdout;
		} else {
			std::cout << "Using output file: " << argv[2] << std::endl;
		}
	}

    token_ptr = 0;  /* ignore any compiler warnings */
    while(true) {
        token_ptr = allocator.allocate(1, token_ptr);

		try {
			identified = alpha_yylex(token_ptr);
		} catch(std::runtime_error &error) {	/* Catch any possible errors such as unknown tokens, malformed escape sequences in strings, EOF reach while block comment is open, etc. */
			fprintf(yyout, "%s\n", error.what());
			allocator.deallocate(token_ptr, 1);
			break;
		}
        
        if(identified == EOF) {
			fprintf(yyout, "EOF reached. Success!\n");
            allocator.deallocate(token_ptr, 1);
            break;
        }

        /* Valid token here */
		fprintf(yyout, "%s\n", (token_ptr->to_str().c_str()));
        token_vector.push_back(token_ptr);
    }

    for(int i=0; i<token_vector.size(); i++) {
        token_vector[i]->~alpha_token_t();      /* Destructor call since placement new operator was used */
        allocator.deallocate(token_vector[i], 1);
    }

	if(yyin != stdin) {
		if(fclose(yyin) == EOF)
			perror("Error while closing input file.");
	}
	if(yyout != stdout) {
		if(fclose(yyout) == EOF)
			perror("Error while closing output file.");
	}

	return 0;
}
