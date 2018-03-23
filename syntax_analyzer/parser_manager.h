//
// Created by nikos on 3/19/18.
//

#ifndef HY340_PROJECT_PARSER_MANAGER_H
#define HY340_PROJECT_PARSER_MANAGER_H

#include "types.h"

namespace syntax_analyzer {

	int func_num = 1; /*counter , set function id (unique)*/

    void_t Manage_program__stmt_program();
    void_t Manage_program();

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

    /*TODO: <type> Manage_expr__assignexpr(); */
    double Manage_expr__expr_PLUS_expr(double opLeft, double opRight);
    double Manage_expr__expr_MINUS_expr(double opLeft, double opRight);
    double Manage_expr__expr_MUL_expr(double opLeft, double opRight);
    double Manage_expr__expr_DIV_expr(double opLeft, double opRight);
    double Manage_expr__expr_MOD_expr(double opLeft, double opRight);
    double Manage_expr__expr_GT_expr(double opLeft, double opRight);
    double Manage_expr__expr_GE_expr(double opLeft, double opRight);
    double Manage_expr__expr_LT_expr(double opLeft, double opRight);
    double Manage_expr__expr_LE_expr(double opLeft, double opRight);
    double Manage_expr__expr_EQ_expr(double opLeft, double opRight);
    double Manage_expr__expr_NE_expr(double opLeft, double opRight);
    double Manage_expr__expr_AND_expr(double opLeft, double opRight);
    double Manage_expr__expr_OR_expr(double opLeft, double opRight);
    double Manage_expr__term(double term);

    void_t Manage_expr__assignexpr();
    void_t Manage_expr__expr_PLUS_expr();
    void_t Manage_expr__expr_MINUS_expr();
    void_t Manage_expr__expr_MUL_expr();
    void_t Manage_expr__expr_DIV_expr();
    void_t Manage_expr__expr_MOD_expr();
    void_t Manage_expr__expr_GT_expr();
    void_t Manage_expr__expr_GE_expr();
    void_t Manage_expr__expr_LT_expr();
    void_t Manage_expr__expr_LE_expr();
    void_t Manage_expr__expr_EQ_expr();
    void_t Manage_expr__expr_NE_expr();
    void_t Manage_expr__expr_AND_expr();
    void_t Manage_expr__expr_OR_expr();
    void_t Manage_expr__term(void_t term);

    void_t Manage_assignexpr__lvalue_ASSIGN_expr();

    void_t Manage_primary__lvalue();
    void_t Manage_primary__call();
    void_t Manage_primary__objectdef();
    void_t Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS();
    void_t Manage_primary__const();

    void_t Manage_lvalue__IDENTIFIER();
    void_t Manage_lvalue__LOCAL_IDENTIFIER();
    void_t Manage_lvalue__void_t_COLON_IDENTIFIER();
    void_t Manage_lvalue__member();

    void_t Manage_member__lvalue_DOT_IDENTIFIER();
    void_t Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET();
    void_t Manage_member__call_DOT_IDENTIFIER();
    void_t Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET();

	/* Manage_call() */
	void_t Manage_call_call_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();
	void_t Manage_call_lvalue_callsuffix();
	void_t Manage_call_LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

	/* Manage_callsuffix() */
	void_t Manage_callsuffix_normcall();
	void_t Manage_callsuffix_methodcall();

	/* Manage_normcall() */
	void_t Manage_normcall_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

	/* Manage_methodcall() */
	void_t Manage_methodcall_DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS();

	/* Manage_elist() */
	void_t Manage_tmp_elist_tmp_elist_COMMA_expr();
	void_t Manage_tmp_elist_empty();
	void_t Manage_elist_empty();

	/* Manage_objectdef()*/
	void_t Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET();
	void_t Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET();

	/* Manage_indexed() */
	void_t Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem();
	void_t Manage_tmp_indexed_empty();
	void_t Manage_indexed_empty();

	/* Manage_indexedelem() */
	void_t Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE();

	/* Manage_block() */
	void_t Manage_block_LEFT_BRACE_tmp_block_RIGHT_BRACE();

	/* Manage_funcdef() */
	void_t Manage_tmp_funcdef_empty(std::string&);
	void_t Manage_funcdef_FUNCTION_tmp_funcdef(std::string&,unsigned int);
	void_t Manage_funcdef_FUNCTION_IDENTIFIER_LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS_block();

	/* Manage_const() */
	void_t Manage_const_CONST_INT();
	void_t Manage_const_CONST_REAL();
	void_t Manage_const_CONST_STR();
	void_t Manage_const_NIL();
	void_t Manage_const_BOOL_TRUE();
	void_t Manage_const_BOOL_FALSE();

	/* Manage_idlist() */
	void_t Manage_tmp_idlist_tmp_idlist_COMMA_IDENTIFIER();
	void_t Manage_tmp_idlist_empty();
    void_t Manage_idlist__IDENTIFIER_tmp_idlist();
	void_t Manage_idlist_empty();

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