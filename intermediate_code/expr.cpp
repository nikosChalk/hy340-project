

#include <cassert>
#include "types.h"

using namespace std;
using namespace intermediate_code;

expr* expr::make_lvalue_expr(syntax_analyzer::symbol_table::entry *sym_entry) {
    assert(sym_entry);
    /*TODO: implement*/
}