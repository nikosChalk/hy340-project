

#include "call_suffix.h"

using namespace std;
using namespace intermediate_code;

call_suffix::call_suffix(call_suffix::type type, const deque<expr*> &elist) {
    this->elist = deque<expr*>();

    for(auto it=elist.begin(); it!=elist.end(); it++)
        this->elist.push_front(*it);
    this->type = type;
}

deque<expr*>& call_suffix::get_elist() {
    return this->elist;
}

call_suffix::type call_suffix::get_type() const {
    return this->type;
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

