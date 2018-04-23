

#ifndef HY340_PROJECT_SCOPE_HANDLER_H
#define HY340_PROJECT_SCOPE_HANDLER_H

namespace syntax_analyzer {

    class scope_handler {

    public:
        enum scope_space {
            PROGRAM_SS, FUNCTION_LOCALS_SS, FORMAL_ARGUMENTS_SS
        };

        /**
         * Initializes the scope handler with the default scope space to be PRGRAM_SS
         */
        scope_handler();    //TODO: static std::stack<unsigned int> func_scope_stack = std::stack<unsigned int>();
                            //TODO: func_scope_stack.push(symbol_table::entry::GLOBAL_SCOPE);

        /**
         * Returns the current scope that the parser is within
         * @return The current scope
         */
        unsigned int get_current_scope();

        /**
         * Returns the scope of the function that the parser is within, i.e. the active function
         * If the parser is not inside a function, then GLOBAL_SCOPE is returned.
         * @return The scope of the function that the parser is within, or GLOBAL_SCOPE
         */
        unsigned int get_active_function_scope();   //TODO: func_scope_stack.top();

        /**
         * Returns the current scope space
         * @return The current scope space
         */
        scope_space get_current_ss();

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
         * Returns the old offset for the current scope space and afterwards it increases it by one(1)
         * @return The old offset of the current scope space
         */
        unsigned int fetch_and_incr_cur_ssoffset();

        /**
         * Enters a function scope space and resets the function offset to zero(0).
         * Note that the scope is not changed
         */
        void enter_function_ss();   //TODO:func_scope_stack.push(scope);

        /**
         * Exits a function scope space and restores the function offset to previous old value (if any such value exists)
         * Note that the scope is not changed
         */
        void exit_function_ss();    //TODO:func_scope_stack.pop();

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
    };

};

#endif //HY340_PROJECT_SCOPE_HANDLER_H
