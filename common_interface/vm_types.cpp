

#include <cassert>
#include <sstream>
#include "vm_types.h"

using namespace std;
using namespace virtual_machine;

std::string virtual_machine::vmopcode_to_str(VMopcode opcode) {
    switch(opcode) {
        case VMopcode::assign:  return "assign";
        case VMopcode::add:     return "add";
        case VMopcode::sub:     return "sub";
        case VMopcode::mul:     return "mul";
        case VMopcode::div:     return "div";
        case VMopcode::mod:     return "mod";
        case VMopcode::jeq:     return "jeq";
        case VMopcode::jne:     return "jne";
        case VMopcode::jle:     return "jle";
        case VMopcode::jge:     return "jge";
        case VMopcode::jlt:     return "jlt";
        case VMopcode::jgt:     return "jgt";
        case VMopcode::call:            return "call";
        case VMopcode::pusharg:         return "pusharg";
        case VMopcode::funcenter:       return "funcenter";
        case VMopcode::funcexit:        return "funcexit";
        case VMopcode::newtable:        return "newtable";
        case VMopcode::tablegetelem:    return "tablegetelem";
        case VMopcode::tablesetelem:    return "tablesetelem";
        case VMopcode::jump:            return "jump";
        case VMopcode::nop:             return "nop";
        default:
            assert(false);  //unreachable statement
    }
};

/* ~~~~~~~~~~~~~~ VMarg ~~~~~~~~~~~~~~ */

VMarg::VMarg(virtual_machine::VMarg::Type type, unsigned int value)
        : type(type), value(value) {
    if(type == boolean)
        assert(value == 0 || value == 1);   //forbid other values for type of boolean
}

VMarg::VMarg() = default;

std::string VMarg::type_to_string(VMarg::Type type) {
    switch(type) {
        case Type::label:       return "label";
        case Type::program:     return "program";
        case Type::formal:      return "formal";
        case Type::local:       return "local";
        case Type::number:      return "number";
        case Type::string:      return "string";
        case Type::boolean:     return "boolean";
        case Type::nil:         return "nil";
        case Type::userfunc:    return "userfunc";
        case Type::libfunc:     return "libfunc";
        case Type::retval:      return "retval";
        default:
            assert(0);  //unreachable statement
    }
}

std::string VMarg::to_string() const {
    stringstream ss;
    ss << VMarg::type_to_string(type) << " " << value;
    return ss.str();
}

/* ~~~~~~~~~~~~~~ VMinstruction ~~~~~~~~~~~~~~ */

VMinstruction::VMinstruction(VMopcode opcode, VMarg *result, VMarg *arg1, VMarg *arg2, unsigned int source_line)
        :  opcode(opcode), result(result), arg1(arg1), arg2(arg2), source_line(source_line) { }

VMinstruction::VMinstruction() {
    opcode = add;   //uses all 3 fields, so it is very likely that an assertion will be hit
    result = arg1 = arg2 = nullptr;
}

std::string VMinstruction::to_string() const {
    stringstream ss;
    ss << "Opcode: " << vmopcode_to_str(opcode) << " ";
    ss << "arg1: {" << ((arg1) ? arg1->to_string() : "nullptr") << "}, ";
    ss << "arg2: {" << ((arg2) ? arg2->to_string() : "nullptr") << "}, ";
    ss << "result: {" << ((result) ? result->to_string() : "nullptr") << "}, ";
    ss << "source line: " << source_line;
    return ss.str();
}

#define MAX_TABS (string("\t\t\t"))
#define MAX_TABS_COUNT ((long)(MAX_TABS).length())
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define TAB(str) (string(MAX(MAX_TABS_COUNT - (long)((str).length()/8), 1), '\t'))
string VMinstruction::to_string(vector<VMinstruction> const &vmi_vector) {
        stringstream ss;
        const string splitter_end = "----------------------------------------------------------------------------------------------------------------";
        const string splitter_start = "------------------------------------- VM instruction vector ----------------------------------------------------";

        ss << "vm instr#" << "\t" << "vmopcode" << MAX_TABS << "result" << MAX_TABS << "arg1" << MAX_TABS << "arg2" << MAX_TABS << "src line" << endl;
        ss <<  splitter_start << endl;

        for(unsigned int i=0; i<vmi_vector.size(); i++) {
            VMinstruction const &vmi = vmi_vector.at(i);

            string iopcode_str = virtual_machine::vmopcode_to_str(vmi.opcode);
            string arg1_str = (vmi.arg1 == nullptr) ? "" : vmi.arg1->to_string();
            string arg2_str = (vmi.arg2 == nullptr) ? "" : vmi.arg2->to_string();
            string res_str = (vmi.result == nullptr) ? "" : vmi.result->to_string();;

            ss << std::to_string(i+1) << ":" << "\t"
               << iopcode_str << TAB(iopcode_str)
               << res_str << TAB(res_str)
               << arg1_str << TAB(arg1_str)
               << arg2_str << TAB(arg2_str)
               << std::to_string(vmi.source_line)
               << endl;
        }
        ss << splitter_end << endl;
        return ss.str();
}


/* ~~~~~~~~~~~~~~ Userfunc ~~~~~~~~~~~~~~ */

Userfunc::Userfunc(unsigned int address, unsigned int nr_locals, const std::string &name)
        : address(address), nr_locals(nr_locals), name(name) { }

Userfunc::Userfunc() = default;

bool Userfunc::operator==(const virtual_machine::Userfunc &other) const {
    if(this == &other)  //check if they are pointing to the same memory
        return true;

    //Perform comparison
    return (this->address == other.address);
}

bool Userfunc::operator!=(const virtual_machine::Userfunc &other) const {
    return !(*this == other);
}