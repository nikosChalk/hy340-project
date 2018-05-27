

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

    retval.clear();

    //Check if the function was called in global scope
    if(program_stack.get_fcall_depth() == 1) {  //The only active function is totalarguments. Return nil
        retval.type = Memcell::Type::nil;
    } else {
        //Find total arguments of the previous called function which is in the previous activation record
        retval.type = Memcell::Type::number;

        Stack_manipulator::move_up(program_stack);  //go to previous activation record
        retval.value.num = program_stack.get_total_actuals();   //get total actuals of the previous active function
        Stack_manipulator::move_down(program_stack);    //reset pointers (top, topsp) to the current active function so that we can safely return
    }
}

void AVM::libfunc_argument(void) {
    unsigned int args = program_stack.get_total_actuals();

    if(args != 1)
        throw internal_error("Expected 1 argument in library function 'argument' but got " + std::to_string(args));

    Memcell const *first_arg = program_stack.get_actual_arg(0);	//the index i of the actual of the previous function that we want to return
    if(first_arg->type != Memcell::Type::number)
        throw internal_error("Argument type of 'argument' library function must be integer number");

    unsigned int idx = (unsigned int) first_arg->value.num;
    if(first_arg->value.num != (long double) idx)    //validate that the given idx is indeed an integer
        throw internal_error("Argument of 'argument' must be integer value but got real number: " + std::to_string(first_arg->value.num));

    retval.clear();

    //Check if the function was called in global scope
    if(program_stack.get_fcall_depth() == 1) {  //The only active function is argument. Return nil
        retval.type = Memcell::Type::nil;
    } else {
        Stack_manipulator::move_up(program_stack);  //go to previous activation record

        unsigned int prev_func_actuals = program_stack.get_total_actuals();
        if(idx >= prev_func_actuals)
            throw internal_error("argument library function: Requested argument in index " +
                                 std::to_string(idx) + " but function only has: " + std::to_string(prev_func_actuals) + " arguments"
            );

        //Get i-th argument and assign it to retval
        retval.assign(program_stack.get_actual_arg(idx));

        Stack_manipulator::move_down(program_stack);    //reset pointers (top, topsp) to the current active function so that we can safely return
    }
}