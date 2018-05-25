

#include "../VMcode_generator.h"

using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate_ASSIGN(quad const *quad) {
    generate(VMopcode::assign, quad);
}
void VMcode_generator::generate_NOP(unsigned int source_line) {
    emit(VMinstruction(
            VMopcode::nop,
            nullptr,
            nullptr,
            nullptr,
            source_line
    ));
}