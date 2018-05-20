
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
/*                     [       saved top+n+2         ] Saved Top                        */
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
#include "Memcell.h"

#define AVM_STACK_SIZE 4096 //Stack's maximum memcells
#define AVM_ENV_SIZE 4      //Saved environment size, counted in memcells

namespace virtual_machine {
    class Program_stack {
    public:

        /**
         * Initializes the program's stack
         * @param total_program_vars The number of total program scope space variables used by the alpha program
         */
        explicit Program_stack(unsigned int total_program_vars);

        /**
         * Returns the program variable at the given offset
         * @param offset The offset in the program scope space
         * @return The program variable which is within the ALLOCATED stack
         */
        Memcell* get_program_var(unsigned int offset);

        /**
         * Returns the local function variable at the given offset
         * @param offset The offset in the function's local scope space
         * @return The local variable which is within the ALLOCATED stack
         */
        Memcell* get_local_var(unsigned int offset);

        /**
         * Returns the formal argument at the given offset
         * @param offset The offset in the formals scope space
         * @return The formal argument which is within the ALLOCATED stack
         */
        Memcell* get_formal_arg(unsigned int offset);

        /**
         * Checks whether or not the given memcell belongs to the stack, i.e. it is within the ALLOCATED stack
         * @param memcell The memcell to check
         * @return True if the memcell belongs to the stack. False otherwise
         */
        bool is_in_stack(Memcell const *memcell) const;

        /**
         * Allocates one memcell in the stack
         */
         //Function's name as given in the slides is: "decrease_top()"
        //void allocate_memcell();    //Internally it decreases top by 1. //TODO: Verify that this function is needed.

        /**
         * Saves the environment in the stack.
         */
        //void save_environment(); //TODO: verify the signature

    private:

        /**
         * Register that points to the next available (free) memcell within the stack
         */
        unsigned int top;

        /**
         * Register that points to the first local variable of the active function
         * If we are not inside a function, topsp==top
         */
        unsigned int topsp;
        unsigned int total_program_vars;

        /**
         * The program's stack.
         * Its semantics are defined by the runtime environment and are documented at the beginning of this file.
         */
        std::array<Memcell, AVM_STACK_SIZE> stack;
    };

}


#endif //HY340_PROJECT_PROGRAM_STACK_H
