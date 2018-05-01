

#ifndef HY340_PROJECT_PARSER_MANAGER_H
#define HY340_PROJECT_PARSER_MANAGER_H

#include <deque>
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
    void_t Manage_stmt__break();
    void_t Manage_stmt__continue();
    void_t Manage_stmt__block();
    void_t Manage_stmt__funcdef();
    void_t Manage_stmt__SEMICOLON();

    /* Manage_returnstmt() */
    void_t Manage_returnstmt__RETURN_SEMICOLON(unsigned int lineno);
    void_t Manage_returnstmt__RETURN_expr_SEMICOLON(intermediate_code::expr *expr, unsigned int lineno);

    /**
     * @throws syntax_analyzer::syntax_error If "break;" was not inside a loop
     */
    void_t Manage_breakstmt__BREAK_SEMICOLON(unsigned int lineno);
    /**
     * @throws syntax_analyzer::syntax_error If "continue;" was not inside a loop
     */
    void_t Manage_continuestmt__CONTINUE_SEMICOLON(unsigned int lineno);

    /* Manage_expr__assignexpr() */

	void_t Manage_expr__assignexpr();

	/* Manage_expr__expr_arithmop_expr */
	intermediate_code::expr* Manage_expr__expr_PLUS_expr(symbol_table &sym_table, unsigned int lineno,
                                                         intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_MINUS_expr(symbol_table &sym_table, unsigned int lineno,
                                                          intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_MUL_expr(symbol_table &sym_table, unsigned int lineno,
                                                        intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_DIV_expr(symbol_table &sym_table, unsigned int lineno,
                                                        intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_MOD_expr(symbol_table &sym_table, unsigned int lineno,
                                                        intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    /* Manage_expr__expr_relop_expr */
	intermediate_code::expr* Manage_expr__expr_GT_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_GE_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_LT_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_LE_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_EQ_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_NE_expr(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    /* Manage_expr__expr_boolop_expr */
    intermediate_code::expr* Manage_expr__expr_AND_expr();
    intermediate_code::expr* Manage_expr__expr_OR_expr();
    intermediate_code::expr* Manage_expr__term();

	/* Manage_term() */
	intermediate_code::expr* Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(intermediate_code::expr *expr);
	intermediate_code::expr* Manage_term__MINUS_expr        (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *uminus_expr);
	intermediate_code::expr* Manage_term__NOT_expr          (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *not_expr);
	intermediate_code::expr* Manage_term__PLUS_PLUS_lvalue  (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__lvalue_PLUS_PLUS  (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__MINUS_MINUS_lvalue(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr* lvalue);
	intermediate_code::expr* Manage_term__lvalue_MINUS_MINUS(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__primary           (intermediate_code::expr *primary);

    intermediate_code::expr* Manage_assignexpr__lvalue_ASSIGN_expr(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno);

    intermediate_code::expr* Manage_primary__lvalue();
    intermediate_code::expr* Manage_primary__call();
    intermediate_code::expr* Manage_primary__objectdef();
	intermediate_code::expr* Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS(symbol_table::func_entry *funcdef);
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
    intermediate_code::expr* Manage_call__call_normcall(symbol_table &sym_table, unsigned int lineno,
                                                        intermediate_code::expr *call, intermediate_code::norm_call *norm_call);

    intermediate_code::expr* Manage_call__lvalue_callsuffix(symbol_table &sym_table, unsigned int lineno,
                                                            intermediate_code::expr *lvalue, intermediate_code::call_suffix *call_suffix);

    intermediate_code::expr* Manage_call__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_normcall(symbol_table &sym_table, unsigned int lineno,
                                                                                             symbol_table::func_entry *funcdef,
                                                                                             intermediate_code::norm_call *norm_call);

	/* Manage_callsuffix() */
	intermediate_code::call_suffix* Manage_callsuffix__normcall(intermediate_code::norm_call* norm_call);
	intermediate_code::call_suffix* Manage_callsuffix__methodcall(intermediate_code::method_call* method_call);

	/* Manage_normcall() */
	intermediate_code::norm_call* Manage_normcall__LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(std::deque<intermediate_code::expr*> const &elist);

	/* Manage_methodcall() */
    intermediate_code::method_call* Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(const std::string &id,
																													  std::deque<intermediate_code::expr*> const &elist);

    /* Manage_elist() */
	std::deque<intermediate_code::expr*> Manage_tmp_elist__tmp_elist_COMMA_expr(std::deque<intermediate_code::expr*> const &tmp_elist,
                                                                                intermediate_code::expr *expr);
	std::deque<intermediate_code::expr*> Manage_tmp_elist__empty();
	std::deque<intermediate_code::expr*> Manage_elist__empty();
	std::deque<intermediate_code::expr*> Manage_elist__expr_tmp_elist(intermediate_code::expr *expr,
                                                                      std::deque<intermediate_code::expr*> const &tmp_elist);

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
	unsigned int Manage_ifprefix__IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(intermediate_code::expr *expr, unsigned int lineno);
	unsigned int Manage_elseprefix__ELSE(unsigned int lineno);
	/**
	 * @param ifprefix The quadno of the incomplete jump that the "ifprefix -> (expr)" generated. The jump skips the "if" code
	 */
	void_t Manage_ifstmt__ifprefix_stmt(unsigned int ifprefix);
	void_t Manage_ifstmt__ifprefix_stmt_elseprefix_stmt(unsigned int ifprefix, unsigned int elseprefix);


	/* Manage_whilestmt() */
	unsigned int Manage_whilecond__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(intermediate_code::expr *expr, unsigned int lineno);
	/**
	 * @param first_quadno First quad number of the whilecond's "expr"
	 * @param whilecond The quadno of the in-complete jump what whilecond produced. This jump is patched to jump out of the while loop.
	 */
	void_t Manage_whilestmt__WHILE_log_next_quad_whilecond_stmt(unsigned int first_quadno, unsigned int whilecond, unsigned int lineno);


	/* Manage_forstmt() */
	unsigned int Manage_N(unsigned int lineno);
	unsigned int Manage_M();
	intermediate_code::for_prefix* Manage_forprefix(unsigned int m, intermediate_code::expr* expr, unsigned int lineno);
	void_t Manage_forprefix_N_elist_RIGHT_PARENTHESIS_N_stmt_N(intermediate_code::for_prefix* forprefix , unsigned int n1,unsigned int n2 ,unsigned int n3);


	/* Manage miscellaneous */
	/**
	 * @return The quadno of the next quad which will be emitted.
	 */
	unsigned int Manage_log_next_quad__empty();
};


#endif //HY340_PROJECT_PARSER_MANAGER_H
