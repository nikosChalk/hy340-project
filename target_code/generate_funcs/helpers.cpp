

#include "../VMcode_generator.h"

using namespace std;
using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate(VMopcode vmopcode, quad const *quad) {
    emit(VMinstruction(
            vmopcode,
            translate_operand(quad->result),
            translate_operand(quad->arg1),
            translate_operand(quad->arg2),
            quad->lineno
    ));
}

void target_code::VMcode_generator::generate_relational(VMopcode vmopcode, quad const *quad) {
    VMarg *arg1 = translate_operand(quad->arg1);
    VMarg *arg2 = translate_operand(quad->arg2);

    VMarg *result = new VMarg();
    result->type = VMarg::Type::label;
    if(quad->label < curr_proc_quad)
        result->value = address_links.at(quad->label).get_vm_instr_label();
    else
        incomplete_jumps.push_back(Incomplete_jump(quad->label, next_vm_instr_address()) );

    emit(VMinstruction(
            vmopcode,
            result,
            arg1,
            arg2,
            quad->lineno
    ));
}