

#include <cassert>
#include "scope_handler.h"

using namespace std;
using namespace syntax_analyzer;

const unsigned int scope_handler::GLOBAL_SCOPE = 0;

scope_handler::scope_handler() {
    scope = GLOBAL_SCOPE;

    ss_stack = stack<scope_space>();
    ss_stack.push(PROGRAM_SS);

    ss_offset_stack = stack<unsigned int>();
    ss_offset_stack.push(0);

    func_scope_stack = stack<unsigned int>();
}

unsigned int scope_handler::get_current_scope() const {
    return scope;
}

unsigned int scope_handler::get_active_function_scope() const {
    return (func_scope_stack.empty()) ? GLOBAL_SCOPE : func_scope_stack.top();
}

scope_handler::scope_space scope_handler::get_current_ss() const {
    return ss_stack.top();
}

unsigned int scope_handler::increase_scope() {
    scope++;
}

unsigned int scope_handler::decrease_scope() {
    assert(scope != 0); //avoid possible wrap-arround due to programming fault
    scope--;
}

unsigned int scope_handler::fetch_and_incr_cur_ssoffset() {
    unsigned int ss_offset = ss_offset_stack.top();
    ss_offset_stack.pop();
    ss_offset_stack.push(ss_offset+1);
    return ss_offset;
}

void scope_handler::enter_function_ss() {
    ss_stack.push(FUNCTION_LOCALS_SS);
    ss_offset_stack.push(0);
    func_scope_stack.push(scope);
}

void scope_handler::exit_function_ss() {
    ss_stack.pop();
    ss_offset_stack.pop();
    func_scope_stack.pop();
}

void scope_handler::enter_formal_arg_ss() {
    ss_stack.push(FORMAL_ARGUMENTS_SS);
    ss_offset_stack.push(0);
}

void scope_handler::exit_formal_arg_ss() {
    ss_stack.pop();
    ss_offset_stack.pop();
}