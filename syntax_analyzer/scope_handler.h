

#ifndef HY340_PROJECT_SCOPE_HANDLER_H
#define HY340_PROJECT_SCOPE_HANDLER_H

#include <stack>
#include <vector>

namespace syntax_analyzer {

    class scope_handler {

    public:
        enum scope_space {
            PROGRAM_SS, FUNCTION_LOCALS_SS, FORMAL_ARGUMENTS_SS
        };

        /**
         * Variable to indicate global scoping
         */
        static const unsigned int GLOBAL_SCOPE;

        /**
         * Initializes the scope handler with the default scope space to be PROGRAM_SS
         */
        scope_handler();

        /**
         * Returns the current scope that the parser is within
         * @return The current scope
         */
        unsigned int get_current_scope() const;

        /**
         * Returns the scope of the function that the parser is within, i.e. the active function.
         * The parser is within a function if he is in FUNCTION_LOCALS_SS and NOT in FORMAL_ARGUMENTS_SS scope space.
         *
         * If the parser is not inside a function, then GLOBAL_SCOPE is returned.
         * @return The scope of the function that the parser is within, or GLOBAL_SCOPE
         */
        unsigned int get_active_function_scope() const;

        /**
         * Returns the current scope space
         * @return The current scope space
         */
        scope_space get_current_ss() const;

        /**
         * Increases the scope by 1. Note that this does NOT change the scope space.
         * @return The new scope
         */
        unsigned int increase_scope();

        /**
         * Decreases the scope by 1. Note that this does NOT change the scope space.
         * @return The new scope
         */
        unsigned int decrease_scope();

        /**
         * Fetches the offset for the current scope space and afterwards it increases it by one(1)
         * @return The offset of the current scope space before the increment, i.e. the old offset
         */
        unsigned int fetch_and_incr_cur_ssoffset();

        /**
         * Enters a function scope space and resets the function offset to zero(0).
         * Note that the scope is not changed
         */
        void enter_function_ss();

        /**
         * Exits a function scope space and restores the function offset to previous old value (if any such value exists)
         * Note that the scope is not changed
         * @return The number of local variables that were created within the function scope space that we exit
         */
        unsigned int exit_function_ss();

        /**
         * Enters a formal argument scope space and resets the formal argument offset to zero(0)
         * Note that the scope is not changed
         */
        void enter_formal_arg_ss();

        /**
         * Exits a formal argument scope space and restores the formal argument offset to previous old value (if any such value exists)
         * Note that the scope is not changed
         */
        void exit_formal_arg_ss();

        /*TODO: otan allazei to scope xrhsimopoiei mia nea list. Otan meithwei to scope adeiazei adiazei thn lista auth???*/
        void break_list_push(unsigned int quadno);  /*TODO: stabilize this function's API*/
        std::vector<unsigned int> get_break_list(); /*TODO: stabilize this function's API*/

    private:
        /**
         * Keep track of the scope that the parser is within
         */
        unsigned int scope;

        /**
         * Keeps track of the current scope space
         */
        std::stack<scope_space> ss_stack;

        /**
         * For the current scope space, keep track of the offset. The top of the stack is the next offset that should be
         * user for the top scope space in ss_offset_stack
         */
        std::stack<unsigned int> ss_offset_stack;

        /**
         * Keep track of the scope of the current open function
         */
        std::stack<unsigned int> func_scope_stack;
    };

};

#endif //HY340_PROJECT_SCOPE_HANDLER_H
