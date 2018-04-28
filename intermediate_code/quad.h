

#ifndef HY340_PROJECT_QUAD_H
#define HY340_PROJECT_QUAD_H

#include "expr.h"

namespace intermediate_code {

    /**
     * Representation of an intermediate code instruction
     */
    struct quad {
        /**
         * Available opcodes for the intermediate code instruction set
         */
        enum iopcode {
            assign,     add,        sub,            mul,            div,
            mod,        uminus,     logical_and,    logical_or,     logical_not,
            if_eq,      if_noteq,   if_lesseq,      if_greatereq,   if_less,
            if_greater,
            call,                   //arg1=function to call, arg2,result=nullptr
            param,                  //arg1=parameter, arg2,result=nullptr
            ret,
            getretval,              //result=return value, arg1,arg2=nullptr
            funcstart,  funcend,
            tablecreate,
            tablegetelem,   tablesetelem,   /*result=table[index], arg1 = table, arg2 = index*/
            jump
        };

        iopcode opcode;
        expr *result;
        expr *arg1;
        expr *arg2;

        unsigned int label; //Position within the quad array
        unsigned int lineno;

        /**
         * Creates a quad with the given attributes and with a default label equal to 0.
         * @param lineno The line in the source file which triggered the creation of the quad
         */
        quad(iopcode opcode, expr *result, expr *arg1, expr *arg2, unsigned int lineno, unsigned int label=0);
    };
};


#endif //HY340_PROJECT_QUAD_H
