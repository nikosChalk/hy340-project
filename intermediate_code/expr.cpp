

#include <cassert>
#include <sstream>
#include "expr.h"

using namespace std;
using namespace syntax_analyzer;
using namespace intermediate_code;

expr::expr() {
    expr_type = CONST_NIL_E;   //for debugging
    sym_entry = nullptr;
    index = nullptr;
    short_circ_extn = sc_extension();
}

expr* expr::make_lvalue_expr(symbol_table::entry *sym_entry) {
    assert(sym_entry);

    expr *new_expr = new expr();
    new_expr->sym_entry = sym_entry;

    if(sym_entry->get_lvalue_type() == symbol_table::entry::VAR) {
        new_expr->expr_type = type::VAR_E;
    } else {
        if(sym_entry->get_sym_type() == symbol_table::entry::USER_FUNC)
            new_expr->expr_type = type::PROGRAM_FUNC_E;
        else
            new_expr->expr_type = type::LIBRARY_FUNC_E;
    }

    return new_expr;
}

expr* expr::make_table_item(symbol_table::entry *sym_entry, expr *index) {
    expr *item = expr::make_expr(type::TABLE_ITEM_E);
    item->sym_entry = sym_entry;
    item->index = index;

    return item;
}

expr* expr::make_table_item(symbol_table::entry *sym_entry, const string &id) {
    return make_table_item(sym_entry, expr::make_const_str(id));
}

expr* expr::make_expr(type t) {
    expr *new_expr = new expr();
    new_expr->expr_type = t;
    return new_expr;
}

expr* expr::make_const_num(long double num) {
    expr *new_expr = expr::make_expr(type::CONST_NUM_E);
    new_expr->const_val.number = num;
    return new_expr;
}

expr* expr::make_const_str(string const &str) {
    expr *new_expr = expr::make_expr(type::CONST_STR_E);
    new_expr->const_val.str = string(str);
    return new_expr;
}

expr* expr::make_const_bool(bool value) {
    expr *new_expr = expr::make_expr(type::CONST_BOOL_E);
    new_expr->const_val.boolean = value;
    return new_expr;
}

expr* expr::make_const_nil() {
    return expr::make_expr(type::CONST_NIL_E);
}

bool expr::can_participate_in_arithmop() const {
    switch(this->expr_type) {
        case type::VAR_E:           //depends on runtime.
        case type::TABLE_ITEM_E:    //depends on runtime
        case type::ARITHM_E:        //ok at compile time
        case type::ASSIGN_E:        //depends on runtime
        case type::CONST_NUM_E:     //ok at compile time
            return true;
        default:    //else
            return false;
    }
}

bool expr::can_participate_in_relop() const {
    return can_participate_in_arithmop();   //same condition
}

const bool expr::must_be_patched() const {
    if(!short_circ_extn.get_truelist().empty() || !short_circ_extn.get_falselist().empty()) {
        assert(expr_type == type::BOOL_E);  //Since it has non-empty truelist or falselist, it MUST be of type::BOOL_E
        return true;
    }
    return false;
}

std::string expr::to_string() const {
    stringstream ss;

    switch(this->expr_type) {
        case type::VAR_E:
        case type::TABLE_ITEM_E:
        case type::PROGRAM_FUNC_E:
        case type::LIBRARY_FUNC_E:
        case type::ARITHM_E:
        case type::BOOL_E:
        case type::ASSIGN_E:
        case type::NEW_TABLE_E:
            assert(sym_entry);
            return string(sym_entry->get_name());
        case type::CONST_NUM_E:
            return std::to_string(const_val.number);
        case type::CONST_BOOL_E:
            return (const_val.boolean) ? string("true") : string("false");
        case type::CONST_STR_E:
            return string(const_val.str);
        case type::CONST_NIL_E:
            return string("nil");
        default:
            assert(0);  //unreachable statement
    }
}



/* ~~~~~~~ sc_extension ~~~~~~~ */
expr::sc_extension::sc_extension() {
    this->truelist = vector<unsigned int>();
    this->falselist = vector<unsigned int>();
}

void expr::sc_extension::append_to_truelist(const std::vector<unsigned int> &truelist) {
    for(unsigned int quadno : truelist)
        append_to_truelist(quadno);
}

void expr::sc_extension::append_to_truelist(unsigned int quadno) {
    truelist.push_back(quadno);
}

void expr::sc_extension::append_to_falselist(const std::vector<unsigned int> &falselist) {
    for(unsigned int quadno : falselist)
        append_to_falselist(quadno);
}

void expr::sc_extension::append_to_falselist(unsigned int quadno) {
    falselist.push_back(quadno);
}

const std::vector<unsigned int>& expr::sc_extension::get_truelist() const {
    return truelist;
}

const std::vector<unsigned int>& expr::sc_extension::get_falselist() const {
    return falselist;
}

