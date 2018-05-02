

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
            assign,                     //result=arg1, arg2=nullptr
            add, sub, mul, div, mod,    //arg1=left operand, arg2=right operand

            uminus,                     //arg1=unary operand, result=-arg1, arg2=nullptr
            logical_and, logical_or,    //arg1=lef operand, arg2=right operand. result=arg1 logical_op arg2
            logical_not,                //arg1=unary operand, result=NOT arg1, arg2=nullptr

            if_eq, if_noteq, if_lesseq, if_greatereq, if_less, if_greater,  //result=arg1 relop arg2, arg1=lef operand, arg2=right operand, label=quadno to jump to if true

            call,   //arg1=function to call, arg2,result=nullptr
            param,  //arg1=parameter, arg2,result=nullptr
            ret,    //arg1=return value, arg2,result=nullptr
            getretval,  //result=return value, arg1,arg2=nullptr
            funcstart,  funcend,

            tablecreate,
            tablegetelem,   tablesetelem,   //result=table[index], arg1 = table, arg2 = index

            jump    //result,arg1,arg2=nullptr. label=quadno to jump to.
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
