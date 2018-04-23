

#include "parser_manager.h"
#include "symbol_table.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <typeinfo>
#include "syntax_error.h"
#include "scope_handler.h"

using namespace std;

extern FILE *yyout;

namespace syntax_analyzer {

    static scope_handler scope_handler; /*Default constructor called*/

    void_t Manage_program__stmt_program() {
		fprintf(yyout, "program -> program stmt\n");
        return void_value;
    }
    void_t Manage_program() {
		fprintf(yyout, "program -> empty\n");
        return void_value;
    }


    void_t Manage_stmt__expr_SEMICOLON() {
        fprintf(yyout, "stmt -> expr ;\n");
        return void_value;
    }
    void_t Manage_stmt__ifstmt() {
		fprintf(yyout, "stmt -> ifstmt\n");
        return void_value;
    }
    void_t Manage_stmt__whilestmt() {
        fprintf(yyout, "stmt -> whilestmt\n");
        return void_value;
    }
    void_t Manage_stmt__forstmt() {
        fprintf(yyout, "stmt -> forstmt\n");
        return void_value;
    }
    void_t Manage_stmt__returnstmt() {
        fprintf(yyout, "stmt -> returnstmt\n");
        return void_value;
    }
    void_t Manage_stmt__BREAK_SEMICOLON() {
        fprintf(yyout, "stmt -> break ;\n");
        return void_value;
    }
    void_t Manage_stmt__CONTINUE_SEMICOLON() {
        fprintf(yyout, "stmt -> continue ;\n");
        return void_value;
    }
    void_t Manage_stmt__block() {
        fprintf(yyout, "stmt -> block\n");
        return void_value;
    }
    void_t Manage_stmt__funcdef() {
        fprintf(yyout, "stmt -> funcdef\n");
        return void_value;
    }
    void_t Manage_stmt__SEMICOLON() {
        fprintf(yyout, "stmt -> ;\n");
        return void_value;
    }


