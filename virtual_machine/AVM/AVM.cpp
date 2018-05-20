

#include <cassert>
#include <iostream>
#include "AVM.h"
#include "Memcell.h"

using namespace std;
using namespace virtual_machine;

AVM::AVM(const std::vector<virtual_machine::VMinstruction> &instructions, const std::vector<double> &doubles,
         const std::vector<std::string> &strings, const std::vector<std::string> &libfuncs,
         const std::vector<virtual_machine::Userfunc> &userfuncs,
         unsigned int total_program_vars)  :
        instructions(instructions), const_pool(doubles, strings, libfuncs, userfuncs), program_stack(total_program_vars)
{
    ax = Memcell();
    bx = Memcell();
    cx = Memcell();
    retval = Memcell();
}

Memcell* AVM::translate_operand(const VMarg *vmarg, Memcell *reg /*=nullptr*/) {
    assert(vmarg);

    //Handle the cases in which vmarg is binded to the program stack or retval register
    switch(vmarg->type) {
        case VMarg::Type::program:  return program_stack.get_program_var(vmarg->value);
        case VMarg::Type::formal:   return program_stack.get_formal_arg(vmarg->value);
        case VMarg::Type::local:    return program_stack.get_local_var(vmarg->value);
        case VMarg::Type::retval:   return &retval;
        default:
            break;
    }

    //Handle the cases where a register is used
    assert(reg);
    switch(vmarg->type) {
        case VMarg::Type::number:
            reg->type = Memcell::Type::number;
            reg->value.num = const_pool.get_numeric(vmarg->value);
            return reg;
        case VMarg::Type::string:
            reg->type = Memcell::Type::string;
            reg->value.str_ptr = const_pool.get_string(vmarg->value).c_str();
            return reg;
        case VMarg::Type::boolean:
            reg->type = Memcell::Type::boolean;
            reg->value.boolean = (vmarg->value == 1);
            return reg;
        case VMarg::Type::nil:
            reg->type = Memcell::Type::nil;
            return reg;
        case VMarg::Type::userfunc:
            reg->type = Memcell::Type::userfunc;
            reg->value.userfunc_addr = vmarg->value;
        case VMarg::Type::libfunc:
            reg->type = Memcell::Type::libfunc;
            reg->value.libfunc_ptr = const_pool.get_libfunc(vmarg->value).c_str();
            return reg;
        case VMarg::Type::label:
            reg->type = Memcell::Type::address;
            reg->value.jump_addr = vmarg->value;
            return reg;

        default:
            assert(false);  //Unreachable statement
    }
}

void AVM::execute_cycle() {
	//Check if we have already finished
	if(finished)
		return;

	//Since we have not finished, execute the next instruction
	assert(pc < AVM_ENDING_PC);
	const VMinstruction &cur_instr = instructions.at(pc);
	unsigned int old_pc = pc;
	( this->*(AVM::execute_functions_array[cur_instr.opcode]) )(cur_instr);	//call execute_<VMinstruction opcode> function

	//pc may change due to branches, jump, call and funcexit.
	if(pc == old_pc) //We advance pc to the next instruction only if it wasn't changed by one of the above instructions
		pc++;

	//Check if the program has ended
	if(pc == AVM_ENDING_PC)
		finished = true;
}

void AVM::print_warning(const std::string &msg, unsigned int source_line) const {
    cerr << "WARNING: " << msg << endl;
    cerr << "\t Caused by source line: " << std::to_string(source_line) << endl;
}


