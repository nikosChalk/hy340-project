

#include <cassert>
#include "Program_stack.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

/**
 * Validates the given index to check if it is within the allocated stack.
 */
#define assert_index(idx) assert( (top < (idx)) && (idx) < Program_stack::AVM_STACK_SIZE )

/**
 * top_allocated_idx is the top allocated memcell index on the stack
 */
#define top_allocated_idx (top+1)

/**
 * Offsets from "topsp" index for the corresponding environmental values
 */

Program_stack::Program_stack(unsigned int total_program_vars) {
    for(unsigned int i=0; i<stack.size(); i++)
        stack[i] = Memcell();

    this->total_program_vars = total_program_vars;
    top = Program_stack::AVM_STACK_SIZE - total_program_vars-1;
    topsp = top; //since we do not have a function record
    total_actuals = 0;
    fcall_depth = 0;
    saved_pointers_stack = std::stack<std::pair<int, int>>();
}

void Program_stack::allocate_memcells(unsigned int n) {

    //Start allocation
    for(unsigned int i=0; i<n; i++) {
        if(top == -1)   //Check for overflow
            throw internal_error("Stack Overflow");

        //No overflow will occur. Safely allocate one memcell
        top--;
    }
}

void Program_stack::save_environment(unsigned int pc) {
    assert(saved_pointers_stack.empty());   //assert that we are in the most recent called function

    for(unsigned int i=0; i< AVM_ENV_SIZE; i++) {
        allocate_memcells(1);
        stack[top_allocated_idx].type = Memcell::Type::number;
    }

    stack[top_allocated_idx].value.num = topsp;
    topsp = top;

    stack[topsp + SAVED_TOP_OFFSET].value.num = top+AVM_ENV_SIZE+total_actuals;
    stack[topsp + SAVED_PC_OFFSET].value.num = pc+1;
    stack[topsp + SAVED_TOTAL_ACTUALS].value.num = total_actuals;
    total_actuals = 0;  //reset counter of actual arguments
    fcall_depth++;  //increase function call depth
}

unsigned int Program_stack::restore_environment() {
    assert(fcall_depth > 0 && saved_pointers_stack.empty());
    int old_top = top;

    //Restore environment
    unsigned int pc = get_env_value((unsigned int)(topsp+SAVED_PC_OFFSET));
    top = get_env_value((unsigned int)(topsp + SAVED_TOP_OFFSET));
    topsp = get_env_value((unsigned int)(topsp + SAVED_TOPSP_OFFSET));

    //Clear de-allocated cells
    while(++old_top <= top)     //Intentionally ignore first
        stack[old_top].clear();

    fcall_depth--;
    return pc;
}



unsigned int Program_stack::get_fcall_depth() const {
    return fcall_depth;
}

void Program_stack::push_actual_arg(Memcell const *actual_arg) {
    assert(actual_arg);

    allocate_memcells(1);
    stack[top_allocated_idx].assign(actual_arg);
    total_actuals++;    //increase the actuals pushed
}

unsigned int Program_stack::get_env_value(unsigned int idx) const {
    assert(idx == topsp+SAVED_TOPSP_OFFSET || idx == topsp+SAVED_TOP_OFFSET ||
           idx == topsp+SAVED_PC_OFFSET   || idx == topsp+SAVED_TOTAL_ACTUALS);
    assert(stack.at(idx).type == Memcell::Type::number);   //environmental values are numbers

    unsigned int env_val = (unsigned int)stack.at(idx).value.num;
    assert(stack.at(idx).value.num == (long double) env_val);   //validate that the environmental value is indeed an integer

    return env_val;
}

unsigned long Program_stack::get_program_var_index(unsigned int offset) const {
    assert(offset < this->total_program_vars);

    unsigned long idx = stack.size()-1-offset;
    assert_index(idx);
    return idx;
}
Memcell* Program_stack::get_program_var(unsigned int offset) {
    return &stack[get_program_var_index(offset)];
}
const Memcell* Program_stack::get_program_var(unsigned int offset) const {
    return &stack.at(get_program_var_index(offset));
}


unsigned long Program_stack::get_local_var_index(unsigned int offset) const {
    //validates that we are NOT in the program scope space by checking if at least one saved environment exists
    assert( top < stack.size() - total_program_vars - AVM_ENV_SIZE);

    unsigned int idx = topsp-offset;
    assert_index(idx);
    return idx;
}
Memcell* Program_stack::get_local_var(unsigned int offset) {
    return &stack[get_local_var_index(offset)];
}
const Memcell* Program_stack::get_local_var(unsigned int offset) const {
    return &stack.at(get_local_var_index(offset));
}


unsigned long Program_stack::get_actual_arg_index(unsigned int offset) const {
    assert(offset < get_total_actuals());    //validate that offset is less thant the total actuals

    unsigned int idx = topsp + AVM_ENV_SIZE + 1 + offset;
    assert_index(idx);
    return idx;
}
Memcell* Program_stack::get_actual_arg(unsigned int offset) {
    return &stack[get_actual_arg_index(offset)];
}
const Memcell* Program_stack::get_actual_arg(unsigned int offset) const {
    return &stack.at(get_actual_arg_index(offset));
}

unsigned int Program_stack::get_total_actuals() const {
    return get_env_value((unsigned int) (topsp + SAVED_TOTAL_ACTUALS));
}

bool Program_stack::is_in_stack(const Memcell *memcell) const {
    return ( &stack[top] < memcell && memcell <= &stack[Program_stack::AVM_STACK_SIZE-1] );
}

void Program_stack::move_up() {
    assert(fcall_depth > saved_pointers_stack.size()+1);    //assert that there is an active function to which we can go up

    saved_pointers_stack.push(make_pair(top, topsp));
    top = get_env_value((unsigned int)(topsp + SAVED_TOP_OFFSET));
    topsp = get_env_value((unsigned int)(topsp + SAVED_TOPSP_OFFSET));
}

void Program_stack::move_down() {
    assert(!saved_pointers_stack.empty());

    std::pair saved_pointers_pair = saved_pointers_stack.top();
    saved_pointers_stack.pop();
    top = saved_pointers_pair.first;
    topsp = saved_pointers_pair.second;
}