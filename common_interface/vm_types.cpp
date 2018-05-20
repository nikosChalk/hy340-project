

#include "vm_types.h"

using namespace std;
using namespace virtual_machine;

VMarg::VMarg(virtual_machine::VMarg::Type type, unsigned int value)
        : type(type), value(value) { }

VMinstruction::VMinstruction(VMopcode opcode, const VMarg &result, const VMarg &arg1, const VMarg &arg2, unsigned int source_line)
        :  opcode(opcode), result(result), arg1(arg1), arg2(arg2), source_line(source_line) { }

Userfunc::Userfunc(unsigned int address, unsigned int nr_locals, const std::string &name)
        : address(address), nr_locals(nr_locals), name(name) { }