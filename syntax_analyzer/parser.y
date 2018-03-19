

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

/*declaration of non-terminal symbols*/
%type <voidVal> stmt
%type <realVal> expr term

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

/****************** DEN PEIRAKSA PARAKATW (NIKOS) ******************/

assignexpr:	lvalue ASSIGN expr {}
		;

primary:	lvalue {}
       		| call {}
		| objectdef {}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS {}
		| const {}
		;

lvalue:		IDENTIFIER {}
		| LOCAL IDENTIFIER {}
		| DOUBLE_COLON IDENTIFIER {}
		| member {}
		;

member:		lvalue DOT IDENTIFIER {}
      		| lvalue LEFT_BRACKET expr RIGHT_BRACKET {}
		| call DOT IDENTIFIER {}
		| call LEFT_BRACKET expr RIGHT_BRACKET {}

call:		call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
    		| lvalue callsuffix {}
		| LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
		;

callsuffix:	normcall {}
	  	| methodcall {}
		;

normcall:	LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
		;

methodcall:	DOUBLE_DOT IDENTIFIER LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
	  	;

elist:		expr {}                                              /* TODO check */
     		| elist COMMA expr {}
		| %empty
		;

objectdef:	LEFT_BRACKET elist RIGHT_BRACKET {}
	 	| LEFT_BRACKET indexed RIGHT_BRACKET {}
		;

indexed:	indexedelem {}
       		| indexed COMMA indexedelem {}
		| %empty
		;

indexedelem:	LEFT_BRACE expr COLON expr RIGHT_BRACE {}
	   	;

help_stmt:	help_stmt stmt
 		| %empty
		;	 

block:		LEFT_BRACE help_stmt RIGHT_BRACE {}
     		;

funcdef:	FUNCTION IDENTIFIER LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block            /* TODO check */
       		| FUNCTION LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
		;

const:		CONST_INT {}
     		| CONST_REAL {}
		| CONST_STR {}
		| NIL {}
		| BOOL_TRUE {}
		| BOOL_FALSE {}
		;

idlist:		IDENTIFIER {}                                       /* TODO check */
      		| idlist COMMA IDENTIFIER {}
		| %empty
		;

ifstmt:		IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt 				/* TODO check  prepei na the %prec */
      		| ifstmt ELSE stmt
		;

whilestmt:	WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {}
	 	;

forstmt:	FOR LEFT_PARENTHESIS elist SEMICLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt {}
       		;

returnstmt:	RETURN SEMICOLON {}
	  	| RETURN expr SEMICOLON {}
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
