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
        fprintf(yyout, "member -> .IDENTFIER\n");
        return void_value;
    }
    void_t Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET() {
        fprintf(yyout, "member -> lvalue[expr]\n");
        return void_value;
    }
    void_t Manage_member__call_DOT_IDENTIFIER() {
        fprintf(yyout, "member -> call.IDENTFIER\n");
        return void_value;
    }
    void_t Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET() {
        fprintf(yyout, "member -> [expr]\n");
        return void_value;
    }
}