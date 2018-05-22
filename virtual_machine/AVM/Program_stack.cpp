

#include <cassert>
#include "Program_stack.h"
#include "errors/internal_error.h"
#include "Memcell.h"

using namespace std;
using namespace virtual_machine;

/**
 * Validates the given index to check if it is within the allocated stack.
 */
#define assert_index(idx) assert( (top < (idx)) && (idx) < AVM_ENV_SIZE )

/**
 * top_allocated_idx is the top allocated memcell index on the stack
 */
#define top_allocated_idx (top+1)

/**
 * Offsets from "topsp" index for the corresponding environmental values
 */
#define AVM_SAVED_TOPSP_OFFSET   (+1)
#define AVM_SAVED_TOP_OFFSET    (+2)
#define AVM_SAVED_PC_OFFSET     (+3)
#define AVM_SAVED_TOTAL_ACTUALS (+4)

Program_stack::Program_stack(unsigned int total_program_vars) {
    for(unsigned int i=0; i<stack.size(); i++)
        stack[i] = Memcell();

    this->total_program_vars = total_program_vars;
    top = total_program_vars;
    topsp = total_program_vars; //since we do not have a function record
    total_actuals = 0;
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
    for(int i=0; i< AVM_ENV_SIZE; i++) {
        allocate_memcells(1);
        stack[top_allocated_idx].type = Memcell::Type::number;
    }

    stack[top_allocated_idx].value.num = topsp;
    topsp = top;

    stack[topsp + AVM_SAVED_TOP_OFFSET].value.num = top+AVM_ENV_SIZE+total_actuals;
    stack[topsp + AVM_SAVED_PC_OFFSET].value.num = pc+1;
    stack[topsp + AVM_SAVED_TOTAL_ACTUALS].value.num = total_actuals;
    total_actuals = 0;  //reset counter of actual arguments
}

unsigned int Program_stack::restore_environment() {
    int old_top = top;

    //Restore environment
    unsigned int pc = get_env_value((unsigned int)(topsp+AVM_SAVED_PC_OFFSET));
    top = get_env_value((unsigned int)(topsp + AVM_SAVED_TOP_OFFSET));
    topsp = get_env_value((unsigned int)(topsp + AVM_SAVED_TOPSP_OFFSET));

    //Clear de-allocated cells
    while(++old_top <= top)     //Intentionally ignore first
        stack[old_top].clear();
    return pc;
}

void Program_stack::push_actual_arg(Memcell const *actual_arg) {
    assert(actual_arg);

    allocate_memcells(1);
    stack[top_allocated_idx].assign(actual_arg);
    total_actuals++;    //increase the actuals pushed
}

unsigned int Program_stack::get_env_value(unsigned int idx) {
    assert(idx == topsp+AVM_SAVED_TOPSP_OFFSET || idx == topsp+AVM_SAVED_TOP_OFFSET ||
           idx == topsp+AVM_SAVED_PC_OFFSET   || idx == topsp+AVM_SAVED_TOTAL_ACTUALS);
    assert(stack[idx].type == Memcell::Type::number);   //environmental values are numbers

    unsigned int env_val = (unsigned int)stack[idx].value.num;
    assert(stack[idx].value.num == (double) env_val);   //validate that the environmental value is indeed an integer

    return env_val;
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

Memcell* Program_stack::get_actual_arg(unsigned int offset) {
    assert(offset < get_total_actuals());    //validate that offset is less thant the total actuals

    unsigned int idx = topsp + AVM_ENV_SIZE + 1 + offset;
    assert_index(idx);

    return &stack[idx];
}

unsigned int Program_stack::get_total_actuals() {
    return get_env_value((unsigned int) (topsp + AVM_SAVED_TOTAL_ACTUALS));
}

bool Program_stack::is_in_stack(const Memcell *memcell) const {
    return ( &stack[top] < memcell && memcell <= &stack[AVM_ENV_SIZE-1] );
}