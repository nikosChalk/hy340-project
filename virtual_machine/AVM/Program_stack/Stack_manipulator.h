

#ifndef HY340_PROJECT_STACK_MANIPULATOR_H
#define HY340_PROJECT_STACK_MANIPULATOR_H

/* Class for the Attorney-client idiom, i.e. providing access to only some private fields/members of class Program_stack    */
/* through the usage of friendship                                                                                          */

namespace virtual_machine {
    class Program_stack;    //Forward declaration

    class Stack_manipulator {
    public:

        /**
         * Proxy for program_stack.move_up()
         */
        static void move_up(Program_stack &program_stack);

        /**
         * Proxy for program_stack.move_down()
         */
        static void move_down(Program_stack &program_stack);
    };
};


#endif //HY340_PROJECT_STACK_MANIPULATOR_H
