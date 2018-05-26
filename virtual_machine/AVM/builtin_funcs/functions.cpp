

#include "../AVM.h"
#include "../Program_stack/Stack_manipulator.h"
#include "../../../not_implemented_error.h"

using namespace std;
using namespace virtual_machine;

/* Functions in this file should be declared in Program_stack.h with the "friend" declaration specifier as they rely    */
/* heavily on stack manipulation                                                                                        */

void AVM::libfunc_totalarguments(void) {
    throw not_implemented_error(); //TODO: implement
}

void AVM::libfunc_argument(void) {
    Stack_manipulator::set_topsp(program_stack, 10000000); //example of how we can access private members. TODO: remove this line
    throw not_implemented_error(); //TODO: implement
}