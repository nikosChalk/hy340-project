

%{
	#include <stdio.h>
	#include <string>
	#include "types.h"
	#include "parser_manager.h"

	using namespace syntax_analyzer;

	extern int yylex(union YYSTYPE *yylval)
	extern int yylineno;
	extern char *yytext;
	extern FILE *yyin;
	unsigned int scope=0;
	int func_flag = 0;
	std::string func_id;

	int yyerror (char *msg);
%}

/* bison parameters */
%define api.value.type { union YYSTYPE }
%language "C++"
%output "alpha_bison"
%debug			/*TODO: validate this choice */
%verbose
%expect 1 		/* Expect 1 conflict */
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

/* type declaration of non-terminal symbols, defined b the grammar */
%type <voidVal> program stmt expr term assignexpr primary lvalue member
%type <voidVal> call callsuffix normcall methodcall elist objectdef indexed indexedelem block
%type <voidVal> funcdef const idlist ifstmt whilestmt forstmt returnstmt

/* type declaration of non-terminal helper symbols, defined by us */
%type <voidVal> tmp_elist tmp_indexed tmp_idlist tmp_funcdef

/*priority*/
/*TODO: check if they are correct. Shouldn't they be in reverse order? */
%right		ASSIGN
%left		OR
%left		AND
%nonassoc 	EQ NE
%nonassoc	GT GE LT LE
%left		PLUS MINUS
%left		MUL DIV MOD
%right		NOT PLUS_PLUS MINUS_MINUS '-'	/*TODO: why minus(-) is again here? */
%left		DOT DOUBLE_DOT
%left		LEFT_BRACKET RIGHT_BRACKET
%left		LEFT_PARENTHESIS RIGHT_PARENTHESIS

%%

program:	program stmt	/*TODO: actions */
       		| %empty
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

expr:	assignexpr 			{/*TODO: bellow actions */}
    	| expr PLUS expr 	{$$ = Manage_expr__expr_PLUS_expr($1, $3); /* Example of how rules should be */ }
		| expr MINUS expr 	{$$ = $1 - $3;} 
		| expr MUL expr 	{$$ = $1 * $3;}
		| expr DIV expr 	{$$ = $1 / $3;}
		| expr MOD expr 	{$$ = $1 % $3;}
		| expr GT expr 		{$$ = ($1 > $3)?1:0;}
		| expr GE expr 		{$$ = ($1 >= $3)?1:0;}
		| expr LT expr 		{$$ = ($1 < $3)?1:0;}
		| expr LE expr 		{$$ = ($1 <= $3)?1:0;}
		| expr EQ expr 		{$$ = ($1 == $3)?1:0;}
		| expr NE expr 		{$$ = ($1 != $3)?1:0;}
		| expr AND expr 	{$$ = ($1 && $3)?1:0;}
		| expr OR expr 		{$$ = Manage_expr__expr_OR_expr($1, $3); /* Example of how rules should be */ }
		| term 				{}
		;

term:	LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
    	| MINUS expr %prec UMINUS {}	/* TODO: validate this */
		| NOT expr {}
		| PLUS_PLUS lvalue {}
		| lvalue PLUS_PLUS {}
		| MINUS_MINUS lvalue {}
		| lvalue MINUS_MINUS {}
		| primary {}
		;

assignexpr:	lvalue ASSIGN expr {$$=Manage_assignexpr__lvalue_ASSIGN_expr();}
			;

primary:	lvalue											{$$ = Manage_primary__lvalue(); }
			| call											{$$ = Manage_primary__call(); }
			| objectdef										{$$ = Manage_primary__objectdef(); }
			| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS	{$$ = Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS(); }
			| const											{$$ = Manage_primary__const(); }
			;

lvalue:	IDENTIFIER					{$$ = Manage_lvalue__IDENTIFIER(); }
		| LOCAL IDENTIFIER			{$$ = Manage_lvalue__LOCAL_IDENTIFIER(); }
		| DOUBLE_COLON IDENTIFIER	{$$ = Manage_lvalue__DOUBLE_COLON_IDENTIFIER(); }
		| member					{$$ = Manage_lvalue__member(); }
		;

member:	lvalue DOT IDENTIFIER						{$$=Manage_member__lvalue_DOT_IDENTIFIER();}
		| lvalue LEFT_BRACKET expr RIGHT_BRACKET	{$$=Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET();}
		| call DOT IDENTIFIER						{$$=Manage_member__call_DOT_IDENTIFIER();}
		| call LEFT_BRACKET expr RIGHT_BRACKET		{$$=Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET();}
		;

/*************GIWRGOS*************/

