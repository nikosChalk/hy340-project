

#include <cassert>
#include "../AVM.h"

using namespace std;
using namespace virtual_machine;

void AVM::execute_assign(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::assign);

    Memcell *lv = translate_operand(instr.result);
    Memcell *rv = translate_operand(instr.arg1, &ax);

    assert(lv && (program_stack.is_in_stack(lv) || lv == &retval));
    assert(rv);

    if(rv->type == Memcell::Type::undef)
        print_warning("Assigning from undef content", instr.source_line);

    lv->assign(rv);
}

void AVM::execute_jump(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::jump);
    assert(instr.result->type == VMarg::Type::label);

    //Perform jump
    pc = instr.result->value;
}

void AVM::execute_nop(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::nop);
    //nop. Nothing to do here
}