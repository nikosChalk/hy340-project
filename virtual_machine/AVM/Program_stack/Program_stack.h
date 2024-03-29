
/* ************************ STACK REPRESENTATION ************************************** */
/*                      stack[0xFFF] (High Addresses)                                   */
/*                      _____________________________                                   */
/*                     [            var0             ] M==total program variables       */
/*                     [Program Scope Space Variables]                                  */
/*                     [           varM-1            ]                                  */
/*                                                                                      */
/* -------------------    Function Activations Records -------------------              */
/* -------------------     Caller's Activation Record  -------------------              */
/*                                                                                      */
/*                     [Caller's First Local (if any)] <-----------  saved topsp        */
/*                     [            ...              ]                                  */
/*                     [Caller's Last  Local (if any)]                                  */
/*                                                                                      */
/*                     [            Xn-1             ] Argument <--- saved top          */
/*                     [            ..               ] Argument                         */
/*                     [            X1               ] Argument                         */
/*                     [            X0               ] Argument                         */
/*               -----  Saved Environment (by caller) -----                             */
/*                     [            n                ] Total ACTUAL Arguments           */
/*                     [          pc+1               ] Address of next instr after call */
/*                     [         saved top           ] Saved Top                        */
/*                     [        saved topsp          ] Saved topsp                      */
/* -------------------     Callee's Activation Record  -------------------              */
/*                     [          local y0           ] <--- topsp                       */
/*                     [            ...              ]                                  */
/*                     [          local yK-1         ] K==callee's total locals         */
/*                     [         (free cell)         ] <--- top                         */
/*                     [         (free cell)         ]                                  */
/*                     [            ...              ]                                  */
/*                     [_____________________________]                                  */
/*                                                                                      */
/*                      stack[0x000] (Low Addresses)                                    */
/*                                                                                      */
/* Side Note: Actual arguments can be more or less than the declared formal arguments   */


#ifndef HY340_PROJECT_PROGRAM_STACK_H
#define HY340_PROJECT_PROGRAM_STACK_H

#include <array>
#include <stack>
#include "../Memcell.h"
#include "Stack_manipulator.h"

namespace virtual_machine {

    class Program_stack {
    public:

        /**
         * Initializes the program's stack
         * @param total_program_vars The number of total program scope space variables used by the alpha program
         */
        explicit Program_stack(unsigned int total_program_vars);

        /**
         * Allocates n memcell in the stack
         * @throws internal_error If the allocation would cause a stack overflow
         */
        void allocate_memcells(unsigned int n);

        /**
         * Saves the environment, as described in the beginning of the file, in the stack.
         * This function should be called by the AVM before entering a function
         * Also increases the function call depth by 1
         * @param pc The current pc, i.e. the one that triggered the save_environment()
         *
         * @throws internal_error If the saving the environment would case a stack overflow
         * If an overflow occurs, the stack's content is undefined
         */
        void save_environment(unsigned int pc);

        /**
         * Restores a previously saved environment and removes it from the stack.
         * This function should be called by the AVM whenever it exits a function
         * Also decreases the function call depth by 1
         *
         * @return The saved pc plus 1 (pc+1)
         */
        unsigned int restore_environment();

        /**
         * Returns the function call depth, i.e. how many functions are currently active
         * @return The total number of currently active functions
         */
        unsigned int get_fcall_depth() const;

        /**
         * Allocates 1 memcell and pushed the given argument
         * This function should be called when the AVM wants to push a function actual argument
         * @param actual_arg The argument to push. Must not be NULL/nullptr
         *
         * @throws internal_error If pushing this argument would cause a stack overflow
         */
        void push_actual_arg(Memcell const *actual_arg);

        /**
         * Returns the program variable at the given offset
         * @param offset The offset in the program scope space
         * @return The program variable which is within the ALLOCATED stack
         */
        Memcell* get_program_var(unsigned int offset);
        const Memcell* get_program_var(unsigned int offset) const;

        /**
         * Returns the local function variable at the given offset of the current active stack frame
         * @param offset The offset in the function's local scope space
         * @return The local variable which is within the ALLOCATED stack
         */
        Memcell* get_local_var(unsigned int offset);
        const Memcell* get_local_var(unsigned int offset) const;

