

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include "AVM.h"
#include "Memcell.h"
#include "errors/internal_error.h"
#include "errors/alpha_runtime_error.h"
#include "../../common_interface/errors/numeric_error.h"

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
    retval = Memcell();
    pc = 0;
    finished = false;
}

Memcell* AVM::translate_operand(const VMarg *vmarg, Memcell *reg /*=nullptr*/) {
    assert(vmarg);

    //Handle the cases in which vmarg is binded to the program stack or retval register
    switch(vmarg->type) {
        case VMarg::Type::program:  return program_stack.get_program_var(vmarg->value);
        case VMarg::Type::formal:   return program_stack.get_actual_arg(vmarg->value);
        case VMarg::Type::local:    return program_stack.get_local_var(vmarg->value);
        case VMarg::Type::retval:   return &retval;
        default:
            break;
    }

    //Handle the cases where a register is used
    assert(reg);
    reg->clear();   //TODO: validate that this is legal
    switch(vmarg->type) {
        case VMarg::Type::number:
            reg->type = Memcell::Type::number;
            reg->value.num = const_pool.get_numeric(vmarg->value);
            return reg;
        case VMarg::Type::string:
            reg->type = Memcell::Type::string;
            reg->value.str_ptr = strdup(const_pool.get_string(vmarg->value).c_str());
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
            reg->value.userfunc_addr = const_pool.get_userfunc_on_idx(vmarg->value).address;
        case VMarg::Type::libfunc:
            reg->type = Memcell::Type::libfunc;
            reg->value.libfunc_ptr = const_pool.get_libfunc(vmarg->value).c_str();
            return reg;

        default:
            assert(false);  //Unreachable statement
    }
}

void AVM::run() {
    while(!finished)
        execute_cycle();
}

void AVM::execute_cycle() {
	//Check if we have already finished
	if(finished)
		return;

	//Since we have not finished, execute the next instruction
	assert(pc < AVM_ENDING_PC);
	const VMinstruction &cur_instr = instructions.at(pc);
	unsigned int old_pc = pc;

	//Execute instruction
	try {
	    //Some Execute functions may throw an internal_error
        (this->*(AVM::execute_functions_array[cur_instr.opcode]))(cur_instr);    //call execute_<VMinstruction opcode> function
    } catch(internal_error const &err) {
	    stringstream ss;
	    ss << "Error caused by VM instruction '" << cur_instr.to_string() << "'. " << endl;
	    ss << err.what();
	    throw alpha_runtime_error(ss.str(), cur_instr.source_line);
	}

	//pc may change due to branches, jump, call and funcexit.
	if(pc == old_pc) //We advance pc to the next instruction only if it wasn't changed by one of the above instructions
		pc++;

	//Check if the program has ended
	if(pc == AVM_ENDING_PC)
		finished = true;
}

AVM::lib_func_t AVM::get_library_function(const std::string &name) {
    return AVM::name_to_libfunc_map.at(name);
}

void AVM::print_warning(const std::string &msg, unsigned int source_line) const {
    cerr << "WARNING: " << msg << endl;
    cerr << "\t Caused by source line: " << std::to_string(source_line) << endl;
}


