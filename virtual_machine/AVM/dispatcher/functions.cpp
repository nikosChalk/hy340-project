

#include <cassert>
#include <sstream>
#include "../AVM.h"
#include "../Memcell.h"
#include "../errors/stack_overflow_error.h"
#include "../errors/alpha_runtime_error.h"

using namespace std;
using namespace virtual_machine;

void AVM::execute_call(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::call);

    Memcell *func = translate_operand(&instr.arg1, &ax);
    assert(func);

    //Prepare to save environment
    unsigned int userfunc_addr;
    char const *libfunc_name = nullptr;
    switch(func->type) {
        case Memcell::Type::userfunc:
            assert(instr.opcode == VMopcode::funcenter);

            userfunc_addr = func->value.userfunc_addr; //address of "funcstart" instruction to which we will jump to
            assert(userfunc_addr < AVM_ENDING_PC);
            break;
        case Memcell::Type::string:
            libfunc_name = func->value.str_ptr;
            break;
        case Memcell::Type::libfunc:
            libfunc_name = func->value.libfunc_ptr;
            break;
        default:
            stringstream ss;
            ss << "call: cannot bind '" << func->to_string() << "' to function!";
            throw alpha_runtime_error(ss.str(), instr.source_line);
    }

    //save environment
    program_stack.save_environment(pc);

    //call function
    switch(func->type) {
        case Memcell::Type::userfunc:
            pc = userfunc_addr;
            return;
        case Memcell::Type::string:
        case Memcell::Type::libfunc:
            call_library_function(libfunc_name);
            return;
        default:
            assert(false);  //unreachable statement
    }
}

void AVM::execute_funcenter(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::funcenter);

    Memcell *func = translate_operand(&instr.arg1, &ax);
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

    Memcell *arg = translate_operand(&instr.arg1, &ax);
    assert(arg);

    program_stack.push_actual_arg(arg);
}

void AVM::call_library_function(const std::string &name) {
    lib_func_t func = get_library_function(name);
    assert(func);

    (this->*func)();    //call built in library function
    pc = program_stack.restore_environment();
}