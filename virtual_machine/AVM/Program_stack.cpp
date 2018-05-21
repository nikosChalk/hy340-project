

#include <cassert>
#include "Program_stack.h"

using namespace std;
using namespace virtual_machine;

/**
 * Validates the given index to check if it is within the allocated stack.
 */
#define assert_index(idx) assert( (top < (idx)) && (idx) < AVM_ENV_SIZE )

Program_stack::Program_stack(unsigned int total_program_vars) {
    for(unsigned int i=0; i<stack.size(); i++)
        stack[i] = Memcell();

    this->total_program_vars = total_program_vars;
    top = total_program_vars;
    topsp = total_program_vars; //since we do not have a function record
}

Memcell* Program_stack::get_program_var(unsigned int offset) {
    assert(offset < this->total_program_vars);

    unsigned long idx = stack.size()-1-offset;
    assert_index(idx);

    return &stack[idx];
}

Memcell* Program_stack::get_local_var(unsigned int offset) {
    //validates that we are NOT in the program scope space by checking if at least one saved environment exists
    assert( top < stack.size() - total_program_vars - AVM_ENV_SIZE);

    unsigned int idx = topsp-offset;
    assert_index(idx);

    return &stack[idx];
}

Memcell* Program_stack::get_formal_arg(unsigned int offset) {
    unsigned int idx = topsp + AVM_ENV_SIZE + 1 + offset;
    assert_index(idx);

    return &stack[idx];
}

bool Program_stack::is_in_stack(const Memcell *memcell) const {
    return ( &stack[top] < memcell && memcell <= &stack[AVM_ENV_SIZE-1] );
}