

#include "Stack_manipulator.h"
#include "Program_stack.h"

using namespace virtual_machine;

void Stack_manipulator::move_up(Program_stack &program_stack) {
    program_stack.move_up();
}

void Stack_manipulator::move_down(Program_stack &program_stack) {
    program_stack.move_down();
}
