

#include <cassert>
#include <sstream>
#include "../AVM.h"
#include "../Memcell.h"
#include "../errors/internal_error.h"
#include "../../../common_interface/utilities.h"

using std::stringstream;
using namespace virtual_machine;

void AVM::execute_call(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::call);

    Memcell *func = translate_operand(instr.arg1, &ax);
    assert(func);

    //save environment
    program_stack.save_environment(pc);

    //Call function
    switch(func->type) {
        case Memcell::Type::userfunc: {
            unsigned int userfunc_addr = func->value.userfunc_addr; //address of "funcstart" instruction to which we will jump to
            assert(instructions.at(userfunc_addr).opcode == VMopcode::funcenter);
            assert(userfunc_addr < AVM_ENDING_PC);
            pc = userfunc_addr;
            return;
        }
        case Memcell::Type::string:
            call_library_function(func->value.str_ptr);
            return;
        case Memcell::Type::libfunc:
            call_library_function(func->value.libfunc_ptr);
            return;
        default: {
            stringstream ss;
            ss << "call: cannot bind '" << func->to_string(const_pool) << "' to function!";
            throw internal_error(ss.str());
        }
    }
}

void AVM::execute_funcenter(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::funcenter);

    Memcell *func = translate_operand(instr.arg1, &ax);
    assert(func && func->type == Memcell::userfunc);
    assert(pc == func->value.userfunc_addr);

    //Callee action. Allocate memory for function's locals
    program_stack.allocate_memcells(const_pool.get_userfunc_on_address(func->value.userfunc_addr).nr_locals);
}

void AVM::execute_funcexit(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::funcexit);
    pc = program_stack.restore_environment();
}

void AVM::execute_pusharg(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::pusharg);

    Memcell *arg = translate_operand(instr.arg1, &ax);
    assert(arg);

    program_stack.push_actual_arg(arg);
}

void AVM::call_library_function(const std::string &name) {
    Lib_membfunc_ptr func = AVM::get_library_function(name);
    assert(func);


    call_member_func_ptr(*this, func)();    //call built in library function
    pc = program_stack.restore_environment();
}