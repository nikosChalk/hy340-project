

#ifndef HY340_PROJECT_PARSER_MANAGER_H
#define HY340_PROJECT_PARSER_MANAGER_H

#include "types.h"
#include "../intermediate_code/types.h"
#include "symbol_table.h"

namespace syntax_analyzer {

	void_t Manage_program__stmts();

	/**
	 * Resets the hidden variable count
	 */
    void_t Manage_stmt__expr_SEMICOLON();
    void_t Manage_stmt__ifstmt();
    void_t Manage_stmt__whilestmt();
    void_t Manage_stmt__forstmt();
    void_t Manage_stmt__returnstmt();
    void_t Manage_stmt__BREAK_SEMICOLON();
    void_t Manage_stmt__CONTINUE_SEMICOLON();
    void_t Manage_stmt__block();
    void_t Manage_stmt__funcdef();
    void_t Manage_stmt__SEMICOLON();

    /* Manage_expr__assignexpr() */

	void_t Manage_expr__assignexpr();

	intermediate_code::expr* Manage_expr__expr_PLUS_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                         intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_MINUS_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                          intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_MUL_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                        intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_DIV_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                        intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_MOD_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                        intermediate_code::expr *arg2, unsigned int lineno);

	intermediate_code::expr* Manage_expr__expr_GT_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
													   intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_GE_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                       intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_LT_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                       intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_LE_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                       intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_EQ_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                       intermediate_code::expr *arg2, unsigned int lineno);

    intermediate_code::expr* Manage_expr__expr_NE_expr(symbol_table &sym_table, intermediate_code::expr *arg1,
                                                       intermediate_code::expr *arg2, unsigned int lineno);

    void_t Manage_expr__expr_AND_expr();
    void_t Manage_expr__expr_OR_expr();
    void_t Manage_expr__term();

	/* Manage_term() */
    intermediate_code::expr* Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS();
    intermediate_code::expr* Manage_term__MINUS_expr();
    intermediate_code::expr* Manage_term__NOT_expr();
    intermediate_code::expr* Manage_term__PLUS_PLUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);
    intermediate_code::expr* Manage_term__lvalue_PLUS_PLUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);
    intermediate_code::expr* Manage_term__MINUS_MINUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);
    intermediate_code::expr* Manage_term__lvalue_MINUS_MINUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);
    intermediate_code::expr* Manage_term__primary();

    intermediate_code::expr* Manage_assignexpr__lvalue_ASSIGN_expr(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);

    intermediate_code::expr* Manage_primary__lvalue();
    intermediate_code::expr* Manage_primary__call();
    intermediate_code::expr* Manage_primary__objectdef();
    intermediate_code::expr* Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS();
    intermediate_code::expr* Manage_primary__const();

    /**
     * Handles the identifier that appears within the grammar rule.
     * @throws syntax_error if no accessible entry was found for the given identifier
     * @return The identifier's symbol_table::entry::lvalue_type
     */
	intermediate_code::expr* Manage_lvalue__IDENTIFIER(symbol_table &sym_table, const std::string &identifier, unsigned int lineno);
	intermediate_code::expr* Manage_lvalue__LOCAL_IDENTIFIER(symbol_table &sym_table, const std::string &identifier, unsigned int lineno);
    /**
     * This rule NEVER inserts to the symbol table
     */
	intermediate_code::expr* Manage_lvalue__DOUBLE_COLON_IDENTIFIER(const symbol_table &sym_table, const std::string &identifier, unsigned int lineno);
    intermediate_code::expr* Manage_lvalue__member();

    intermediate_code::expr* Manage_member__lvalue_DOT_IDENTIFIER();
    intermediate_code::expr* Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET();
    intermediate_code::expr* Manage_member__call_DOT_IDENTIFIER();
    intermediate_code::expr* Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET();

	/* Manage_call() */
    intermediate_code::expr* Manage_call_call_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();
    intermediate_code::expr* Manage_call_lvalue_callsuffix();
    intermediate_code::expr* Manage_call_LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

	/* Manage_callsuffix() */
	void_t Manage_callsuffix_normcall();
	void_t Manage_callsuffix_methodcall();

	/* Manage_normcall() */
	void_t Manage_normcall_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

	/* Manage_methodcall() */
    void_t Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

    /* Manage_elist() */
	void_t Manage_tmp_elist_tmp_elist_COMMA_expr();
	void_t Manage_tmp_elist_empty();
	void_t Manage_elist_empty();
	void_t Manage_elist__expr_tmp_elist();

	/* Manage_objectdef()*/
    intermediate_code::expr* Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET();
    intermediate_code::expr* Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET();

	/* Manage_indexed() */
	void_t Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem();
	void_t Manage_tmp_indexed_empty();
    void_t Manage_indexed__indexedelem_tmp_indexed();

	/* Manage_indexedelem() */
	void_t Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE();

	/* Manage_block() */
    void_t Manage_stmts__stmts_stmt();
    void_t Manage_stmts__empty();
    /**
     * Increases scope
     */
    void_t Manage_block_open__LEFT_BRACE();
    /**
     * Decreases scope
     */
    void_t Manage_block_close__RIGHT_BRACE();

	void_t Manage_block__block_open_stmts_block_close(symbol_table &sym_table);

	/* Manage_funcdef() */
    std::string Manage_funcname__IDENTIFIER(const std::string &id);
    std::string Manage_funcname__empty();
    /**
     * This is needed because once we have identified "block" in the grammar rule, first the block's semantic will be called
     * and AFTERWARDS the "funcdef" semantic rule will be called. Calling a "block" semantic rule, hides symbols and changes scopes.
     *
     * Also enters into scope space: formal arguments and increases scope
     */
	symbol_table::func_entry* Manage_funcprefix__FUNCTION_funcname(symbol_table &sym_table, const std::string &id, unsigned int lineno);
	/**
	 * Exits scope space formal arguments, decreases scope and then enters to function scope space.
	 */
    void_t Manage_funcargs__LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS();
    /**
     * Exits function scope space
     */
    unsigned int Manage_funcbody__block();
    /**
     * @param func_entry The function's entry within the symbol table
     * @param total_func_locals The function's total local variables
     * @return The function's entry within the symbol table
     */
    symbol_table::func_entry* Manage_funcdef__funcprefix_funcargs_funcbody(symbol_table::func_entry *func_entry,
                                                                           unsigned int lineno, unsigned int total_func_locals);


	/* Manage_const() */
    intermediate_code::expr* Manage_const_CONST_INT();
    intermediate_code::expr* Manage_const_CONST_REAL();
    intermediate_code::expr* Manage_const_CONST_STR();
    intermediate_code::expr* Manage_const_NIL();
    intermediate_code::expr* Manage_const_BOOL_TRUE();
    intermediate_code::expr* Manage_const_BOOL_FALSE();

	/* Manage_idlist() */
    /**
     * "idlist" grammar rule appears only within grammar rule "funcargs"
     */
    std::vector<std::string> Manage_tmp_idlist__empty();
    std::vector<std::string> Manage_tmp_idlist__tmp_idlist_COMMA_IDENTIFIER(std::vector<std::string> &tmp_id_list, const std::string &identifier);
    std::vector<std::string> Manage_idlist__IDENTIFIER_tmp_idlist(symbol_table &sym_table, std::vector<std::string> tmp_id_list,
                                                                  std::string identifier, unsigned int lineno);
    std::vector<std::string> Manage_idlist__empty();

	/* Manage_ifstmt() */
	void_t Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();
	void_t Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt_ELSE_stmt();

	/* Manage_whilestmt() */
	void_t Manage_WHILE_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS_stmt();

	/* Manage_forstmt() */
	void_t Manage_FOR_LEFT_PARENTHESIS_elist_SEMICLON_expr_SEMICOLON_elist_RIGHT_PARENTHESIS_stmt();

	/* Manage_returnstmt() */
	void_t Manage_RETURN_SEMICOLON();
	void_t Manage_RETURN_expr_SEMICOLON();
};


#endif //HY340_PROJECT_PARSER_MANAGER_H
