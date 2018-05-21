

#include <cassert>
#include "vm_types.h"

using namespace std;
using namespace virtual_machine;

VMarg::VMarg(virtual_machine::VMarg::Type type, unsigned int value)
        : type(type), value(value) {
    if(type == boolean)
        assert(value == 0 || value == 1);   //forbid other values for type of boolean
}

VMarg::VMarg() = default;

VMinstruction::VMinstruction(VMopcode opcode, VMarg *result, VMarg *arg1, VMarg *arg2, unsigned int source_line)
        :  opcode(opcode), result(result), arg1(arg1), arg2(arg2), source_line(source_line) { }

VMinstruction::VMinstruction() {
    opcode = add;   //uses all 3 fields, so it is very likely that an assertion will be hit
    result = arg1 = arg2 = nullptr;
}


Userfunc::Userfunc(unsigned int address, unsigned int nr_locals, const std::string &name)
        : address(address), nr_locals(nr_locals), name(name) { }

Userfunc::Userfunc() = default;