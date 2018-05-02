
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
%type <exprPtr> expr lvalue member primary assignexpr call term objectdef const
%type <dequeExpr> elist
%type <callSuffixPtr> callsuffix normcall methodcall

%type <voidVal> program stmt
%type <voidVal> indexed indexedelem block
%type <voidVal> ifstmt whilestmt forstmt returnstmt

/* type declaration of non-terminal helper symbols, defined by us */
%type <strVector> tmp_idlist
%type <strVal> funcname
%type <unsignedIntVal> funcbody	/* contains the number of local variables within the function */
%type <unsignedIntVal> ifprefix elseprefix whilecond
%type <funcEntryPtr> funcprefix
%type <dequeExpr> tmp_elist
%type <unsignedIntVal> log_next_quad emit_incomplete_jmp
%type <forPrefixPtr> forprefix

%type <voidVal> breakstmt continuestmt
%type <voidVal> funcargs
%type <voidVal> tmp_indexed
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
		| breakstmt 			{$$ = Manage_stmt__break();				}
		| continuestmt			{$$ = Manage_stmt__continue();			}
		| block 				{$$ = Manage_stmt__block();				}
		| funcdef 				{$$ = Manage_stmt__funcdef();			}
		| SEMICOLON 			{$$ = Manage_stmt__SEMICOLON();			}
		;

returnstmt:	RETURN SEMICOLON		{$$ = Manage_returnstmt__RETURN_SEMICOLON(yylineno);}
			| RETURN expr SEMICOLON	{$$ = Manage_returnstmt__RETURN_expr_SEMICOLON($2, yylineno);}
			;

breakstmt:	BREAK SEMICOLON	{$$ = Manage_breakstmt__BREAK_SEMICOLON();}
		;

continuestmt:	CONTINUE SEMICOLON	{$$ = Manage_continuestmt__CONTINUE_SEMICOLON();}
			;

expr:	assignexpr 			{$$ = Manage_expr__assignexpr();}
    	| expr PLUS expr 	{$$ = Manage_expr__expr_PLUS_expr	(sym_table, yylineno, $1,$3);}
		| expr MINUS expr 	{$$ = Manage_expr__expr_MINUS_expr	(sym_table, yylineno, $1,$3);} 
		| expr MUL expr 	{$$ = Manage_expr__expr_MUL_expr	(sym_table, yylineno, $1,$3);}
		| expr DIV expr 	{$$ = Manage_expr__expr_DIV_expr	(sym_table, yylineno, $1,$3);}
		| expr MOD expr 	{$$ = Manage_expr__expr_MOD_expr	(sym_table, yylineno, $1,$3);}
		| expr GT expr 		{$$ = Manage_expr__expr_GT_expr		(sym_table, yylineno, $1,$3);}
		| expr GE expr 		{$$ = Manage_expr__expr_GE_expr		(sym_table, yylineno, $1,$3);}
		| expr LT expr 		{$$ = Manage_expr__expr_LT_expr		(sym_table, yylineno, $1,$3);}
		| expr LE expr 		{$$ = Manage_expr__expr_LE_expr		(sym_table, yylineno, $1,$3);}
		| expr EQ expr 		{$$ = Manage_expr__expr_EQ_expr		(sym_table, yylineno, $1,$3);}
		| expr NE expr 		{$$ = Manage_expr__expr_NE_expr		(sym_table, yylineno, $1,$3);}
		| expr AND expr 	{$$ = Manage_expr__expr_AND_expr();}
		| expr OR expr 		{$$ = Manage_expr__expr_OR_expr();}
		| term 				{$$ = Manage_expr__term($1);}
		;

term:	LEFT_PARENTHESIS expr RIGHT_PARENTHESIS		{$$ = Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS($2);}
    	| MINUS expr %prec UMINUS 					{$$ = Manage_term__MINUS_expr			(sym_table, yylineno, $2);}	/* Special precedence for this rule */
		| NOT expr 									{$$ = Manage_term__NOT_expr				(sym_table, yylineno, $2);}
		| PLUS_PLUS lvalue 							{$$ = Manage_term__PLUS_PLUS_lvalue		(sym_table, yylineno, $2);}
		| lvalue PLUS_PLUS 							{$$ = Manage_term__lvalue_PLUS_PLUS		(sym_table, yylineno, $1);}
		| MINUS_MINUS lvalue 						{$$ = Manage_term__MINUS_MINUS_lvalue	(sym_table, yylineno, $2);}
		| lvalue MINUS_MINUS 						{$$ = Manage_term__lvalue_MINUS_MINUS	(sym_table, yylineno, $1);}
		| primary 									{$$ = Manage_term__primary($1);}
		;

assignexpr:	lvalue ASSIGN expr {$$ = Manage_assignexpr__lvalue_ASSIGN_expr($1, yylineno);}
			;

