

#ifndef HY340_PROJECT_AVM_H
#define HY340_PROJECT_AVM_H

#include <vector>
#include <string>
#include <array>
#include "../../common_interface/vm_types.h"
#include "Constants_pool.h"
#include "Memcell.h"
#include "Program_stack.h"

#define AVM_ENDING_PC (this->instructions.size())

namespace virtual_machine {

    class AVM {
    public:

        /**
         * Initializes the virtual machine. Note that for each vector argument, an internal copy is made.
         * @param instructions The vector containing the instructions
         * @param doubles The constant vector for the doubles
         * @param strings The constant vector for the strings
         * @param libfuncs The constant vector for the libfuncs
         * @param userfuncs The constant vector for the userfuncs
         * @param total_program_vars The number of total program scope space variables
         */
        AVM(const std::vector<VMinstruction> &instructions,
            const std::vector<double> &doubles, const std::vector<std::string> &strings,
            const std::vector<std::string> &libfuncs, const std::vector<Userfunc> &userfuncs,
            unsigned int total_program_vars);

        /**
         * Executes the alpha binary file
         */
        void run();

    private:
        std::vector<VMinstruction> instructions;
        Constants_pool const_pool;
        Program_stack program_stack;

        Memcell ax, bx, cx; //general purpose registers
        Memcell retval;     //special purpose register. Used for function return value
        unsigned int pc;	//address of the current instruction to execute

        bool finished;  //whether or not the execution has finished


        /**
         * Translates the given vmarg (possible fetched from an VMinstruction) int a memory address.
         * @param vmarg The VMarg to translate. Must not be NULL/nullptr
         * @param reg A register to store the translation's result. If null (default), the vmarg is translated
         * to a memory address within the program stack
         * @return The memory address which in which vmarg is binded (could be either inside the stack or a register)
         */
        Memcell* translate_operand(VMarg const *vmarg, Memcell *reg = nullptr);

        /**
         * Executes the instruction currently pointed by pc, effectively changing the state of the AVM
         * If no more instructions exist to execute then finished is set to 1.
         * Also if finished is already 1, then no action is taken.
         */
        void execute_cycle();

        /**
         * execute_<VMinstruction opcode> functions. Each function has the sole task of executing the given instruction.
         * The instruction's opcode must match the function's name. All these functions alter the state of the AVM
         * Implementations are in the "dispatcher" directory
         */
        void execute_assign(VMinstruction const &instr);

        void execute_add(VMinstruction const &instr);
        void execute_sub(VMinstruction const &instr);
        void execute_mul(VMinstruction const &instr);
        void execute_div(VMinstruction const &instr);
        void execute_mod(VMinstruction const &instr);

        void execute_jeq(VMinstruction const &instr);
        void execute_jne(VMinstruction const &instr);
        void execute_jle(VMinstruction const &instr);
        void execute_jge(VMinstruction const &instr);
        void execute_jlt(VMinstruction const &instr);
        void execute_jgt(VMinstruction const &instr);

        void execute_call(VMinstruction const &instr);
        void execute_pusharg(VMinstruction const &instr);
        void execute_funcenter(VMinstruction const &instr);
        void execute_funcexit(VMinstruction const &instr);

        void execute_newtable(VMinstruction const &instr);
        void execute_tablegetelem(VMinstruction const &instr);
        void execute_tablesetelem(VMinstruction const &instr);

        void execute_jump(VMinstruction const &instr);

        void execute_nop(VMinstruction const &instr);

        /**********************************   Helper functions  *********************************** */
        /* Helper functions. These functions are used by the execute_<opcode> functions.            */
        /* Their implementation can be found in the corresponding .cpp in the dispatcher directory  */
        /**********************************   ----------------  *********************************** */


        /**
         * Prints a warning message to cerr concerning the AVM due to a soft error during the execution of an instruction
         * @param msg The message
         * @param source_line The source line of the instruction that caused the warning
         */
        void print_warning(const std::string &msg, unsigned int source_line) const;

        /**
         * Type declaration of the execute functions for convenience
         */
        typedef void (AVM::*exec_func_t)(VMinstruction const &);

        /**
         * WARNING: The order of the elements of the array must be the same as the enumeration order
         * of the VMopcode
         */
        static exec_func_t const execute_functions_array[] = {
                AVM::execute_assign,

                AVM::execute_add, AVM::execute_sub, AVM::execute_mul, AVM::execute_div, AVM::execute_mod,

                AVM::execute_jeq, AVM::execute_jne, AVM::execute_jle, AVM::execute_jge, AVM::execute_jlt, AVM::execute_jgt,

                AVM::execute_call,
                AVM::execute_pusharg,
                AVM::execute_funcenter, AVM::execute_funcexit,

                AVM::execute_newtable,
                AVM::execute_tablegetelem,
                AVM::execute_tablesetelem,

                AVM::execute_jump,
                AVM::execute_nop
        };
    };


};


#endif //HY340_PROJECT_AVM_H