%{
	#include <stdio.h>
	#include <string>
	int yyerror (char* yaccProvideMessage);
	int yylex (void);
	extern int yylineno;
	extern char* yytext;
	extern FILE* yyin;
%}

%union {
        int   		intVal;
	double 		realVal;
	std::string 	strVal;
}

/*start symbol*/
%start program

/*declaration of terminal symbols*/
%token <intVal>		CONST_INT
%token <realVal>	CONST_REAL
%token <strVal>		CONST_STR IDENTIFIER LINE_COMMENT BLOCK_COMMENT NESTED_COMMENT  /* mporei na min xreiazontai ta comments*/

/*declaration of non-terminal symbols*/
%type <>


/*priority*/
%right		'='
%left		'or'
%left		'and'
%nonassoc 	'==' '!='
%nonassoc	 '>' '>=' '<' '<='
%left		'+' '-'
%left		'*' '/' '%'
%right		'not' '++' '--' '-'
%left		'.' '..'
%left		'[' ']'
%left		'(' ')'

%%

program:	stmt program
       		| /*empty*/ {;}
		;

stmt:		expr ';'
    		| ifstmt {}
		| whilestmt {}
		| forstmt {}
		| returnstmt {}
		| 'break' ';' {}
		| 'continue' ';' {}
		| block {}
		| funcdef {}
		| ';' {}
		;

expr:		assignexpr {}
    		| expr '+' expr {$$ = $1 + $3;}
		| expr '-' expr {$$ = $1 - $3;} 
		| expr '*' expr {$$ = $1 * $3;}
		| expr '/' expr {$$ = $1 / $3;}
		| expr '%' expr {$$ = $1 % $3;}
		| expr '>' expr {$$ = ($1 > $3)?1:0;}
		| expr '>=' expr {$$ = ($1 >= $3)?1:0;}
		| expr '<' expr {$$ = ($1 < $3)?1:0;}
		| expr '<=' expr {$$ = ($1 <= $3)?1:0;}
		| expr '==' expr {$$ = ($1 == $3)?1:0;}
		| expr '!=' expr {$$ = ($1 != $3)?1:0;}
		| expr 'and' expr {$$ = ($1 && $3)?1:0;}
		| expr 'or' expr {$$ = ($1 || $3)?1:0;}
		| term {}
		;

term:		'(' expr ')'
    		| '-' expr %prec UMINUS {}
		| 'not' expr {}
		| '++' lvalue {}
		| lvalue '++' {}
		| '--' lvalue {}
		| lvalue '--' {}
		| primary {}
		;

assignexpr:	lvalue '=' expr {}
		;

primary:	lvalue {}
       		| call {}
		| objectdef {}
		| '(' functdef ')' {}
		| const {}
		;

lvalue:		IDENTIFIER {}
		| 'local' IDENTIFIER {}
		| '::' IDENTIFIER {}
		| member {}
		;

member:		lvalue '.' IDENTIFIER {}
      		| lvalue '[' expr ']' {}
		| call '.' IDENTIFIER {}
		| call '[' expr ']' {}

call:		call '(' elist ')' {}
    		| lvalue callsuffix {}
		| '(' funcdef ')' '(' elist ')' {}
		;

callsuffix:	normcall {}
	  	| methodcall {}
		;

normcall:	'(' elist ')' {}
		;

methodcall:	'..' IDENTIFIER '(' elist ')' {}
	  	;

elist:		expr {}                                              /* TODO check */
     		| elist ',' expr {}
		| /* empty */ {;}
		;

objectdef:	'[' elist ']' {}
	 	| '[' indexed ']' {}
		;

indexed:	indexedelem {}
       		| indexed ',' indexedelem {}
		;

indexedelem:	'{' expr ':' expr '}' {}
	   	;

help_stmt:	help_stmt stmt
 		| /* empty */ {;}
		;	 

block:		'{' help_stmt '}' {}
     		;

funcdef:	'function' IDENTIFIER '(' idlist ')' block            /* TODO check */
		;

const:		CONST_INT {}
     		| CONST_REAL {}
		| CONST_STR {}
		| 'nil' {}
		| 'true' {}
		| 'false' {}
		;

idlist:		IDENTIFIER {}                                       /* TODO check */
      		| idlist ',' IDENTIFIER {}
		| /* empty */ {;}
		;

ifstmt:		'if' '(' expr ')' stmt 				/* TODO check  prepei na the %prec */
      		| ifstmt 'else' stmt
		;

whilestmt:	'while' '(' empr ')' stmt {}
	 	;

forstmt:	'for' '(' elist ';' expr ';' elist ')' stmt {}
       		;

returnstmt:	'return' ';' {}
	  	| 'return' expr ';' {}
		;

%%

int yyerror (char* yaccProvideMessage){
	fprintf(stderr, "%s: at line %d, before token: %s\n",yaccProvideMessage,yylineno,yytext);
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
