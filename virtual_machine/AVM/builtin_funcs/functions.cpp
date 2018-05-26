

#include "../AVM.h"
#include "../Program_stack/Stack_manipulator.h"
#include "../../../not_implemented_error.h"
#include "../errors/internal_error.h"
#include "../Memcell.h"

using namespace std;
using namespace virtual_machine;

/* Functions in this file should be declared in Program_stack.h with the "friend" declaration specifier as they rely    */
/* heavily on stack manipulation                                                                                        */

void AVM::libfunc_totalarguments(void) {
    /*
     * Get topsp of previous activation record.
     */
    unsigned p_topsp = Program_stack::get_env_value(program_stack.topsp + AVM_SAVED_TOPSP_OFFSET);
    retval.clear();

    if(!p_topsp){ /* If 0, no previous activation record. */
        throw internal_error("'totalarguments' called outside a function!");
        retval.type = Memcell::Type::nil;
    }
    else{
        /* Extract the number of actual arguments for the previous activation record. */
        retval.type = Memcell::Type::number;
        retval.value.num = Program_stack::get_env_value(p_topsp + AVM_SAVED_TOTAL_ACTUALS);
    }
}

void AVM::libfunc_argument(void) {
    //Stack_manipulator::set_topsp(program_stack, 10000000); //example of how we can access private members. TODO: remove this line

    /*
    * Get topsp of previous activation record.
    */
    unsigned p_topsp = Program_stack::get_env_value(program_stack.topsp + AVM_SAVED_TOPSP_OFFSET);
    retval.clear();

    /*
     * Get the argument of the stack.
     */
    retval.assign(Program_stack::get_actual_arg(p_topsp + 0);

    if(!p_topsp){ /* If 0, no previous activation record. */
        throw internal_error("'totalarguments' called outside a function!");
        retval.type = Memcell::Type::nil;
    } else{
        /* Extract the number of actual arguments for the previous activation record. */
        retval.type = Memcell::Type::number;
        retval.value.num = Program_stack::get_env_value(p_topsp + AVM_SAVED_TOTAL_ACTUALS);
    }
}