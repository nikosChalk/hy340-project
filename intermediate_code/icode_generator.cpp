

#include <cassert>
#include <sstream>
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

void icode_generator::patch_label(unsigned int quadno, unsigned int label) {
    assert(quadno < this->next_quad_label());   //We cannot patch a quad's label if that quad has not been emitted
    quad_vector[quadno]->label = label;
}

void icode_generator::patch_label(const vector<unsigned int> &quadno_vector, unsigned int label) {
    for(unsigned int quadno : quadno_vector)
        patch_label(quadno, label);
}

unsigned int icode_generator::next_quad_label() const {
    return (unsigned int)quad_vector.size();
}

std::string icode_generator::to_string() const {
    stringstream ss;

    const string sp = " \t\t";
    const string splitter = "---------------------------------------------------------------------------------------------";

    ss << "quad#" << sp << "opcode" << sp << "result" << sp << "arg1" << sp << "arg2" << "label" << endl;
    ss <<  splitter << endl;
    for(quad const *q : this->quad_vector) {
        string arg1_str = (q->arg1 == nullptr) ? "" : q->arg1->to_string();
        string arg2_str = (q->arg2 == nullptr) ? "" : q->arg2->to_string();
        string res_str = (q->result == nullptr) ? "" : q->result->to_string();

        ss << quad::iopcode_to_str(q->opcode) << sp << res_str << sp
           << arg1_str << sp << arg2_str << sp << std::to_string(q->label) << endl;
    }
    ss << splitter << endl;
    return ss.str();
}

namespace intermediate_code {
    ostream &operator<<(ostream &os, const icode_generator &igen) {
        return os << igen.to_string();
    }
}