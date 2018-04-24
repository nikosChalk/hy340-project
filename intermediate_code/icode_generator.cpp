

#include <cassert>
#include "icode_generator.h"

using namespace std;
using namespace intermediate_code;

icode_generator::icode_generator() {
    quad_vector = vector<quad*>();
}

void icode_generator::emit_quad(quad *quad) {
    assert(quad);
    quad->label=(unsigned int)quad_vector.size();
    quad_vector.push_back(quad);
}

void icode_generator::patch_label(unsigned long quadno, unsigned int label) const {
    assert(quadno < quad_vector.size());
    quad_vector[quadno]->label = label;
}