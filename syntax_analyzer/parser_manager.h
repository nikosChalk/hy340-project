//
// Created by nikos on 3/19/18.
//

#ifndef HY340_PROJECT_PARSER_MANAGER_H
#define HY340_PROJECT_PARSER_MANAGER_H

#include "types.h"

namespace syntax_analyzer {
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
};


#endif //HY340_PROJECT_PARSER_MANAGER_H
