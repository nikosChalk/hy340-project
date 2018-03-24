//
// Created by nikos on 3/20/18.
//

#include "parser_manager.h"
#include "symbol_table.h"
#include <vector>
#include <iostream>
#include "../not_implemented_error.h"
#include "syntax_error.h"

using namespace std;

extern FILE *yyout;

namespace syntax_analyzer {

    /**
     * Handles an identifier when it appears within a grammar rule. For example:
     * lvalue -> id
     * member -> lvalue.id
     * member -> call.id
     * etc.
     * But not for ::id or local id.
     */
    static void handle_identifier(symbol_table &sym_table, const string &identifier, unsigned int scope, unsigned int lineno) {
        vector<symbol_table::entry> all_entries = sym_table.recursive_lookup(identifier, scope);

        if (all_entries.empty()) { /* This identifier was not found in any enclosing scope. ==> Insert new symbol */
            sym_table.insert(symbol_table::var_entry(
                    scope, lineno, identifier, symbol_table::entry::LOCAL
            ));
        } else {
            /* We need to check if there is an accessible reference in all_entries */
            if (!sym_table.is_var_accessible(identifier, scope))
                throw syntax_error("Variable \'" + identifier + "\' does not refer to any accessible symbol.", lineno);
        }
    }

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

    void_t Manage_lvalue__IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int scope, unsigned int lineno) {
        fprintf(yyout, "lvalue -> IDENTIFIER\n");
        handle_identifier(sym_table, identifier, scope, lineno);
        return void_value;
    }
    void_t Manage_lvalue__LOCAL_IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int scope, unsigned int lineno) {
        fprintf(yyout, "lvalue -> local IDENTIFIER\n");
        vector<symbol_table::entry> scope_entries = sym_table.lookup(identifier, scope);
        vector<symbol_table::entry> global_entries = sym_table.lookup(identifier, symbol_table::entry::GLOBAL_SCOPE);

        if(scope_entries.empty()) {
            //Check if it shadows a library function
            bool lib_func_clash = false;
            for(const auto &global_entry: global_entries)
                if(global_entry.get_sym_type() == symbol_table::entry::LIB_FUNC)
                    lib_func_clash = true;
            if(lib_func_clash)
                throw syntax_error("Local variable \'" + identifier + "\' name shadows library function", lineno);

            //Identifier is okay. Add it to the symbol table
            sym_table.insert(symbol_table::var_entry(
               scope, lineno, identifier, symbol_table::entry::LOCAL
            ));
        }
        return void_value;
    }
    void_t Manage_lvalue__DOUBLE_COLON_IDENTIFIER(const symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> ::IDENTIFIER\n");

        /* This rule NEVER inserts to the symbol table */
        vector<symbol_table::entry> global_entries = sym_table.lookup(identifier, symbol_table::entry::GLOBAL_SCOPE);
        if(!global_entries.empty())
            throw syntax_error("No global symbol \'" + identifier + "\' found.", lineno);

        return void_value;
    }
    void_t Manage_lvalue__member() {
        fprintf(yyout, "lvalue -> member\n");
        return void_value;
    }


    void_t Manage_member__lvalue_DOT_IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int scope, unsigned int lineno) {
        fprintf(yyout, "member -> .IDENTIFIER\n");
        handle_identifier(sym_table, identifier, scope, lineno);
        return void_value;
    }
    void_t Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET() {
        fprintf(yyout, "member -> lvalue[expr]\n");
        return void_value;
    }
    void_t Manage_member__call_DOT_IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int scope, unsigned int lineno) {
        fprintf(yyout, "member -> call.IDENTIFIER\n");
        handle_identifier(sym_table, identifier, scope, lineno);
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
	void_t Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(symbol_table &sym_table,
                                                                                             const string &identifier,
                                                                                             unsigned int scope, unsigned int lineno) {
		fprintf(yyout, "methodcall -> ..id(elist)\n");
        handle_identifier(sym_table, identifier, scope, lineno);
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
    void_t Manage_tmp_block__tmp_block_stmt() {
        fprintf(yyout, "tmp_block -> tmp_block stmt\n");
        return void_value;
    }
    void_t Manage_tmp_block__empty() {
        fprintf(yyout, "tmp_block -> <empty>\n");
        return void_value;
    }
	void_t Manage_block__LEFT_BRACE_tmp_block_RIGHT_BRACE(symbol_table &sym_table, unsigned int scope){
		sym_table.hide(scope);
		fprintf(yyout, "block -> { stmt }\n");
		return void_value;
	}

	/* Manage_funcdef() */
    string Manage_tmp_funcdef__IDENTIFIER(const string &id) {
        fprintf(yyout, "tmp_funcdef -> IDENTIFIER\n");
        return id;
    }
    string Manage_tmp_funcdef__empty() {
        fprintf(yyout, "tmp_funcdef -> <empty>\n");
        return string();    /* Empty string */
    }

    void_t Manage_funcdef__FUNCTION_tmp_funcdef(symbol_table &sym_table, const std::string &id, unsigned int scope, unsigned int lineno) {
        vector<symbol_table::entry> cur_scope_entries = sym_table.lookup(id, scope);  /* Look up only at the current scope */
        vector<symbol_table::entry> global_entries = sym_table.lookup(id, symbol_table::entry::GLOBAL_SCOPE);  /* Look up only at the global scope */

        //Sanity checks
        if(!cur_scope_entries.empty())
            throw syntax_error("Function definition \'" + id
                               + "\' name clashes with already defined function or variable name in the same scope", lineno);
        if(!global_entries.empty())
            throw syntax_error("Function definition \'" + id + "\' name shadows library function", lineno);

        //Insert in the symbol table
        sym_table.insert(symbol_table::func_entry(
           scope, lineno, id, symbol_table::entry::USER_FUNC
        ));

		return void_value;
	}
    void_t Manage_funcdef__FUNCTION_IDENTIFIER_LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS_block() {
        fprintf(yyout, "funcdef -> FUNCTION IDENTIFIER (idlist) block\n");
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
    vector<string> Manage_tmp_idlist__empty() {
        fprintf(yyout, "tmp_idlist -> <empty>\n");
        return vector<string>();
    }
    vector<string> Manage_tmp_idlist__tmp_idlist_COMMA_IDENTIFIER(vector<string> &tmp_id_list, const string &identifier) {
        /* Gather the IDs. We postpone the processing until we have gathered all the IDs from the idlsit */
        tmp_id_list.push_back(identifier);
		fprintf(yyout, "tmp_idlist -> tmp_idlist , IDENTIFIER\n");
        return tmp_id_list;
	}
    vector<string> Manage_idlist__IDENTIFIER_tmp_idlist(symbol_table &sym_table, vector<string> tmp_id_list, string identifier,
                                                unsigned int scope, unsigned int lineno) {
        fprintf(yyout, "idlist -> IDENTIFIER tmp_idlist\n");
        tmp_id_list.push_back(identifier);
        for(const auto &cur_id : tmp_id_list) {
            vector<symbol_table::entry> cur_scope_entries = sym_table.lookup(cur_id, scope);  /* Look up only at the current scope */
            vector<symbol_table::entry> global_entries = sym_table.lookup(cur_id, symbol_table::entry::GLOBAL_SCOPE);  /* Look up only at the global scope */

            if(cur_scope_entries.empty()) {
                sym_table.insert(symbol_table::var_entry(
                    scope, lineno, cur_id, symbol_table::entry::FORMAL_ARG
                ));
            } else {
                throw syntax_error("Formal Argument \'" + cur_id + "\' defined multiple times", lineno);
            }

            if(!global_entries.empty())
                throw syntax_error("Formal Argument \'" + cur_id + "\' shadows library function", lineno);
        }
        return tmp_id_list;
    }
    vector<string> Manage_idlist__empty(){
		fprintf(yyout, "idlist -> <empty>\n");
		return vector<string>();
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