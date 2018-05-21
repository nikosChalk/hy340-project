

#include <cassert>
#include "../AVM.h"

using namespace std;
using namespace virtual_machine;

void AVM::execute_call(const VMinstruction &instr) {

    Memcell *func = translate_operand(&instr.arg1, &ax);
    assert(func);

}