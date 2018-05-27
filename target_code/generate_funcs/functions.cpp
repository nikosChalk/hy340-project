

#include <cassert>
#include "../VMcode_generator.h"

using namespace std;
using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;
using namespace syntax_analyzer;

void VMcode_generator::generate_PARAM(quad const *quad) {
    generate(VMopcode::pusharg, quad);
}
void VMcode_generator::generate_CALL(quad const *quad) {
    generate(VMopcode::call, quad);
}
void VMcode_generator::generate_GETRETVAL(quad const *quad) {
    emit(VMinstruction(
            VMopcode::assign,
            translate_operand(quad->result),
            translate_retval(),
            nullptr,
            quad->lineno
    ));
}

void VMcode_generator::generate_FUNCSTART(quad const *quad) {
    assert(quad->opcode == quad::iopcode::funcstart);

    //Ignore the generic linking of the caller.
    //We want to link the the "funcenter"quad to the "funcenter" VM instruction, not to the "jump" VM instruction
    //address_links[curr_proc_quad] = Address_link(curr_proc_quad, next_vm_instr_address()+1);

    //Emit incomplete jump before funcenter
    jumps_before_funcenter.push(next_vm_instr_address());
    emit(VMinstruction(
            VMopcode::jump,
            new VMarg(VMarg::Type::label, 0),
            nullptr,
            nullptr,
            quad->lineno)
    );

    //Emit funcenter
    generate(VMopcode::funcenter, quad);
    func_stack.push(vector<unsigned int>());
}

void VMcode_generator::generate_RET(quad const *quad) {
    //In case that we have "return;" (and not e.g. return x;), we do not want to have a nullptr in arg1 because
    //it would crash the AVM when the assign would execute
    VMarg *arg1 = ((quad->arg1 == nullptr) ? new VMarg(VMarg::Type::nil, 0) : translate_operand(quad->arg1));

    emit(VMinstruction(
            VMopcode ::assign,
            translate_retval(),
            arg1,
            nullptr,
            quad->lineno
    ));

    func_stack.top().push_back(next_vm_instr_address());    //push the incomplete jump which we will now emit

    //Emit an incomplete jump
    emit(VMinstruction(
            VMopcode::jump,
            new VMarg(VMarg::Type::label, 0),   //incomplete jump
            nullptr,
            nullptr,
            quad->lineno
    ));
}

void VMcode_generator::generate_FUNCEND(quad const *quad) {
    assert(!func_stack.empty() && !jumps_before_funcenter.empty());
    vector<unsigned int> incomplete_jumps = func_stack.top();
    func_stack.pop();

    for(unsigned int vm_addr : incomplete_jumps)    //patch the "return" incomplete jumps
        vm_instructions.at(vm_addr).result->value = next_vm_instr_address();

    emit(VMinstruction(
            VMopcode::funcexit,
            nullptr,
            nullptr,
            nullptr,
            quad->lineno
    ));

    //Patch the incomplete jump before funcenter to the next instruction after funcexit.
    unsigned int addr_incomp_jmp = jumps_before_funcenter.top();
    jumps_before_funcenter.pop();
    vm_instructions.at(addr_incomp_jmp).result->value = next_vm_instr_address();
}