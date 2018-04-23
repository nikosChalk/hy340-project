

#include <cassert>
#include "icode_generator.h"

using namespace std;
using namespace intermediate_code;

icode_generator::icode_generator() {
    quad_vector = vector<quad*>();
}

void icode_generator::emit_quad(quad *quad) {
    assert(quad);
    quad_vector.push_back(quad);
}

unsigned long icode_generator::total_quads() const {
    return quad_vector.size();
}

unsigned long icode_generator::next_quad_label() const {
    /*TODO:???????? what is ths?*/
    return quad_vector.size();
}

void icode_generator::patch_label(unsigned long quadno, unsigned int label) const {
    assert(quadno < total_quads());
    quad_vector[quadno]->label = label;
}