

#include "Stack_manipulator.h"
#include "Program_stack.h"

using namespace virtual_machine;

int Stack_manipulator::get_topsp(const Program_stack &program_stack) {
    return program_stack.topsp;
}

void Stack_manipulator::set_topsp(Program_stack &program_stack, int topsp) {
    program_stack.topsp = topsp;
}
