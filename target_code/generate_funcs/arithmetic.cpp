

#include "../VMcode_generator.h"

using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate_ADD(quad const *quad) {
    generate(VMopcode::add, quad);
}
void VMcode_generator::generate_SUB(quad const *quad) {
    generate(VMopcode::sub, quad);
}
void VMcode_generator::generate_MUL(quad const *quad) {
    generate(VMopcode::mul, quad);
}
void VMcode_generator::generate_DIV(quad const *quad) {
    generate(VMopcode::div, quad);
}
void VMcode_generator::generate_MOD(quad const *quad) {
    generate(VMopcode::mod, quad);
}
void VMcode_generator::generate_UMINUS(quad const *quad) {
    emit(VMinstruction(
            VMopcode::mul,
            translate_operand(quad->result),
            translate_operand(quad->arg1),
            translate_number(-1),
            quad->lineno
    ));
}