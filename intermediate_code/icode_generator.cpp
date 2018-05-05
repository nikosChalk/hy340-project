

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

#define MAX_TABS (string("\t\t\t"))
#define MAX_TABS_COUNT ((long)(MAX_TABS).length())
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define TAB(str) (string(MAX(MAX_TABS_COUNT - (long)((str).length()/8), 1), '\t'))
std::string icode_generator::to_string() const {
    stringstream ss;
    const string splitter = "----------------------------------------------------------------------------------------------------------------";

    ss << "quad#" << "\t" << "opcode" << MAX_TABS << "result" << MAX_TABS << "arg1" << MAX_TABS << "arg2" << MAX_TABS << "label" << endl;
    ss <<  splitter << endl;

    int quadno_counter = 0;
    for(quad const *q : this->quad_vector) {
        string iopcode_str = quad::iopcode_to_str(q->opcode);
        string arg1_str = (q->arg1 == nullptr) ? "" : q->arg1->to_string();
        string arg2_str = (q->arg2 == nullptr) ? "" : q->arg2->to_string();
        string res_str = (q->result == nullptr) ? "" : q->result->to_string();;

        ss << std::to_string(quadno_counter) << ":" << "\t"
           << iopcode_str << TAB(iopcode_str)
           << res_str << TAB(res_str)
           << arg1_str << TAB(arg1_str)
           << arg2_str << TAB(arg2_str)
           << std::to_string(q->label)
           << endl;
        quadno_counter++;
    }
    ss << splitter << endl;
    return ss.str();
}

namespace intermediate_code {
    ostream &operator<<(ostream &os, const icode_generator &igen) {
        return os << igen.to_string();
    }
}