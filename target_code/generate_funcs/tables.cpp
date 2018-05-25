

#include "../VMcode_generator.h"

using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate_TABLECREATE(quad const *quad) {
    generate(VMopcode::newtable, quad);
}
void VMcode_generator::generate_TABLEGETELEM(quad const *quad) {
    generate(VMopcode::tablegetelem, quad);
}
void VMcode_generator::generate_TABLESETELEM(quad const *quad) {
    generate(VMopcode::tablesetelem, quad);
}