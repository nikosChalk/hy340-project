

#ifndef HY340_PROJECT_VM_TYPES_H
#define HY340_PROJECT_VM_TYPES_H

#include <string>

namespace virtual_machine {

    /**
     * Target code instruction set's opcodes
     */
    enum VMopcode {
        assign,                     //stores arg1 to result (result=arg1), arg2=nullptr
        add, sub, mul, div, mod,    //arg1=left operand, arg2=right operand. result=arg1 op arg2

        jeq, jne, jle, jge, jlt, jgt,   //result=address to jump to if true, arg1=lef operand, arg2=right operand

        call,                   //arg1=function to call, arg2,result=nullptr
        pusharg,                //arg1=parameter, arg2,result=nullptr
        funcenter, funcexit,    //arg1=function that starts/ends, arg2=nullptr, result=nullptr

        newtable,               //arg1=table to create, arg2=nullptr, result=nullptr
        tablegetelem,           //result=variable to store arg1[arg2], arg1 = table, arg2 = index
        tablesetelem,           //arg1=table, arg2=index, result=value to set. That is arg1[arg2] = result

        jump,                   //result=address to jump to, arg1,arg2=nullptr
        nop                     //no fields used
    };

    struct VMarg {
        enum Type {
            label,                  //value=address of a target code instruction
            program, formal, local, //value=The memory offset in the corresponding scope space
            number,                 //value=Index in the numbers' constant pool
            string,                 //value=Index in the strings' constant pool
            boolean,                //value=1 for true, 0 for false.
            nil,                    //- (value is unused)
            userfunc,               //value=address of funcenter vm instruction
            libfunc,                //value=Index in the library constant pool
            retval                  //- (value is unused). Indicates that the value is within the "retval" register of the AVM
        };
        Type type;

        unsigned int value;

        /**
         * Default initialization. (Junk)
         */
        VMarg();

        /**
         * Initializes the VMarg with the given arguments
         */
        VMarg(Type type, unsigned int value);
    };

    struct VMinstruction {
        VMopcode  opcode;
        VMarg     result;
        VMarg     arg1;
        VMarg     arg2;
        unsigned int source_line;

        /**
         * Default initialization. (Junk)
         */
        VMinstruction();

        /**
         * Initializes the VMinstructions with the given arguments
         */
        VMinstruction(VMopcode opcode, const VMarg &result, const VMarg &arg1, const VMarg &arg2, unsigned int source_line);
    };

    /**
     * Representation of a user function within the vm
     */
    struct Userfunc {
        unsigned int address;
        unsigned int nr_locals;
        std::string  name;

        /**
         * Default initialization. (Junk)
         */
        Userfunc();

        /**
         * Initializes the user function representation with the given attributes
         * @param address The address of the "funcstart" vm instruction
         * @param nr_locals The total number of local variables used by this function
         * @param name The function's name
         */
        Userfunc(unsigned int address, unsigned int nr_locals, const std::string &name);
    };
};

#endif //HY340_PROJECT_VM_TYPES_H
