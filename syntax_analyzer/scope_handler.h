

#ifndef HY340_PROJECT_SCOPE_HANDLER_H
#define HY340_PROJECT_SCOPE_HANDLER_H

namespace syntax_analyzer {

    enum scope_space {
        PROGRAM_VAR, FUNCTION_LOCAL_VAR, FORMAL_ARGUMENT_VAR
    };

    /**
     * Returns the current scope that the parser is within
     * @return The current scope
     */
    unsigned int get_current_scope();

    /**
     * Returns the current scope space
     * @return The current scope space
     */
    scope_space get_current_ss();

    /**
     * Returns the old offset for the current scope spaces and afterwards it increases it by one(1)
     * @return The old offset of the current scope space
     */
    unsigned int fetch_and_incr_cur_ssoffset();

    /**
     * Enters a function scope space, resets the function offset to zero(0) and increases the scope by 1.
     * @return The new scope resulting from entering this scope space
     */
    unsigned int enter_function_ss();

    /**
     * Exits a function scope space, restores the function offset to previous old value (if any such value exists)
     * and decreases the scope by 1
     * @return The new scope resulting from exiting this scope space
     */
    unsigned int exit_function_ss();

    /**
     * Enters a formal argument scope space, resets the formal argument offset to zero(0) and increases the scope by 1.
     * @return The new scope resulting from entering this scope space
     */
    unsigned int enter_formal_arg_ss();

    /**
     * Exits a formal argument scope space, restores the formal argument offset to previous old value (if any such value exists)
     * and decreases the scope by 1
     * @return The new scope resulting from exiting this scope space
     */
    unsigned int exit_formal_arg_ss();

    /**
     * Enters into a block by increasing the scope by 1. Note that this does NOT change the scope space.
     * @return The new scope resulting from entering a block
     */
    unsigned int enter_block();

    /**
     * Exits from a block by decreasing the scope by 1. Note that this does NOT change the scope space.
     * @return The new scope resulting from exiting a block
     */
    unsigned int exit_block();

};

#endif //HY340_PROJECT_SCOPE_HANDLER_H
