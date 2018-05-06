

#include <cassert>
#include "hidden_var_handler.h"

using namespace std;
using namespace syntax_analyzer;

hidden_var_handler::hidden_var_handler() {
    count = 0;
}

symbol_table::entry* hidden_var_handler::make_new(symbol_table &sym_table, scope_handler &scope_handler, unsigned int lineno) {
    unsigned int scope = scope_handler.get_current_scope();
    string hidden_var_id = "_t" + to_string(count++);  //hidden variables are always locals
    vector<symbol_table::entry *> const &local_entries = sym_table.lookup(hidden_var_id, scope);

    if (local_entries.empty()) {
        symbol_table::entry *new_hidden_var = new symbol_table::var_entry(
                scope, lineno, hidden_var_id, symbol_table::entry::LOCAL, scope_handler.get_current_ss(), scope_handler.fetch_and_incr_cur_ssoffset()
        );
        sym_table.insert(new_hidden_var);
        return new_hidden_var;
    } else {
        assert(local_entries.size() == 1); //only one visible hidden var should exist with the hidden_var_id
        return local_entries.at(0);
    }
}

void hidden_var_handler::reset_count() {
    count = 0;
}
