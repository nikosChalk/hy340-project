

#ifndef HY340_PROJECT_HIDDEN_VAR_HANDLER_H
#define HY340_PROJECT_HIDDEN_VAR_HANDLER_H

#include "symbol_table.h"
#include "scope_handler.h"

namespace syntax_analyzer {

    class hidden_var_handler {
    public:

        /**
         * Creates a new hidden variable handler
         */
        hidden_var_handler();

        /**
         * Creates a new hidden variable in the current scopespace and in the current scope which does not clash
         * with a user id or a hidden function name, and inserts that variable in the symbol table.
         * If a hidden variable already exists due to a previous reset, then that hidden variable is used and no insertion
         * occurs within the symbol table.
         *
         * @param sym_table The symbol table to use.
         * @param scope_handler The scope handler to use
         * @param lineno The line of the source file which triggered the creation of the hidden variable
         * @return The symbol table entry to the hidden variable.
         */
        symbol_table::entry *make_new(symbol_table &sym_table, scope_handler &scope_handler, unsigned int lineno);

        /**
         * Resets this generator in order to recycle already produced hidden variables. A working heuristic is
         * to reset this generator whenever the grammar rule "stmt -> expr;" is reduced.
         */
        void reset_count();

    private:
        unsigned int count;
    };

}


#endif //HY340_PROJECT_HIDDEN_VAR_HANDLER_H
