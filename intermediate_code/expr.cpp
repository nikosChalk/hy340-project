

#include <cassert>
#include "types.h"

using namespace std;
using namespace syntax_analyzer;
using namespace intermediate_code;

expr::expr() {
    type = NIL_E;   //for debugging
    sym_entry = nullptr;
    index = nullptr;
}

expr* expr::make_lvalue_expr(symbol_table::entry *sym_entry) {
    expr *new_expr;
    assert(sym_entry);

    new_expr = new expr();
    new_expr->sym_entry = sym_entry;

    if(sym_entry->get_lvalue_type() == symbol_table::entry::VAR) {
        new_expr->type = expression_type::VAR_E;
    } else {
        if(sym_entry->get_sym_type() == symbol_table::entry::USER_FUNC)
            new_expr->type = expression_type::PROGRAM_FUNC_E;
        else
            new_expr->type = expression_type::LIBRARY_FUNC_E;
    }

    return new_expr;
}

expr* expr::make_expr(intermediate_code::expression_type type) {
    expr *new_expr = new expr();
    new_expr->type = type;
    return new_expr;
}

expr* expr::make_const_str(string const &str) {
    expr *new_expr = expr::make_expr(expression_type::CONST_STR_E);
    new_expr->const_val.str = str;

    return new_expr;
}