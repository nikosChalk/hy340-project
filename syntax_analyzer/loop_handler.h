

#ifndef HY340_PROJECT_LOOP_HANDLER_H
#define HY340_PROJECT_LOOP_HANDLER_H

#include <stack>
#include <vector>

namespace syntax_analyzer {
    class loop_handler {
    public:

        /**
         * Initializes the loop handler
         */
        loop_handler();

        /**
         * Increases the loop depth by 1
         * @return The new loop depth
         */
        unsigned int enter_loop();

        /**
         * Decreases the loop depth by 1
         * @return The new loop depth
         *
         * @throws std::logic_error if the parser is not in a loop, i.e. is_in_loop() would return false
         */
        unsigned int exit_loop();

        /**
         * Returns the current loop depth, i.e. the depth within a nested loop. A value of 0 indicates that we are not in a loop.
         * Note that when entering a function, the loop depth is saved and reset to 0.
         * When exiting a function, the loop depth is reset to its previous value.
         * @return The current loop depth
         */
        unsigned int get_current_depth();

        /**
         * Enters a function scope space and effectively resets the loop depth to zero(0).
         */
        void enter_function_ss();

        /**
         * Exits a function scope space and restores the loop depth to previous old value (if any such value exists)
         * @throws std::logic_error if we are not within a function, i.e. enter_function_ss() was not previously called
         */
        void exit_function_ss();

        /**
         * Returns a vector for the current loop depth, which contains quad numbers of in-complete jumps due
         * to "break;" statements that must be patched.
         * @return The vector with the quad numbers
         *
         * @throws std::logic_error if the parser is not in a loop, i.e. is_in_loop() would return false
         */
        std::vector<unsigned int> get_break_list() const;

        /**
         * Returns a vector for the current loop depth, which contains quad numbers of in-complete jumps due
         * to "continue;" statements that must be patched.
         * @return The vector with the quad numbers
         *
         * @throws std::logic_error if the parser is not in a loop, i.e. is_in_loop() would return false
         */
        std::vector<unsigned int> get_continue_list() const;


        /**
         * Appends the given quadno to the end of the break list for the current loop depth. The instruction at the
         * given quadno is considered an in-complete jump due to to a "break;" statement that was encountered.
         * @param quadno The quad number of the in-complete jump that was emitted due to a "break;" statement
         *
         * @throws std::logic_error if the parser is not in a loop, i.e. is_in_loop() would return false
         */
        void append_to_break_list(unsigned int quadno);

        /**
         * Appends the given quadno to the end of the continue list for the current loop depth. The instruction at the
         * given quadno is considered an in-complete jump due to to a "continue;" statement that was encountered.
         * @param quadno The quad number of the in-complete jump that was emitted due to a "continue;" statement
         *
         * @throws std::logic_error if the parser is not in a loop, i.e. is_in_loop() would return false
         */
        void append_to_continue_list(unsigned int quadno);

        /**
         * Checks whether or not the parser is inside a loop, i.e. the current loop depth is greater than zero
         * @return True iff we are inside a loop. Otherwise, false
         */
        bool is_in_loop() const;

    private:
        std::stack<unsigned int> loop_depth_stack;  //keeps track the depth of the nested looping for each function.
        std::stack<std::vector<unsigned int>> break_list_stack, continue_list_stack;
    };
};

#endif //HY340_PROJECT_LOOP_HANDLER_H