primary:	lvalue											{$$ = Manage_primary__lvalue($1); }
			| call											{$$ = Manage_primary__call($1); }
			| objectdef										{$$ = Manage_primary__objectdef($1); }
			| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	{$$ = Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS($2); }
			| const											{$$ = Manage_primary__const($1); }
			;

lvalue:	IDENTIFIER					{$$ = Manage_lvalue__IDENTIFIER(sym_table, $1, yylineno); }
		| LOCAL IDENTIFIER			{$$ = Manage_lvalue__LOCAL_IDENTIFIER(sym_table, $2, yylineno); }
		| DOUBLE_COLON IDENTIFIER	{$$ = Manage_lvalue__DOUBLE_COLON_IDENTIFIER(sym_table, $2, yylineno); }
		| member					{$$ = Manage_lvalue__member($1); }
		;

member:	lvalue DOT IDENTIFIER						{$$=Manage_member__lvalue_DOT_IDENTIFIER($1, $3);}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET	{$$=Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET($1, $3);}
		| call DOT IDENTIFIER						{$$=Manage_member__call_DOT_IDENTIFIER();}
		| call LEFT_BRACKET expr RIGHT_BRACKET		{$$=Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET();}
		;

call:	call normcall	{$$ = Manage_call__call_normcall(sym_table, yylineno, $1, $2;}
		| lvalue callsuffix								{$$ = Manage_call__lvalue_callsuffix(sym_table, yylineno, $1, $2);}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS normcall	{$$ = Manage_call__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_normcall(sym_table, yylineno, $2, $4);}
		;

callsuffix:	normcall		{$$ = Manage_callsuffix__normcall($1);}
			| methodcall	{$$ = Manage_callsuffix__methodcall($1);}
			;

normcall:	LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_normcall__LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS($2);}
			;

methodcall:	DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS($2, $4); }
			;

tmp_elist:	tmp_elist COMMA expr	{$$ = Manage_tmp_elist__tmp_elist_COMMA_expr($1, $3);}
		 	| %empty				{$$ = Manage_tmp_elist__empty();}
			;

elist:	expr tmp_elist	{$$ = Manage_elist__expr_tmp_elist($1, $2);}
		| %empty 		{$$ = Manage_elist__empty();}
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

const:	CONST_INT 		{$$ = Manage_const__CONST_INT	($1);}
		| CONST_REAL	{$$ = Manage_const__CONST_REAL	($1);}
		| CONST_STR 	{$$ = Manage_const__CONST_STR	($1);}
		| NIL 			{$$ = Manage_const__NIL			();}
		| BOOL_TRUE 	{$$ = Manage_const__BOOL_TRUE	();}
		| BOOL_FALSE 	{$$ = Manage_const__BOOL_FALSE	();}
		;

tmp_idlist:	tmp_idlist COMMA IDENTIFIER {$$ = Manage_tmp_idlist__tmp_idlist_COMMA_IDENTIFIER($1, $3);}
			| %empty					{$$ = Manage_tmp_idlist__empty();}
			;

idlist:	IDENTIFIER tmp_idlist {$$ = Manage_idlist__IDENTIFIER_tmp_idlist(sym_table, $2, $1, yylineno);}
		| %empty {$$ = Manage_idlist__empty();}
		;

ifprefix:	IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS { $$ = Manage_ifprefix__IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS($3, yylineno);}
			;

elseprefix: ELSE {$$ = Manage_elseprefix__ELSE(yylineno);}
	    ;

ifstmt:	ifprefix stmt { $$ = Manage_ifstmt__ifprefix_stmt($1);}
		| ifprefix stmt elseprefix stmt	{ $$ = Manage_ifstmt__ifprefix_stmt_elseprefix_stmt($1,$3);}
		;

whilecond:	LEFT_PARENTHESIS expr RIGHT_PARENTHESIS{$$ = Manage_whilecond__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS($2, yylineno);}
			;

whilestmt:	WHILE log_next_quad whilecond stmt {$$ = Manage_whilestmt__WHILE_log_next_quad_whilecond_stmt($2, $3, yylineno);}
			;

forprefix:	FOR LEFT_PARENTHESIS elist SEMICOLON log_next_quad expr SEMICOLON {$$=Manage_forprefix($5,$6,yylineno);}
			;

forstmt:	forprefix emit_incomplete_jmp elist RIGHT_PARENTHESIS emit_incomplete_jmp stmt emit_incomplete_jmp {$$ = Manage_forstmt($1, $2, $5, $7);}
			;

log_next_quad:	%empty	{$$ = Manage_log_next_quad__empty()}
				;

emit_incomplete_jmp:	%empty	{$$ = Manage_emit_incomplete_jmp__empty(yylineno);}
						;

%%

int yyerror (const symbol_table &sym_table, char const *msg){
	std::cerr << msg << ": at line " << std::to_string(yylineno) << ", before token: " << yytext << std::endl;
	std::cerr << "INPUT NOT VALID" << std::endl;
}
