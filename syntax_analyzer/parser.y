
%code requires {
	#include "symbol_table.h"
	#include "types.h"
}

%{
	#include <cstdio>
	#include <string>
	#include <iostream>
	#include <stack>
	#include "parser_manager.h"

	using namespace syntax_analyzer;

	extern int yylex();
	extern int yylineno;
	extern char *yytext;

	int yyerror (const symbol_table &sym_table, char const *msg);
%}

/* bison parameters */
%define api.value.type { syntax_analyzer::value_stack_t }
%language "C"
%output "alpha_bison.cpp"
%defines "alpha_bison.h"
%parse-param {syntax_analyzer::symbol_table &sym_table}
%debug
%start program	/*start symbol*/


/*declaration of terminal symbols*/
%token <voidVal>	IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL BOOL_TRUE BOOL_FALSE NIL
%token <voidVal>	ASSIGN PLUS MINUS MUL DIV MOD 
%token <voidVal>	PLUS_PLUS MINUS_MINUS 
%token <voidVal>	EQ NE GT LT GE LE
%token <voidVal>	LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token <voidVal>	SEMICOLON COMMA COLON DOUBLE_COLON DOT DOUBLE_DOT

%token <strVal>		CONST_STR IDENTIFIER
%token <intVal>		CONST_INT
%token <realVal>	CONST_REAL

/* type declaration of non-terminal symbols, defined by the grammar */
%type <strVector> idlist
%type <funcEntryPtr> funcdef
%type <exprPtr> lvalue member primary assignexpr call term objectdef const

%type <voidVal> program stmt expr
%type <voidVal> callsuffix normcall methodcall elist indexed indexedelem block
%type <voidVal> ifstmt whilestmt forstmt returnstmt

/* type declaration of non-terminal helper symbols, defined by us */
%type <strVector> tmp_idlist
%type <strVal> funcname
%type <intVal> funcbody	/* contains the number of local variables within the function */
%type <funcEntryPtr> funcprefix
%type <uIntVector> break continue

%type <voidVal> funcargs
%type <voidVal> tmp_elist tmp_indexed
%type <voidVal> block_open stmts block_close

/* Define the priority of tokens */
%right		ASSIGN
%left		OR
%left		AND
%nonassoc 	EQ NE
%nonassoc	GT GE LT LE
%left		PLUS MINUS
%left		MUL DIV MOD
%right		NOT PLUS_PLUS MINUS_MINUS UMINUS	/*UMINUS is a pseudo-token never return by lex. We just need its precedence for a grammar rule in order to resolve shift/reduce conflict */
%left		DOT DOUBLE_DOT
%left		LEFT_BRACKET RIGHT_BRACKET
%left		LEFT_PARENTHESIS RIGHT_PARENTHESIS

%%

program:	stmts	{$$ = Manage_program__stmts();}
			;

stmts:		stmts stmt		{$$ = Manage_stmts__stmts_stmt();}
			| %empty		{$$ = Manage_stmts__empty();}
			;

stmt:	expr SEMICOLON			{$$ = Manage_stmt__expr_SEMICOLON(); 	}
    	| ifstmt 				{$$ = Manage_stmt__ifstmt(); 			}
		| whilestmt 			{$$ = Manage_stmt__whilestmt(); 		}
		| forstmt 				{$$ = Manage_stmt__forstmt(); 			}
		| returnstmt 			{$$ = Manage_stmt__returnstmt(); 		}
		| BREAK SEMICOLON 		{$$ = Manage_stmt__BREAK_SEMICOLON();	}
		| CONTINUE SEMICOLON	{$$ = Manage_stmt__CONTINUE_SEMICOLON();}
		| block 				{$$ = Manage_stmt__block();				}
		| funcdef 				{$$ = Manage_stmt__funcdef();			}
		| SEMICOLON 			{$$ = Manage_stmt__SEMICOLON();			}
		;

