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

};



#endif //HY340_PROJECT_PARSER_MANAGER_H
