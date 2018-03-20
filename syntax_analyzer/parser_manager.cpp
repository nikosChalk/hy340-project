//
// Created by nikos on 3/20/18.
//

#include "parser_manager.h"
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

    double Manage_expr__expr_PLUS_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_MINUS_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_MUL_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_DIV_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_MOD_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_GT_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_GE_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_LT_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_LE_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_EQ_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_NE_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_AND_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__expr_OR_expr(double opLeft, double opRight) {
        throw not_implemented_error();
    }

    double Manage_expr__term(double term) {
        throw not_implemented_error();
    }
}