expr:	assignexpr 			{$$ = Manage_expr__assignexpr();}
    	| expr PLUS expr 	{$$ = Manage_expr__expr_PLUS_expr(sym_table,$1,$3,yylineno);} /* we must change expr type */
		| expr MINUS expr 	{$$ = Manage_expr__expr_MINUS_expr(sym_table,$1,$3,yylineno);} 
		| expr MUL expr 	{$$ = Manage_expr__expr_MUL_expr(sym_table,$1,$3,yylineno);}
		| expr DIV expr 	{$$ = Manage_expr__expr_DIV_expr(sym_table,$1,$3,yylineno);}
		| expr MOD expr 	{$$ = Manage_expr__expr_MOD_expr(sym_table,$1,$3,yylineno);}
		| expr GT expr 		{$$ = Manage_expr__expr_GT_expr(sym_table,$1,$3,yylineno);}
		| expr GE expr 		{$$ = Manage_expr__expr_GE_expr(sym_table,$1,$3,yylineno);}
		| expr LT expr 		{$$ = Manage_expr__expr_LT_expr(sym_table,$1,$3,yylineno);}
		| expr LE expr 		{$$ = Manage_expr__expr_LE_expr(sym_table,$1,$3,yylineno);}
		| expr EQ expr 		{$$ = Manage_expr__expr_EQ_expr(sym_table,$1,$3,yylineno);}
		| expr NE expr 		{$$ = Manage_expr__expr_NE_expr(sym_table,$1,$3,yylineno);}
		| expr AND expr 	{$$ = Manage_expr__expr_AND_expr();}
		| expr OR expr 		{$$ = Manage_expr__expr_OR_expr();}
		| term 				{$$ = Manage_expr__term();}
		;

term:	LEFT_PARENTHESIS expr RIGHT_PARENTHESIS		{$$ = Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS();}
    	| MINUS expr %prec UMINUS 					{$$ = Manage_term__MINUS_expr();}	/* Special precedence for this rule */
		| NOT expr 									{$$ = Manage_term__NOT_expr();}
		| PLUS_PLUS lvalue 							{$$ = Manage_term__PLUS_PLUS_lvalue($2, yylineno);}
		| lvalue PLUS_PLUS 							{$$ = Manage_term__lvalue_PLUS_PLUS($1, yylineno);}
		| MINUS_MINUS lvalue 						{$$ = Manage_term__MINUS_MINUS_lvalue($2, yylineno);}
		| lvalue MINUS_MINUS 						{$$ = Manage_term__lvalue_MINUS_MINUS($1, yylineno);}
		| primary 									{$$ = Manage_term__primary();}
		;

assignexpr:	lvalue ASSIGN expr {$$ = Manage_assignexpr__lvalue_ASSIGN_expr($1, yylineno);}
			;

primary:	lvalue											{$$ = Manage_primary__lvalue(); }
			| call											{$$ = Manage_primary__call(); }
			| objectdef										{$$ = Manage_primary__objectdef(); }
			| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	{$$ = Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS(); }
			| const											{$$ = Manage_primary__const(); }
			;

lvalue:	IDENTIFIER					{$$ = Manage_lvalue__IDENTIFIER(sym_table, $1, yylineno); }
		| LOCAL IDENTIFIER			{$$ = Manage_lvalue__LOCAL_IDENTIFIER(sym_table, $2, yylineno); }
		| DOUBLE_COLON IDENTIFIER	{$$ = Manage_lvalue__DOUBLE_COLON_IDENTIFIER(sym_table, $2, yylineno); }
		| member					{$$ = Manage_lvalue__member(); }
		;

member:	lvalue DOT IDENTIFIER						{$$=Manage_member__lvalue_DOT_IDENTIFIER();}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET	{$$=Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET();}
		| call DOT IDENTIFIER						{$$=Manage_member__call_DOT_IDENTIFIER();}
		| call LEFT_BRACKET expr RIGHT_BRACKET		{$$=Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET();}
		;

call:	call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS	{$$ = Manage_call_call_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();}
		| lvalue callsuffix								{$$ = Manage_call_lvalue_callsuffix();}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS	{
			$$ = Manage_call_LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();
		}
		;

