

#include <cassert>
#include "../AVM.h"

using namespace std;
using namespace virtual_machine;

void AVM::execute_assign(const virtual_machine::VMinstruction &instr) {
    Memcell *lv = translate_operand(&instr.result);
    Memcell *rv = translate_operand(&instr.arg1, &ax);

    assert(lv && program_stack.is_in_stack(lv));
    assert(rv);

    if(rv->type == Memcell::Type::undef)
        print_warning("Assigning from undef content", instr.source_line);

    lv->assign(rv);
}