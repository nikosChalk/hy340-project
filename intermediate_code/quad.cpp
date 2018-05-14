

#include <cassert>
#include <sstream>
#include "quad.h"

using namespace std;
using namespace intermediate_code;

std::string quad::iopcode_to_str(intermediate_code::quad::iopcode iopcode) {
    switch(iopcode) {
	case quad::iopcode::assign:   return "assign";
	case quad::iopcode::add:      return "add";
	case quad::iopcode::sub:      return "sub";
	case quad::iopcode::mul:      return "mul";
	case quad::iopcode::div:      return "div";
	case quad::iopcode::mod:      return "mod";
	case quad::iopcode::uminus:   return "uminus";
	case quad::iopcode::logical_and:  return "and";
	case quad::iopcode::logical_or:   return "or";
	case quad::iopcode::logical_not:  return "not";
	case quad::iopcode::if_eq:        return "if_eq";
	case quad::iopcode::if_noteq:     return "if_noteq";
	case quad::iopcode::if_lesseq:    return "if_lesseq";
	case quad::iopcode::if_greatereq: return "if_greatereq";
	case quad::iopcode::if_less:      return "if_less";
	case quad::iopcode::if_greater:   return "if_greater";
	case quad::iopcode::call:         return "call";
	case quad::iopcode::param:        return "param";
	case quad::iopcode::ret:          return "ret";
	case quad::iopcode::getretval:    return "getretval";
	case quad::iopcode::funcstart:    return "funcstart";
	case quad::iopcode::funcend:      return "funcend";
	case quad::iopcode::tablecreate:  return "tablecreate";
	case quad::iopcode::tablegetelem: return "tablegetelem";
	case quad::iopcode::tablesetelem: return "tablesetelem";
	case quad::iopcode::jump:         return "jump";
	case quad::iopcode::nop:          return "nop";
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