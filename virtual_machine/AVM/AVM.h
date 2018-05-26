

#ifndef HY340_PROJECT_AVM_H
#define HY340_PROJECT_AVM_H

#include <vector>
#include <string>
#include <array>
#include "../../common_interface/Constants_pool.h"
#include "Memcell.h"
#include "Program_stack.h"
#include "../../common_interface/vm_types.h"
#include "../../common_interface/Builtin_funcs.h"

namespace virtual_machine {

    class AVM {
    public:

        //Declarations of pointer-to-member function types
        /**
         * Signature for library functions
         */
        using Lib_membfunc_ptr = void (AVM::*) (void);
        using Exec_membfunc_ptr = void (AVM::*) (VMinstruction const &instr);

        //Type declarations of mapping: int --> member function
        using Lib_func_map = std::map<std::string, Lib_membfunc_ptr>;   //matches library function names to callable library functions
        using Exec_func_map = std::map<unsigned int, Exec_membfunc_ptr>; //matches VMopcodes to member "execute_" functions


        /**
         * Initializes the virtual machine. Note that for each vector argument, an internal copy is made.
         * @param instructions The vector containing the instructions
         * @param numbers The constant vector for the doubles
         * @param strings The constant vector for the strings
         * @param libfuncs The constant vector for the libfuncs
         * @param userfuncs The constant vector for the userfuncs
         * @param total_program_vars The number of total program scope space variables
         */
        AVM(const std::vector<VMinstruction> &instructions, Constants_pool const &const_pool, unsigned int total_program_vars);

        /**
         * Executes the alpha binary file.
         * @throws alpha_runtime_error If something went wrong
         */
        void run();

    private:
        const std::vector<VMinstruction> instructions;
        const Constants_pool const_pool;
        Program_stack program_stack;

        Memcell ax, bx;     //general purpose registers
        Memcell retval;     //special purpose register. Used for function return value
        unsigned int pc;	//address of the current instruction to execute
        bool finished;  //whether or not the execution has finished
        const unsigned long AVM_ENDING_PC;   //value that indicates that the execution has finished and no more instructions exist


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
         * @throws alpha_runtime_error If something went wrong
         */
        void execute_cycle();

        /**************************************   execute_<VMinstruction opcode>  *************************************** */
        /* execute_<VMinstruction opcode> functions. Each function has the sole task of executing the given instruction.  */
        /* The instruction's opcode must match the function's name. All these functions alter the state of the AVM        *
        /* Implementations are in the "dispatcher" directory                                                              */
        /**************************************   ------------------------------  *************************************** */

        void execute_assign(VMinstruction const &instr);

        /**
         * Handles VMopcodes::add, sub, mul, div, mod
         */
        void execute_arithmetic(VMinstruction const &instr);

        /**
         * Handles VMopcodes::jeq, jne
         */
        void execute_equality(VMinstruction const &instr);

        /**
         * Handles VMopcodes::jle, jge, jlt, jgt
         */
        void execute_relational(VMinstruction const &instr);

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
         * Calls the library function with the given name
         * @param name The name of the library function which is a name, declared in "common_interface/Builtin_funcs.h"
         */
        void call_library_function(const std::string &name);

        /**
         * Prints a warning message to cerr concerning the AVM due to a soft error during the execution of an instruction
         * @param msg The message
         * @param source_line The source line of the instruction that caused the warning
         */
        void print_warning(const std::string &msg, unsigned int source_line) const;


        /*********************************** Built in functions *********************************** */
        /* Builtin library functions must have the prefix "libfunc_" before their actual name       */
        /* All these functions have their arguments in the stack and use "retval" register for      */
        /* return value                                                                             */
        /*********************************** ------------------ *********************************** */

        /**
         * Takes variadic arguments and prints them in cout
         */
        void libfunc_print(void);

        void libfunc_input(void);
        void libfunc_objectmemberkeys(void);
        void libfunc_objecttotalmembers(void);
        void libfunc_objectcopy(void);

        void libfunc_totalarguments(void);
        void libfunc_argument(void);

        /**
         * Takes one argument and returns its type as string representation
         */
        void libfunc_typeof(void);
        void libfunc_strtonum(void);
        void libfunc_sqrt(void);
        void libfunc_cos(void);
        void libfunc_sin(void);

        /**
         * Map that maps all Builtin_funcs::libname_x to the corresponding "libfunx_x"  member function
         */
        static const Lib_func_map name_to_libfunc_map;

        /**
         * Returns the library function that matches to the given name
         * @param name A builtin library function name, declared in "common_interface/Builtin_funcs.h"
         * @return The executable library function
         */
        static Lib_membfunc_ptr get_library_function(const std::string &name);

        /**
         * Map that maps all VMopcode:: enumerable values to the corresponding "execute_"  member function
         */
        static const Exec_func_map execute_func_map;
    };


};


#endif //HY340_PROJECT_AVM_H
