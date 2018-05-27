

#ifndef HY340_PROJECT_FUNC_HANDLER_H
#define HY340_PROJECT_FUNC_HANDLER_H

#include "symbol_table.h"

namespace syntax_analyzer {
    class Func_handler {
    public:
        Func_handler();

        void enter_function(symbol_table::func_entry *func_st_entry);

        void exit_function();

        symbol_table::func_entry* get_active_func() const;

    private:
        std::stack<symbol_table::func_entry*> func_stack;
    };

}


#endif //HY340_PROJECT_FUNC_HANDLER_H
