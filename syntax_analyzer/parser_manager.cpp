//
// Created by nikos on 3/20/18.
//

#include "parser_manager.h"
#include "symbol_table.h"
#include <vector>
#include "../not_implemented_error.h"

extern FILE *yyout;

namespace syntax_analyzer {

    void_t Manage_program__stmt_program() {
        throw not_implemented_error();
    }
    void_t Manage_program() {
        throw not_implemented_error();
    }


    void_t Manage_stmt__expr_SEMICOLON() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__ifstmt() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__whilestmt() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__forstmt() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__returnstmt() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__BREAK_SEMICOLON() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__CONTINUE_SEMICOLON() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__block() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__funcdef() {
        throw not_implemented_error();
    }
    void_t Manage_stmt__SEMICOLON() {
        throw not_implemented_error();
    }


    void_t Manage_expr__assignexpr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_PLUS_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_MINUS_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_MUL_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_DIV_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_MOD_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_GT_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_GE_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_LT_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_LE_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_EQ_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_NE_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_AND_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__expr_OR_expr() {
        throw not_implemented_error();
    }
    void_t Manage_expr__term(void_t term) {
        throw not_implemented_error();
    }

    void_t Manage_assignexpr__lvalue_ASSIGN_expr() {
        fprintf(yyout, "assignexpr -> lvalue = expr\n");
        return void_value;
    }


    void_t Manage_primary__lvalue() {
        fprintf(yyout, "primary -> lvalue\n");
        return void_value;
    }
    void_t Manage_primary__call() {
        fprintf(yyout, "primary -> call\n");
        return void_value;
    }
    void_t Manage_primary__objectdef() {
        fprintf(yyout, "primary -> objectdef\n");
        return void_value;
    }
    void_t Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS() {
        fprintf(yyout, "primary -> (funcdef)\n");
        return void_value;
    }
    void_t Manage_primary__const() {
        fprintf(yyout, "primary -> const\n");
        return void_value;
    }

