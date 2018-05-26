

#ifndef HY340_PROJECT_STACK_MANIPULATOR_H
#define HY340_PROJECT_STACK_MANIPULATOR_H

/* Class for the Attorney-client idiom, i.e. providing access to only some private fields/members of class Program_stack    */
/* through the usage of friendship                                                                                          */

namespace virtual_machine {
    class Program_stack;    //Forward declaration

    class Stack_manipulator {
    public:

        //TODO: remove this function if it is not needed
        //TODO: move getters to public interface?
        static int get_topsp(Program_stack const &program_stack);   //exmaple of how to have private access.

        //TODO: remove this function if it is not needed
        static void set_topsp(Program_stack &program_stack, int topsp);   //exmaple of how to have private access.
    };
};


#endif //HY340_PROJECT_STACK_MANIPULATOR_H
