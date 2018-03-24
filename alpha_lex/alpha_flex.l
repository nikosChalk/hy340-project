 /* ~~~ user macros  and forward declarations ~~~ */

%{
	#include <stdio.h>
	#include <iostream>
	#include <string>
	#include <memory>
	#include <vector>
	#include <sstream>
	#include <stack>
	#include <stdexcept>
	#include "../utilities.h"
	#include "../syntax_analyzer/alpha_bison.hpp"
	#include "../syntax_analyzer/types.h"
	#define YY_DECL int yylex(syntax_analyzer::value_stack_t *yylval)

	/* Global variables within alpha_flex.cpp */
	static std::stack<int> stack_comment = std::stack<int>();
%}

 /* ~~~ FLEX parameters ~~~ */

%option header-file="alpha_flex.h"
%option outfile = "alpha_flex.cpp"
%option noyywrap
%option yylineno
%option stack

 /* ~~~ start conditions */
%x X_STR
%x X_STR_ESC
%x X_BLOCK_COMMENT
%x X_LINE_COMMENT
%x X_NESTED_COMMENT

 /* ~~~ regex definitions ~~~ */

ws				[ \t\n\v\f\r]+
letter			[a-zA-Z]
digit			[0-9]
underscore		[_]
id				{letter}({letter}|{digit}|{underscore})*
const_int  		{digit}+
const_real		{digit}*"."{digit}+

 /* ~~~ actions section ~~~ */
%%

%{
	int str_open_lineno;		/* Local variable within yylex function. Used for holding the line where current_str opened */
	std::string current_str; 	/* Local variable within yylex function. Used in string identification */
%}

"if"					{ return IF; }
"else"					{ return ELSE; }
"while"					{ return WHILE; }
"for"					{ return FOR;}
"function"				{ return FUNCTION; }
"return"				{ return RETURN; }
"break"					{ return BREAK; }
"continue"				{ return CONTINUE; }
"and"					{ return AND; }
"not"					{ return NOT; }
"or"					{ return OR; }
"local"					{ return LOCAL; }
"true"					{ yylval->boolVal = true; return BOOL_TRUE; }
"false"					{ yylval->boolVal = false; return BOOL_FALSE; }
"nil"					{ return NIL; }

"="						{ return ASSIGN; }
"+"						{ return PLUS; }
"-"						{ return MINUS; }
"*"						{ return MUL; }
"/"						{ return DIV; }
"%"						{ return MOD; }
"=="					{ return EQ; }
"!="					{ return NE; }
"++"					{ return PLUS_PLUS; }
"--"					{ return MINUS_MINUS; }
">"						{ return GT; }
"<"						{ return LT; }
">="					{ return GE; }
"<="					{ return LE; }

"{"						{ return LEFT_BRACE; }
"}"						{ return RIGHT_BRACE; }
"["						{ return LEFT_BRACKET; }
"]"						{ return RIGHT_BRACKET; }
"("						{ return LEFT_PARENTHESIS; }
")"						{ return RIGHT_PARENTHESIS; }
";"						{ return SEMICOLON; }
","						{ return COMMA; }
":"						{ return COLON; }
"::"					{ return DOUBLE_COLON; }
"."						{ return DOT; }
".."					{ return DOUBLE_DOT; }

{const_int}				{ yylval->intVal = std::stoi(std::string(yytext)); return CONST_INT; }
{const_real}			{ yylval->realVal = std::stod(std::string(yytext)); return CONST_REAL; }
{id}                    { yylval->strVal = std::string(yytext); return IDENTIFIER; /* id MUST be bellow all the above enumerated rules. */ }

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

<X_STR>\\				{ BEGIN(X_STR_ESC); current_str.append(yytext); }
<X_STR_ESC>.|\n			{ BEGIN(X_STR); 	current_str.append(utilities::unescape_ch(yytext[0])); /* Appened user's escaped sequence and validate it later */ }

<X_STR>\"				{	/* Identify a non-escaped double quote. This means that the string has closed. We do no append the double quote character. */
							yylval->strVal = utilities::escape_str(current_str); /* Replace unescaped characters (e.g. char *str = {'\\' 'n'}; ) within str with the corresponding escaped character */
							BEGIN(INITIAL);
							str_open_lineno = -1; /* For debugging reasons */
							return CONST_STR;
						}

<X_STR>.|\n				{ current_str.append(utilities::unescape_ch(yytext[0])); /* Append any character found */ }


 /* BLOCK COMMENTS */
"/*"					{ yy_push_state(X_BLOCK_COMMENT); stack_comment.push(yylineno); }

<X_BLOCK_COMMENT,X_NESTED_COMMENT>"/*"	{ yy_push_state(X_NESTED_COMMENT); stack_comment.push(yylineno); }

<X_BLOCK_COMMENT>"*/"					|
<X_NESTED_COMMENT>"*/"	{ yy_pop_state(); stack_comment.pop(); /* DO NOT RETURN */ }

<X_BLOCK_COMMENT,X_NESTED_COMMENT>.|\n	{ ; }

 /* LINE COMMENTS*/
"//"					{ BEGIN(X_LINE_COMMENT); }

<X_LINE_COMMENT>[^\n]*	{ ; }
<X_LINE_COMMENT>\n		{ BEGIN(INITIAL); /* DO NOT RETURN */ }


 /* deal with unknown characters */
.	        			{ throw std::runtime_error("UNDEFINED input " + std::string(yytext) + " in line: " + std::to_string(yylineno)); }

%%
