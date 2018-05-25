

#include <cassert>
#include "../VMcode_generator.h"

using namespace std;
using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate_PARAM(quad const *quad) {
    generate(VMopcode::pusharg, quad);
}
void VMcode_generator::generate_CALL(quad const *quad) {
    generate(VMopcode::call, quad);
}
void VMcode_generator::generate_GETRETVAL(quad const *quad) {
    emit(VMinstruction(
            VMopcode::assign,
            translate_retval(),
            translate_operand(quad->arg1),
            nullptr,
            quad->lineno
    ));
}

void VMcode_generator::generate_FUNCSTART(quad const *quad) {
    generate(VMopcode::funcenter, quad);
    func_stack.push(vector<unsigned int>());
}

void VMcode_generator::generate_RET(quad const *quad) {
    emit(VMinstruction(
            VMopcode ::assign,
            translate_retval(),
            translate_operand(quad->arg1),
            nullptr,
            quad->lineno
    ));

    func_stack.top().push_back(next_vm_instr_address());    //push the incomplete jump which we will now emit

    //Emit an incomplete jump
    VMarg *result = new VMarg();
    result->type = VMarg::Type::label;

    emit(VMinstruction(
            VMopcode::jump,
            result,
            nullptr,
            nullptr,
            quad->lineno
    ));
}

void VMcode_generator::generate_FUNCEND(quad const *quad) {
    assert(!func_stack.empty());
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
}