        /**
         * Returns the actual argument at the given offset of the current active stack frame
         * @param offset The offset in the formals scope space, or the actual argument's offset within this stack
         * @return The actual argument which is within the ALLOCATED stack
         */
        Memcell* get_actual_arg(unsigned int offset);
        const Memcell* get_actual_arg(unsigned int offset) const;

        /**
         * Returns the total actual arguments of the current active stack frame
         * @return The total actual arguments
         */
        unsigned int get_total_actuals() const;

        /**
         * Checks whether or not the given memcell belongs to the stack, i.e. it is within the ALLOCATED stack
         * @param memcell The memcell to check
         * @return True if the memcell belongs to the stack. False otherwise
         */
        bool is_in_stack(Memcell const *memcell) const;

    private:
        friend Stack_manipulator;   //Attorney-client idiom

        /**
         * Stack's maximum memcells
         */
        static const unsigned int AVM_STACK_SIZE = 4096;

        /**
         * Saved environment size, counted in memcells
         */
        static const unsigned int AVM_ENV_SIZE = 4;

        /**
         * Offsets from "topsp" index for the corresponding environmental values
         */
        static const int SAVED_TOPSP_OFFSET = +1;
        static const int SAVED_TOP_OFFSET = +2;
        static const int SAVED_PC_OFFSET = +3;
        static const int SAVED_TOTAL_ACTUALS = +4;


        /**
         * Register that points to the next available (free) memcell within the stack
         */
        int top;

        /**
         * Register that points to the first local variable of the active function
         * If we are not inside a function, topsp==top
         */
        int topsp;

        unsigned int total_program_vars;
        unsigned int total_actuals; //counter of total actual arguments when calling a function
        unsigned int fcall_depth;   //depth of function calls, i.e. how many functions are currently active

        /**
         * The program's stack.
         * Its semantics are defined by the runtime environment and are documented at the beginning of this file.
         */
        std::array<Memcell, AVM_STACK_SIZE> stack;

        /**
         * This stack is solely used by move_up() and move_down() functions for implementation purpose
         * When we move up we push <top, topsp> and when we move down we pop()
         */
        std::stack<std::pair<int, int>> saved_pointers_stack;

        /**
         * Returns the environmental value at the given index
         * @param idx The index
         * @return The environmental value stored at that index, i.e. stack[idx]
         */
        unsigned int get_env_value(unsigned int idx) const;

        /**
         * Returns the the index where the the program variable can be found within the ALLOCATED stack for the given
         * program scope space offset
         * @param offset The offset in the program scope space
         * @return The index within the ALLOCATED stack, where the program variable can be found
         */
        unsigned long get_program_var_index(unsigned int offset) const;

        /**
         * Returns the index where the local function variable can be found within the ALLOCATED stack for the given
         * offset of the current active stack frame
         * @param offset The offset in the function's local scope space
         * @return The index within the ALLOCATED stack, where the local variable can be found
         */
        unsigned long get_local_var_index(unsigned int offset) const;

        /**
         * Returns the index where the actual argument can be found within the ALLOCATED stack for the given
         * offset of the current active stack frame
         * @param offset The offset in the formals scope space, or the actual argument's offset within this stack
         * @return The index within the ALLOCATED stack, where the actual argument can be found
         */
        unsigned long get_actual_arg_index(unsigned int offset) const;

        /**
         * Move one activation record up, i.e. to one previous function. This changes top and topsp but does not
         * restore the environment. It can be used with combination of e.g. get_actual_arg() in order to get an argument
         * of the previously called function.
         * Note that save/call environment cannot be invoked when we are not in the most recent active function stack frame
         */
        void move_up();

        /**
         * Move one activation record down, i.e. to one more recently called function. This changes top and topsp but does not
         * save or restore the environment. It can be used after the move_up() function
         * Note that save/call environment cannot be invoked when we are not in the most recent active function stack frame
         */
        void move_down();
    };

}


#endif //HY340_PROJECT_PROGRAM_STACK_H
