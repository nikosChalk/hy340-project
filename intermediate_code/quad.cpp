

#include "quad.h"

using namespace intermediate_code;

quad::quad(intermediate_code::quad::iopcode opcode, intermediate_code::expr *result, intermediate_code::expr *arg1,
           intermediate_code::expr *arg2, unsigned int lineno, unsigned int label/*=0*/) {

    this->opcode = opcode;
    this->result = result;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->label = label;
    this->lineno = lineno;
}