call:		call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_call_call_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();}
    		| lvalue callsuffix {$$ = Manage_call_lvalue_callsuffix();}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {
			$$ = Manage_call_LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();
		}
		;

callsuffix:	normcall {$$ = Manage_callsuffix_normcall();}
			| methodcall {$$ = Manage_callsuffix_methodcall();}
			;

normcall:	LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_normcall_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();}
			;

methodcall:	DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = Manage_methodcall_DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(); }
			;

tmp_elist:	tmp_elist COMMA expr {$$ = Manage_tmp_elist_tmp_elist_COMMA_expr();}
		 	| %empty {$$ = Manage_tmp_elist_empty();}
			;

elist:		expr tmp_elist
		| %empty {$$ = Manage_elist_empty();}
		;

objectdef:	LEFT_BRACKET elist RIGHT_BRACKET {$$ = Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET();}
			| LEFT_BRACKET indexed RIGHT_BRACKET {$$ = Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET();}
			;

tmp_indexed:	tmp_indexed COMMA indexedelem {$$ = Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem();}
				|%empty {$$ = Manage_tmp_indexed_empty();}
				;

indexed:	indexedelem tmp_indexed
			| %empty {$$ = Manage_indexed_empty();}
			;

indexedelem:	LEFT_BRACE expr COLON expr RIGHT_BRACE {$$ = Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE();}
				;

tmp_block:	tmp_block stmt
 		| %empty
		;	 

block:		LEFT_BRACE {
                            if(func_flag == 0){scope++;}
                            func_flag = 0; 
                           } tmp_block RIGHT_BRACE {$$ = Manage_block_LEFT_BRACE_tmp_block_RIGHT_BRACE(scope); scope--;}  
     		;

tmp_funcdef:	IDENTIFIER{func_id = $1;}
   		| %empty { $$ = Manage_tmp_funcdef_empty(&func_id);}	   

funcdef:	FUNCTION tmp_funcdef{
       					$$ = Manage_funcdef_FUNCTION_tmp_funcdef(&func_id,scope,yylineno);
					} LEFT_PARENTHESIS{scope++;func_flag=1;} idlist RIGHT_PARENTHESIS block {
       			$$ = Manage_funcdef_FUNCTION_IDENTIFIER_LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS_block();
		}         
		;

const:		CONST_INT {$$ = Manage_const_CONST_INT();}
     		| CONST_REAL {$$ = Manage_const_CONST_REAL();}
		| CONST_STR {$$ = Manage_const_CONST_STR();}
		| NIL {$$ = Manage_const_NIL();}
		| BOOL_TRUE {$$ = Manage_const_BOOL_TRUE();}
		| BOOL_FALSE {$$ = Manage_const_BOOL_FALSE();}
		;

tmp_idlist:	tmp_idlist COMMA IDENTIFIER{$$ = Manage_tmp_idlist_tmp_idlist_COMMA_IDENTIFIER($3,scope,yylineno);}
	  	| %empty { $$ = Manage_tmp_idlist_empty();}
		;

idlist:		IDENTIFIER {
      				$$ = Manage_idlist_IDENTIFIER($1,scope,yylineno);
				}tmp_idlist	{$$=Manage_idlist__IDENTIFIER_tmp_idlist();}
		| %empty				{$$ = Manage_idlist_empty();}
		;

ifstmt:		IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt { $$ = Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();}
      		| IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt { $$ = Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt_ELSE_stmt();}
		;

whilestmt:	WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {$$ = Manage_WHILE_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();}
	 	;

forstmt:	FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt {
       			$$ = Manage_FOR_LEFT_PARENTHESIS_elist_SEMICLON_expr_SEMICOLON_elist_RIGHT_PARENTHESIS_stmt();
		}
       		;

returnstmt:	RETURN SEMICOLON {$$ = Manage_RETURN_SEMICOLON();}
	  	| RETURN expr SEMICOLON {$$ = Manage_RETURN_expr_SEMICOLON();}
		;

%%

int yyerror (char* msg){
	fprintf(stderr, "%s: at line %d, before token: %s\n", msg, yylineno, yytext);
	fprintf(stderr,"INPUT NOT VALID\n");
}

int main(int argc , char* argv[]){

	int identified;
	if(argc > 1){
		if(!(yyin = fopen(argv[1],"r"))){
			fprintf(stderr,"Cannot read file: %s\n",argv[1]);
			return 1;
		}
	}
	else{ 
		yyin = stdin; 
	}

	identified = yyparse();
	if(identified == EOF) {
	 	   fprintf(stdout, "EOF reached. Success!\n");
	}
	return 0;
}
