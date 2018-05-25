

#include "../VMcode_generator.h"

using namespace std;
using namespace target_code;
using namespace intermediate_code;
using namespace virtual_machine;

void VMcode_generator::generate_JUMP(quad const *quad) {
    generate_relational(VMopcode::jump, quad);
}
void VMcode_generator::generate_IF_EQ(quad const *quad) {
    generate_relational(VMopcode::jeq, quad);
}
void VMcode_generator::generate_IF_NOTEQ(quad const *quad) {
    generate_relational(VMopcode::jne, quad);
}
void VMcode_generator::generate_IF_GREATEREQ(quad const *quad) {
    generate_relational(VMopcode::jge, quad);
}
void VMcode_generator::generate_IF_LESSEQ(quad const *quad) {
    generate_relational(VMopcode::jle, quad);
}
void VMcode_generator::generate_IF_GREATER(quad const *quad) {
    generate_relational(VMopcode::jgt, quad);
}
void VMcode_generator::generate_IF_LESS(quad const *quad) {
    generate_relational(VMopcode::jlt, quad);
}