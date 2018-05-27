

#include <cassert>
#include "Func_handler.h"

using namespace std;
using namespace syntax_analyzer;

Func_handler::Func_handler() {
    func_stack = stack<symbol_table::func_entry*>();
}

void Func_handler::enter_function(symbol_table::func_entry *func_st_entry) {
    assert(func_st_entry);
    func_stack.push(func_st_entry);
}

void Func_handler::exit_function() {
    assert(!func_stack.empty());
    func_stack.pop();
}

symbol_table::func_entry* Func_handler::get_active_func() const {
    return func_stack.top();
}