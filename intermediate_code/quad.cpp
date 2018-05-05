

#include <cassert>
#include <sstream>
#include "quad.h"

using namespace std;
using namespace intermediate_code;

std::string quad::iopcode_to_str(intermediate_code::quad::iopcode iopcode) {
    switch(iopcode) {
        case iopcode::assign:   return "assign";
        case iopcode::add:      return "add";
        case iopcode::sub:      return "sub";
        case iopcode::mul:      return "mul";
        case iopcode::div:      return "div";
        case iopcode::mod:      return "mod";
        case iopcode::uminus:   return "uminus";
        case iopcode::logical_and:  return "and";
        case iopcode::logical_or:   return "or";
        case iopcode::logical_not:  return "not";
        case iopcode::if_eq:        return "if_eq";
        case iopcode::if_noteq:     return "if_noteq";
        case iopcode::if_lesseq:    return "if_lesseq";
        case iopcode::if_greatereq: return "if_greatereq";
        case iopcode::if_less:      return "if_less";
        case iopcode::if_greater:   return "if_greater";
        case iopcode::call:         return "call";
        case iopcode::param:        return "param";
        case iopcode::ret:          return "ret";
        case iopcode::getretval:    return "getretval";
        case iopcode::funcstart:    return "funcstart";
        case iopcode::funcend:      return "funcend";
        case iopcode::tablecreate:  return "tablecreate";
        case iopcode::tablegetelem: return "tablegetelem";
        case iopcode::tablesetelem: return "tablesetelem";
        case iopcode::jump:         return "jump";
        default:
            assert(false);  //unreachable statement
    }
}

quad::quad(intermediate_code::quad::iopcode opcode, intermediate_code::expr *result, intermediate_code::expr *arg1,
           intermediate_code::expr *arg2, unsigned int lineno, unsigned int label/*=0*/) {

    this->opcode = opcode;
    this->result = result;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->label = label;
    this->lineno = lineno;
}