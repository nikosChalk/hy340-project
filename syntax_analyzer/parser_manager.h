

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
    void_t Manage_stmt__expr_SEMICOLON(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *e);
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
    void_t Manage_returnstmt__RETURN_expr_SEMICOLON(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *expr);

    /**
     * @throws syntax_analyzer::syntax_error If "break;" was not inside a loop
     */
    void_t Manage_breakstmt__BREAK_SEMICOLON(unsigned int lineno);
    /**
     * @throws syntax_analyzer::syntax_error If "continue;" was not inside a loop
     */
    void_t Manage_continuestmt__CONTINUE_SEMICOLON(unsigned int lineno);

    /* Manage_expr__assignexpr() */
	intermediate_code::expr* Manage_expr__assignexpr(intermediate_code::expr *assignexpr);

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
	intermediate_code::expr* Manage_expr__expr_GT_expr(unsigned int lineno, intermediate_code::expr *leftOperand,
                                                       intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_GE_expr(unsigned int lineno, intermediate_code::expr *leftOperand,
                                                       intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_LT_expr(unsigned int lineno, intermediate_code::expr *leftOperand,
                                                       intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__expr_LE_expr(unsigned int lineno, intermediate_code::expr *leftOperand,
                                                       intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__eq_prefix_expr(symbol_table &sym_table, unsigned int lineno,
                                                         intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_expr__ne_prefix_expr(symbol_table &sym_table, unsigned int lineno,
                                                         intermediate_code::expr *leftOperand, intermediate_code::expr *rightOperand);

    intermediate_code::expr* Manage_eq_prefix__expr_EQ(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr* leftOperand);
    intermediate_code::expr* Manage_ne_prefix__expr_NE(symbol_table &sym_table, unsigned int lineno,
                                                       intermediate_code::expr* leftOperand);

    /* Manage_expr__expr_boolop_expr */
    intermediate_code::expr* Manage_expr__and_prefix_log_next_quad_expr(symbol_table &sym_table, unsigned int lineno,
                                                                        intermediate_code::expr *leftOperand,
                                                                        intermediate_code::expr *rightOperand, unsigned int right_op_first_quadno);

    intermediate_code::expr* Manage_expr__or_prefix_log_next_quad_expr(symbol_table &sym_table, unsigned int lineno,
                                                                       intermediate_code::expr *leftOperand,
                                                                       intermediate_code::expr *rightOperand, unsigned int right_op_first_quadno);

    intermediate_code::expr* Manage_expr__term(intermediate_code::expr *expr);

    intermediate_code::expr* Manage_and_prefix__expr_AND(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *left_operand);
    intermediate_code::expr* Manage_or_prefix__expr_OR(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *left_operand);

	/* Manage_term() */
	intermediate_code::expr* Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(intermediate_code::expr *expr);
	intermediate_code::expr* Manage_term__MINUS_expr        (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *uminus_expr);
	intermediate_code::expr* Manage_term__NOT_expr          (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *not_expr);
	intermediate_code::expr* Manage_term__PLUS_PLUS_lvalue  (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__lvalue_PLUS_PLUS  (symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__MINUS_MINUS_lvalue(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr* lvalue);
	intermediate_code::expr* Manage_term__lvalue_MINUS_MINUS(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
	intermediate_code::expr* Manage_term__primary           (intermediate_code::expr *primary);

    intermediate_code::expr* Manage_assignexpr__lvalue_ASSIGN_expr(symbol_table &sym_table, unsigned int lineno,
                                                                   intermediate_code::expr *lvalue, intermediate_code::expr *right_expr);

    intermediate_code::expr* Manage_primary__lvalue(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *lvalue);
    intermediate_code::expr* Manage_primary__call(intermediate_code::expr *call);
    intermediate_code::expr* Manage_primary__objectdef(intermediate_code::expr *objectdef);
	intermediate_code::expr* Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS(symbol_table::func_entry *funcdef);
    intermediate_code::expr* Manage_primary__const(intermediate_code::expr *const_expr);

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
    intermediate_code::expr* Manage_lvalue__member(intermediate_code::expr *member);

    intermediate_code::expr* Manage_member__lvalue_DOT_IDENTIFIER(symbol_table &sym_table, unsigned int lineno,
                                                                  intermediate_code::expr *lvalue, const std::string &id);

    intermediate_code::expr* Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET(symbol_table &sym_table, unsigned int lineno,
                                                                                   intermediate_code::expr *lvalue, intermediate_code::expr *expr);
	intermediate_code::expr* Manage_member__call_DOT_IDENTIFIER(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *call, const std::string &id);
	intermediate_code::expr* Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr* call, intermediate_code::expr* expr);

	/* Manage_call() */
    intermediate_code::expr* Manage_call__call_normcall(symbol_table &sym_table, unsigned int lineno,
                                                        intermediate_code::expr *call, intermediate_code::norm_call *ncall);

    intermediate_code::expr* Manage_call__lvalue_callsuffix(symbol_table &sym_table, unsigned int lineno,
                                                            intermediate_code::expr *lvalue, intermediate_code::call_suffix *csuffix);

    intermediate_code::expr* Manage_call__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_normcall(symbol_table &sym_table, unsigned int lineno,
                                                                                             symbol_table::func_entry *funcdef,
                                                                                             intermediate_code::norm_call *ncall);

	/* Manage_callsuffix() */
	intermediate_code::call_suffix* Manage_callsuffix__normcall(intermediate_code::norm_call* norm_call);
	intermediate_code::call_suffix* Manage_callsuffix__methodcall(intermediate_code::method_call* method_call);

	/* Manage_normcall() */
	intermediate_code::norm_call* Manage_normcall__LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(std::deque<intermediate_code::expr*> const &elist);

	/* Manage_methodcall() */
    intermediate_code::method_call* Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(const std::string &id,
																													  std::deque<intermediate_code::expr*> const &elist);

    /* Manage_elist() */
	std::deque<intermediate_code::expr*> Manage_tmp_elist__tmp_elist_COMMA_expr(symbol_table &sym_table, unsigned int lineno,
																				std::deque<intermediate_code::expr*> const &tmp_elist,
                                                                                intermediate_code::expr *expr);
	std::deque<intermediate_code::expr*> Manage_tmp_elist__empty();
	std::deque<intermediate_code::expr*> Manage_elist__empty();
	std::deque<intermediate_code::expr*> Manage_elist__expr_tmp_elist(symbol_table &sym_table, unsigned int lineno,
                                                                      intermediate_code::expr *expr,
                                                                      std::deque<intermediate_code::expr*> const &tmp_elist);

	/* Manage_objectdef()*/
    intermediate_code::expr* Manage_objectdef__LEFT_BRACKET_elist_RIGHT_BRACKET(symbol_table &sym_table, unsigned int lineno,
                                                                                std::deque<intermediate_code::expr*> const &elist);

    intermediate_code::expr* Manage_objectdef__LEFT_BRACKET_indexed_RIGHT_BRACKET(symbol_table &sym_table, unsigned int lineno,
                                                                                  std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>>const &indexed);

	/* Manage_indexed() */
	std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>> Manage_tmp_indexed__tmp_indexed_COMMA_indexedelem(std::deque<std::pair<intermediate_code::expr*,intermediate_code::expr*>> const &tmp_indexed,
                                                                                                                                std::pair<intermediate_code::expr*, intermediate_code::expr*> const &indexedelem);
    std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>> Manage_tmp_indexed__empty();
    std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>> Manage_indexed__indexedelem_tmp_indexed(std::pair<intermediate_code::expr*, intermediate_code::expr*> const &indexedelem,
                                                                                                                      std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>> const &tmp_indexed);

	/* Manage_indexedelem() */
    std::pair<intermediate_code::expr*, intermediate_code::expr*> Manage_indexedelem(symbol_table &sym_table, unsigned int lineno,
                                                                                     intermediate_code::expr *left_expr,
                                                                                     intermediate_code::expr *right_expr);

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
    intermediate_code::expr* Manage_const__CONST_INT(long value);
    intermediate_code::expr* Manage_const__CONST_REAL(long double value);
    intermediate_code::expr* Manage_const__CONST_STR(const std::string &str);
    intermediate_code::expr* Manage_const__NIL();
    intermediate_code::expr* Manage_const__BOOL_TRUE();
    intermediate_code::expr* Manage_const__BOOL_FALSE();

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
	unsigned int Manage_ifprefix__IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *expr);
	unsigned int Manage_elseprefix__ELSE(unsigned int lineno);
	/**
	 * @param ifprefix The quadno of the incomplete jump that the "ifprefix -> (expr)" generated. The jump skips the "if" code
	 */
	void_t Manage_ifstmt__ifprefix_stmt(unsigned int ifprefix);
	void_t Manage_ifstmt__ifprefix_stmt_elseprefix_stmt(unsigned int ifprefix, unsigned int elseprefix);


	/* Manage_whilestmt() */
	unsigned int Manage_whilecond__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(symbol_table &sym_table, unsigned int lineno, intermediate_code::expr *expr);
	/**
	 * @param first_quadno First quad number of the whilecond's "expr"
	 * @param whilecond The quadno of the in-complete jump what whilecond produced. This jump is patched to jump out of the while loop.
	 */
	void_t Manage_whilestmt__WHILE_log_next_quad_whilecond_stmt(unsigned int first_quadno, unsigned int whilecond, unsigned int lineno);


	/* Manage_forstmt() */
	/**
	 * @param cond_first_quad The quad number of the first quad of the for's condition "expr"
	 * @param cond_expr The condition's expr
	 */
	intermediate_code::for_prefix* Manage_forprefix(symbol_table &sym_table, unsigned int lineno,
                                                    unsigned int cond_first_quad, intermediate_code::expr *cond_expr);
	/**
	 * @param incomplete_jmp_to_exit The quadno of the already emitted incomplete jump which jumps out of the loop, i.e. the rest code
	 * @param incomplete_jmp_to_cond The quadno of the already emitted incomplete jump which jumps to the first quad of the condition
	 * @param incomplete_jmp_to_incr_expr The quadno of the already emitted incomplete jump which jumps to the first quad of the elist
	 * that executes at the end of every successful loop
	 */
	void_t Manage_forstmt(intermediate_code::for_prefix *forprefix, unsigned int incomplete_jmp_to_exit,
                          unsigned int incomplete_jmp_to_cond, unsigned int incomplete_jmp_to_incr_expr);

	/* Manage miscellaneous */
	/**
	 * @return The quadno of the next quad which will be emitted.
	 */
	unsigned int Manage_log_next_quad__empty();
	/**
	 * @return The quadno of the incomplete jump
	 */
    unsigned int Manage_emit_incomplete_jmp__empty(unsigned int lineno);
};


#endif //HY340_PROJECT_PARSER_MANAGER_H
