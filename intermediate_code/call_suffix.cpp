

#include "call_suffix.h"

using namespace std;
using namespace intermediate_code;

call_suffix::call_suffix(call_suffix::type suffix_type, const deque<expr*> &elist) {
    this->elist = elist;
    this->suffix_type = suffix_type;
}

call_suffix::~call_suffix() { /*noop. Just for derived classes to be considered virtual at runtime*/ }

deque<expr*>& call_suffix::get_elist() {
    return this->elist;
}

call_suffix::type call_suffix::get_type() const {
    return this->suffix_type;
}

method_call::method_call(const std::string &name, const std::deque<intermediate_code::expr *> &elist)
        : call_suffix(type::METHOD_CALL, elist) {
    this->name = string(name);
}

const std::string& method_call::get_name() const {
    return this->name;
}

norm_call::norm_call(const std::deque<intermediate_code::expr *> &elist)
        : call_suffix(type::NORM_CALL, elist) {
    /*noop*/
}