    void_t Manage_lvalue__IDENTIFIER() {
        fprintf(yyout, "lvalue -> IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_lvalue__LOCAL_IDENTIFIER() {
        fprintf(yyout, "lvalue -> local IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_lvalue__void_t_COLON_IDENTIFIER() {
        fprintf(yyout, "lvalue -> ::IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_lvalue__member() {
        fprintf(yyout, "lvalue -> member\n");
        return void_value;
    }


    void_t Manage_member__lvalue_DOT_IDENTIFIER() {
        fprintf(yyout, "member -> .IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET() {
        fprintf(yyout, "member -> lvalue[expr]\n");
        return void_value;
    }
    void_t Manage_member__call_DOT_IDENTIFIER() {
        fprintf(yyout, "member -> call.IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET() {
        fprintf(yyout, "member -> [expr]\n");
        return void_value;
    }

	/* Manage_call() */
	void_t Manage_call_call_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(){
		fprintf(yyout, " call -> call(elist)\n");
		return void_value;
	}
	void_t Manage_call_lvalue_callsuffix(){
		fprintf(yyout, "call -> lvalue callsuffix\n");
		return void_value;
	}
	void_t Manage_call_LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(){
		fprintf(yyout, "call -> (funcdef) (elist)\n");
		return void_value;
	}

	/* Manage_callsuffix() */

	void_t Manage_callsuffix_normcall(){
		fprintf(yyout, "callsuffix -> normcall\n");
		return void_value;
	}
	void_t Manage_callsuffix_methodcall(){
		fprintf(yyout, "callsuffix -> methodcall\n");
		return void_value;
	}

	/* Manage_normcall() */
	void_t Manage_normcall_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(){
		fprintf(yyout, "normcall -> (elist)\n");
		return void_value;
	}

	/* Manage_methodcall() */
	void_t Manage_methodcall_DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(){
		fprintf(yyout, "methodcall -> ..id(elist)\n");
		return void_value;
	}

	/* Manage_elist() */
	void_t Manage_tmp_elist_tmp_elist_COMMA_expr(){
		fprintf(yyout, "elist -> expr,...,expr\n");
		return void_value;
	}
	void_t Manage_tmp_elist_empty(){
		fprintf(yyout, "elist -> expr\n");
		return void_value;
	}
	void_t Manage_elist_empty(){
		fprintf(yyout, "elist -> empty\n");
		return void_value;
	}

	/* Manage_objectdef()*/
	void_t Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET(){
		fprintf(yyout, "objectdef -> [ elist ]\n");
		return void_value;
	}
	void_t Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET(){
		fprintf(yyout, "objectdef -> [ indexed ]\n");
		return void_value;
	}

	/* Manage_indexed */
	void_t Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem(){
		fprintf(yyout, "indexed -> indexedelem,...,indexedelem\n");
		return void_value;
	}
	void_t Manage_tmp_indexed_empty(){
		fprintf(yyout, "indexed -> indexedelem\n");
		return void_value;
	}
	void_t Manage_indexed_empty(){
		fprintf(yyout, "indexed -> empty\n");
		return void_value;
	}

	/* Manage_indexedelem() */
	void_t Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE(){
		fprintf(yyout, "indexedelem -> { expr : expr }\n");
		return void_value;
	}

	/* Manage_block() */
	void_t Manage_block_LEFT_BRACE_tmp_block_RIGHT_BRACE(){
		fprintf(yyout, "block -> { stmt }\n");
		return void_value;
	}

	/* Manage_funcdef() */
	void_t Manage_tmp_funcdef_empty(std::string& func_id){
		func_id = "_f" + func_num;
		func_num++; /*increase for future use*/
		return void_value;
	}

	void_t Manage_funcdef_FUNCTION_tmp_funcdef(std::string& func_id, unsigned int scope, int yylineno){
		std::vector<symbol_table::entry> v = symbol_table::recursive_lookup(func_id,scope);
		if (v.empty()){
			symbol_table::func_entry::func_entry(scope, yylineno, func_id, func_entry::sym_type::USER_FUNC,
				                                                  const vector<var_entry> &arg_list);
			/*INSERT HERE.. HOW? and with arg_list?*/
		}
		else{
			for (unsigned long i = 0; i < v.size(); i++){
				if (v[i].get_scope() == scope){ 
					fprintf(yyout, "Syntax Error --> line:%d , Function definition \t"
						           "with the same function or variable name in the same scope\n",yylineno); 
				}
				else if ((v[i].get_sym_type() == func_entry::sym_type::LIB_FUNC) &&
					     (v[i].get_scope() == 0)){
					fprintf(yyout, "Syntax Error --> line:%d , Function definition \t"
						"with the same library function name\n", yylineno);
				}
			}
		}
		return void_value;
	}

	void_t Manage_funcdef_FUNCTION_IDENTIFIER_LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS_block(){
		fprintf(yyout, "funcdef -> function id (idlist) block\n");
		return void_value;
	}

	/* Manage_const() */
	void_t Manage_const_CONST_INT(){
		fprintf(yyout, "const -> const int\n");
		return void_value;
	}
	void_t Manage_const_CONST_REAL(){
		fprintf(yyout, "const -> const real\n");
		return void_value;
	}
	void_t Manage_const_CONST_STR(){
		fprintf(yyout, "const -> const string\n");
		return void_value;
	}
	void_t Manage_const_NIL(){
		fprintf(yyout, "const -> nil\n");
		return void_value;
	}
	void_t Manage_const_BOOL_TRUE(){
		fprintf(yyout, "const -> true\n");
		return void_value;
	}
	void_t Manage_const_BOOL_FALSE(){
		fprintf(yyout, "const -> false\n");
		return void_value;
	}

	/* Manage_idlist() */
	void_t Manage_tmp_idlist_tmp_idlist_COMMA_IDENTIFIER(){
		fprintf(yyout, "idlist -> id,...,id\n");
		return void_value;
	}
	void_t Manage_tmp_idlist_empty(){
		fprintf(yyout, "idlist -> id\n");
		return void_value;
	}
    void_t Manage_idlist__IDENTIFIER_tmp_idlist() {
        fprintf(yyout, "idlist -> IDENTIFIER tmp_idlist\n");
        return void_value;
    }
	void_t Manage_idlist_empty(){
		fprintf(yyout, "idlist -> empty\n");
		return void_value;
	}

	/* Manage_ifstmt() */
	void_t Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt(){
		fprintf(yyout, "ifstmt -> if ( expr ) stmt\n");
		return void_value;
	}
	void_t Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt_ELSE_stmt(){
		fprintf(yyout, "ifstmt -> if ( expr ) stmt else stmt\n");
		return void_value;
	}

	/* Manage_whilestmt() */
	void_t Manage_WHILE_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt(){
		fprintf(yyout, "whilestmt -> while ( expr ) stmt\n");
		return void_value;
	}

	/* Manage_forstmt() */
	void_t Manage_FOR_LEFT_PARENTHESIS_elist_SEMICLON_expr_SEMICOLON_elist_RIGHT_PARENTHESIS_stmt(){
		fprintf(yyout, "forstmt -> for (elist;expr;elist) stmt\n");
		return void_value;
	}

	/* Manage_returnstmt() */
	void_t Manage_RETURN_SEMICOLON(){
		fprintf(yyout, "returnstmt -> return ;\n");
		return void_value;
	}
	void_t Manage_RETURN_expr_SEMICOLON(){
		fprintf(yyout, "returnstmt -> return expr ;\n");
		return void_value;
	}
}