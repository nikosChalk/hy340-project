
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
	static unsigned int scope=0;
	static value_stack_t *lvalue = new value_stack_t();
	static std::stack<unsigned int> func_scope_stack = std::stack<unsigned int>();
	static bool is_func_open = false;

	int yyerror (const symbol_table &sym_table, char const *msg);
%}

/* bison parameters */
%define api.value.type { syntax_analyzer::value_stack_t }
%language "C"
%output "alpha_bison.cpp"
%defines
%parse-param {syntax_analyzer::symbol_table &sym_table}
%debug
%start program	/*start symbol*/

%initial-action {
	func_scope_stack.push(symbol_table::entry::GLOBAL_SCOPE);
}

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

/* type declaration of non-terminal symbols, defined b the grammar */
%type <strVector> idlist
%type <lvalueType> lvalue
%type <voidVal> program stmt expr term assignexpr primary member
%type <voidVal> call callsuffix normcall methodcall elist objectdef indexed indexedelem block
%type <voidVal> funcdef const ifstmt whilestmt forstmt returnstmt

/* type declaration of non-terminal helper symbols, defined by us */
%type <strVector> tmp_idlist
%type <strVal> tmp_funcdef
%type <voidVal> tmp_elist tmp_indexed tmp_block

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

program:	program	stmt		{$$ = Manage_program__stmt_program();}
       		| %empty			{$$ = Manage_program();}
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
    	| expr PLUS expr 	{$$ = Manage_expr__expr_PLUS_expr();}
		| expr MINUS expr 	{$$ = Manage_expr__expr_MINUS_expr();} 
		| expr MUL expr 	{$$ = Manage_expr__expr_MUL_expr();}
		| expr DIV expr 	{$$ = Manage_expr__expr_DIV_expr();}
		| expr MOD expr 	{$$ = Manage_expr__expr_MOD_expr();}
		| expr GT expr 		{$$ = Manage_expr__expr_GT_expr();}
		| expr GE expr 		{$$ = Manage_expr__expr_GE_expr();}
		| expr LT expr 		{$$ = Manage_expr__expr_LT_expr();}
		| expr LE expr 		{$$ = Manage_expr__expr_LE_expr();}
		| expr EQ expr 		{$$ = Manage_expr__expr_EQ_expr();}
		| expr NE expr 		{$$ = Manage_expr__expr_NE_expr();}
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

lvalue:	IDENTIFIER					{$$ = Manage_lvalue__IDENTIFIER(sym_table, $1, scope, yylineno, func_scope_stack.top()); }
		| LOCAL IDENTIFIER			{$$ = Manage_lvalue__LOCAL_IDENTIFIER(sym_table, $2, scope, yylineno); }
		| DOUBLE_COLON IDENTIFIER	{$$ = Manage_lvalue__DOUBLE_COLON_IDENTIFIER(sym_table, $2, yylineno); }
		| member					{$$ = Manage_lvalue__member(); }
		;

member:	lvalue DOT IDENTIFIER						{$$=Manage_member__lvalue_DOT_IDENTIFIER(sym_table, $3, scope, yylineno, func_scope_stack.top());}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET	{$$=Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET();}
		| call DOT IDENTIFIER						{$$=Manage_member__call_DOT_IDENTIFIER(sym_table, $3, scope, yylineno, func_scope_stack.top());}
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

methodcall:	DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(sym_table, $2, scope, yylineno, func_scope_stack.top()); }
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
/*			| %empty				{$$ = Manage_indexed_empty();} //We changed the grammar because it was disambiguous*/
			;

indexedelem:	LEFT_BRACE expr COLON expr RIGHT_BRACE {$$ = Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE();}
				;

tmp_block:	tmp_block stmt	{$$ = Manage_tmp_block__tmp_block_stmt();}
			| %empty		{$$ = Manage_tmp_block__empty();}
			;	 

block:	LEFT_BRACE {scope++;} tmp_block RIGHT_BRACE {$$ = Manage_block__LEFT_BRACE_tmp_block_RIGHT_BRACE(sym_table, scope); scope--;}  
		;

tmp_funcdef:	IDENTIFIER	{$$ = Manage_tmp_funcdef__IDENTIFIER($1); }
				| %empty	{$$ = Manage_tmp_funcdef__empty();}
				;  

funcdef:	FUNCTION tmp_funcdef {$<voidVal>$ = Manage_funcdef__FUNCTION_tmp_funcdef(sym_table, $2, scope, yylineno);} LEFT_PARENTHESIS {scope++;} idlist RIGHT_PARENTHESIS {scope--;}
			{func_scope_stack.push(scope);} block {func_scope_stack.pop();}
			{$$ = Manage_funcdef__FUNCTION_IDENTIFIER_LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS_block();}
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

idlist:	IDENTIFIER tmp_idlist {$$ = Manage_idlist__IDENTIFIER_tmp_idlist(sym_table, $2, $1, scope, yylineno);}
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
