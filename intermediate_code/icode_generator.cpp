

#include <cassert>
#include "icode_generator.h"

using namespace std;
using namespace intermediate_code;

icode_generator::icode_generator() {
    quad_vector = vector<quad*>();
}

void icode_generator::emit_quad(quad *quad) {
    emit_quad(quad, this->next_quad_label());
}

void icode_generator::emit_quad(intermediate_code::quad *quad, unsigned int label) {
    assert(quad);
    quad->label = label;
    quad_vector.push_back(quad);
}

void icode_generator::patch_label(unsigned long quadno, unsigned int label) {
    assert(quadno < this->next_quad_label());   //We cannot patch a quad's label if that quad has not been emitted
    quad_vector[quadno]->label = label;
}

unsigned int icode_generator::next_quad_label() const {
    return (unsigned int)quad_vector.size();
}