callsuffix:	normcall		{$$ = Manage_callsuffix_normcall();}
			| methodcall	{$$ = Manage_callsuffix_methodcall();}
			;

normcall:	LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_normcall_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();}
			;

methodcall:	DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(); }
			;

tmp_elist:	tmp_elist COMMA expr	{$$ = Manage_tmp_elist_tmp_elist_COMMA_expr();}
		 	| %empty				{$$ = Manage_tmp_elist_empty();}
			;

elist:	expr tmp_elist	{$$ = Manage_elist__expr_tmp_elist();}
		| %empty 		{$$ = Manage_elist_empty();}
		;

objectdef:	LEFT_BRACKET elist RIGHT_BRACKET		{$$ = Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET();}
			| LEFT_BRACKET indexed RIGHT_BRACKET	{$$ = Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET();}
			;

tmp_indexed:	tmp_indexed COMMA indexedelem	{$$ = Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem();}
				|%empty							{$$ = Manage_tmp_indexed_empty();}
				;

indexed:	indexedelem tmp_indexed	{$$ = Manage_indexed__indexedelem_tmp_indexed();}
			;

indexedelem:	LEFT_BRACE expr COLON expr RIGHT_BRACE {$$ = Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE();}
				;

block_open:		LEFT_BRACE	{$$ = Manage_block_open__LEFT_BRACE();}
				;

block_close:	RIGHT_BRACE {$$ = Manage_block_close__RIGHT_BRACE();}
				;

block:	block_open stmts block_close {$$ = Manage_block__block_open_stmts_block_close(sym_table);}
		;

funcname:	IDENTIFIER	{$$ = Manage_funcname__IDENTIFIER($1); }
			| %empty	{$$ = Manage_funcname__empty();}
			;

funcprefix:	FUNCTION funcname {$$ = Manage_funcprefix__FUNCTION_funcname(sym_table, $2, yylineno);}
			;

funcargs:	LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS	{$$ = Manage_funcargs__LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS();}
			;

funcbody:	block {$$ = Manage_funcbody__block();}
			;

funcdef:	funcprefix funcargs funcbody	{$$ = Manage_funcdef__funcprefix_funcargs_funcbody($1, yylineno, $3);}
			;

const:	CONST_INT 		{$$ = Manage_const_CONST_INT();}
		| CONST_REAL	{$$ = Manage_const_CONST_REAL();}
		| CONST_STR 	{$$ = Manage_const_CONST_STR();}
		| NIL 			{$$ = Manage_const_NIL();}
		| BOOL_TRUE 	{$$ = Manage_const_BOOL_TRUE();}
		| BOOL_FALSE 	{$$ = Manage_const_BOOL_FALSE();}
		;

tmp_idlist:	tmp_idlist COMMA IDENTIFIER {$$ = Manage_tmp_idlist__tmp_idlist_COMMA_IDENTIFIER($1, $3);}
			| %empty					{$$ = Manage_tmp_idlist__empty();}
			;

idlist:	IDENTIFIER tmp_idlist {$$ = Manage_idlist__IDENTIFIER_tmp_idlist(sym_table, $2, $1, yylineno);}
		| %empty {$$ = Manage_idlist__empty();}
		;

ifstmt:	IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt				{ $$ = Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();}
		| IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt	{ $$ = Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt_ELSE_stmt();}
		;

whilestmt:	WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {$$ = Manage_WHILE_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();}
			;

forstmt:	FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt {$$ = Manage_FOR_LEFT_PARENTHESIS_elist_SEMICLON_expr_SEMICOLON_elist_RIGHT_PARENTHESIS_stmt();}
			;

returnstmt:	RETURN SEMICOLON		{$$ = Manage_RETURN_SEMICOLON();}
			| RETURN expr SEMICOLON	{$$ = Manage_RETURN_expr_SEMICOLON();}
			;

%%

int yyerror (const symbol_table &sym_table, char const *msg){
	std::cerr << msg << ": at line " << std::to_string(yylineno) << ", before token: " << yytext << std::endl;
	std::cerr << "INPUT NOT VALID" << std::endl;
}