    void_t Manage_expr__assignexpr() {
        fprintf(yyout, "expr -> assignexpr\n");
        return void_value;
    }
    void_t Manage_expr__expr_PLUS_expr() {
        fprintf(yyout, "expr -> expr + expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_MINUS_expr() {
        fprintf(yyout, "expr -> expr - expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_MUL_expr() {
        fprintf(yyout, "expr -> expr * expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_DIV_expr() {
        fprintf(yyout, "expr -> expr / expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_MOD_expr() {
        fprintf(yyout, "expr -> expr %% expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_GT_expr() {
        fprintf(yyout, "expr -> expr > expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_GE_expr() {
        fprintf(yyout, "expr -> expr >= expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_LT_expr() {
        fprintf(yyout, "expr -> expr < expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_LE_expr() {
        fprintf(yyout, "expr -> expr <= expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_EQ_expr() {
        fprintf(yyout, "expr -> expr == expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_NE_expr() {
        fprintf(yyout, "expr -> expr != expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_AND_expr() {
        fprintf(yyout, "expr -> expr AND expr\n");
        return void_value;
    }
    void_t Manage_expr__expr_OR_expr() {
        fprintf(yyout, "expr -> expr OR expr\n");
        return void_value;
    }
    void_t Manage_expr__term() {
        fprintf(yyout, "expr -> term\n");
        return void_value;
    }

	void_t Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(){
		fprintf(yyout, "term -> (expr)\n");
        return void_value;
	}
	void_t Manage_term__MINUS_expr(){
		fprintf(yyout, "term -> -expr\n");
        return void_value;
	}
	void_t Manage_term__NOT_expr(){
		fprintf(yyout, "term -> NOT expr\n");
        return void_value;
	}
	void_t Manage_term__PLUS_PLUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> ++lvalue\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__lvalue_PLUS_PLUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term ->lvalue ++\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__MINUS_MINUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> --lvalue\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__lvalue_MINUS_MINUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> lvalue--\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__primary(){
		fprintf(yyout, "term -> primary\n");
        return void_value;
	}
	
    void_t Manage_assignexpr__lvalue_ASSIGN_expr(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno) {
        fprintf(yyout, "assignexpr -> lvalue = expr\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
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

    symbol_table::entry::lvalue_type Manage_lvalue__IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> IDENTIFIER\n");

        unsigned int scope = scope_handler.get_current_scope();
        unsigned int active_function_scope = scope_handler.get_active_function_scope();
        vector<symbol_table::entry*> all_entries = sym_table.recursive_lookup(identifier, scope);

        if (all_entries.empty()) { /* This identifier was not found in any enclosing scope. ==> Insert new symbol */
            sym_table.insert(new symbol_table::var_entry(
                    scope, lineno, identifier, symbol_table::entry::LOCAL
            ));
            return symbol_table::entry::lvalue_type::VAR;
        } else {
            /* We need to check if there is an accessible reference in all_entries */
            try {
                return sym_table.exists_accessible_symbol(identifier, scope, active_function_scope)->get_lvalue_type();
            } catch(runtime_error &err) {
                throw syntax_error("Variable \'" + identifier + "\': " + err.what(), lineno);
            }
        }
    }
    symbol_table::entry::lvalue_type Manage_lvalue__LOCAL_IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> local IDENTIFIER\n");

        unsigned int scope = scope_handler.get_current_scope();
        vector<symbol_table::entry*> scope_entries = sym_table.lookup(identifier, scope);
        vector<symbol_table::entry*> global_entries = sym_table.lookup(identifier, symbol_table::entry::GLOBAL_SCOPE);
        symbol_table::entry::lvalue_type ret_val;

        if(scope_entries.empty()) {
            //Check if it shadows a library function
            bool lib_func_clash = false;
            for(const auto &global_entry: global_entries)
                if(global_entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
                    lib_func_clash = true;
            if(lib_func_clash)
                throw syntax_error("Local variable \'" + identifier + "\' name shadows library function", lineno);

            //Identifier is okay. Add it to the symbol table
            sym_table.insert(new symbol_table::var_entry(
               scope, lineno, identifier, symbol_table::entry::LOCAL
            ));
            ret_val = symbol_table::entry::lvalue_type::VAR;
        } else {
            assert(scope_entries.size() == 1);
            ret_val = scope_entries.at(0)->get_lvalue_type();
        }
        return ret_val;
    }
    symbol_table::entry::lvalue_type Manage_lvalue__DOUBLE_COLON_IDENTIFIER(const symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> ::IDENTIFIER\n");

        /* This rule NEVER inserts to the symbol table */
        vector<symbol_table::entry*> global_entries = sym_table.lookup(identifier, symbol_table::entry::GLOBAL_SCOPE);
        if(global_entries.empty())
            throw syntax_error("No global symbol \'" + identifier + "\' found.", lineno);

        assert(global_entries.size() == 1);
        return global_entries.at(0)->get_lvalue_type();
    }
    symbol_table::entry::lvalue_type Manage_lvalue__member() {
        fprintf(yyout, "lvalue -> member\n");
        return symbol_table::entry::lvalue_type::VAR;
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
	void_t Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS() {
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
    void_t Manage_elist__expr_tmp_elist() {
        fprintf(yyout, "elist -> expr tmp_elist\n");
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
    void_t Manage_indexed__indexedelem_tmp_indexed() {
        fprintf(yyout, "indexed -> indexelem tmp_indexed\n");
        return void_value;
    }

	/* Manage_indexedelem() */
	void_t Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE(){
		fprintf(yyout, "indexedelem -> { expr : expr }\n");
		return void_value;
	}

	/* Manage_block() */
    void_t Manage_stmts__stmts_stmt() {
        fprintf(yyout, "stmts -> stmts stmt\n");
        return void_value;
    }
    void_t Manage_stmts__empty() {
        fprintf(yyout, "stmts -> <empty>\n");
        return void_value;
    }
    void_t Manage_block_open__LEFT_BRACE() {
        fprintf(yyout, "block_open -> {\n");
        scope_handler.increase_scope();
        return void_value;
    }
    void_t Manage_block_close__RIGHT_BRACE() {
        fprintf(yyout, "block_close -> }\n");
        scope_handler.decrease_scope();
        return void_value;
    }
	void_t Manage_block__block_open_stmts_block_close(symbol_table &sym_table) {
		sym_table.hide(scope_handler.get_current_scope()+1);    //+1 because the block closed and thus we exited that scope
		fprintf(yyout, "block -> { stmts }\n");
		return void_value;
	}

	/* Manage_funcdef() */
    string Manage_funcname__IDENTIFIER(const string &id) {
        /* Lookup operation is done by Manage_funcdef__FUNCTION_tmp_funcdef */
        fprintf(yyout, "funcname -> IDENTIFIER\n");
        return id;
    }
    string Manage_funcname__empty() {
        fprintf(yyout, "funcname -> <empty>\n");
        return string();    /* Empty string */
    }

    void_t Manage_funcprefix__FUNCTION_funcname(symbol_table &sym_table, const std::string &id, unsigned int lineno) {
        fprintf(yyout, "funcprefix -> FUNCTION funcname\n");

        unsigned int scope = scope_handler.get_current_scope();
        vector<symbol_table::entry*> cur_scope_entries = sym_table.lookup(id, scope);  /* Look up only at the current scope */
        vector<symbol_table::entry*> global_entries = sym_table.lookup(id, symbol_table::entry::GLOBAL_SCOPE);  /* Look up only at the global scope */

        //Sanity checks
        if(!cur_scope_entries.empty())
            throw syntax_error("Function definition \'" + id
                               + "\' name clashes with already defined function or variable name in the same scope", lineno);

        for(const auto &entry : global_entries)
            if(entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
                throw syntax_error("Function definition \'" + id + "\' name shadows library function", lineno);

        //Insert in the symbol table
        sym_table.insert(new symbol_table::func_entry(
           scope, lineno, id, symbol_table::entry::USER_FUNC
        ));

        scope_handler.enter_formal_arg_ss();
        scope_handler.increase_scope();
		return void_value;
	}
    void_t Manage_funcargs__LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS() {
        fprintf(yyout, "funcars -> ( idlist )\n");
        scope_handler.exit_formal_arg_ss();
        scope_handler.decrease_scope();

        scope_handler.enter_function_ss();
        //"block" grammar rule will increase scope again
        return void_value;
    };

    void_t Manage_funcbody__block() {
        fprintf(yyout, "funcbody -> block\n");
        scope_handler.exit_function_ss();
        //scope already reduced by "block" grammar rule
        return void_value;
    }
    void_t Manage_funcdef__funcprefix_funcargs_funcbody() {
        fprintf(yyout, "funcdef -> FUNCTION IDENTIFIER (idlist) block\n");
        return void_value;
    }

	/* Manage_const() */
	void_t Manage_const_CONST_INT(){
		fprintf(yyout, "const -> CONST_INT\n");
		return void_value;
	}
	void_t Manage_const_CONST_REAL(){
		fprintf(yyout, "const -> CONST_REAL\n");
		return void_value;
	}
	void_t Manage_const_CONST_STR(){
		fprintf(yyout, "const -> CONST_STRING\n");
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
    vector<string> Manage_idlist__IDENTIFIER_tmp_idlist(symbol_table &sym_table, vector<string> tmp_id_list, string identifier, unsigned int lineno) {
        //The gammer rule idlist-> (zero, one, or more "id") is used only by the grammar rule funcdef

        fprintf(yyout, "idlist -> IDENTIFIER tmp_idlist\n");
        unsigned int scope = scope_handler.get_current_scope();

        tmp_id_list.push_back(identifier);
        for(const auto &cur_id : tmp_id_list) {
            vector<symbol_table::entry*> cur_scope_entries = sym_table.lookup(cur_id, scope);  /* Look up only at the current scope */
            vector<symbol_table::entry*> global_entries = sym_table.lookup(cur_id, symbol_table::entry::GLOBAL_SCOPE);  /* Look up only at the global scope */

            if(cur_scope_entries.empty()) {
                sym_table.insert(new symbol_table::var_entry(
                    scope, lineno, cur_id, symbol_table::entry::FORMAL_ARG
                ));
            } else {
                throw syntax_error("Formal Argument \'" + cur_id + "\' defined multiple times", lineno);
            }

            for(const auto &entry : global_entries)
                if(entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
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