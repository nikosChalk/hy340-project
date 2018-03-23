

#include <stdexcept>
#include "symbol_table.h"


using namespace std;

namespace syntax_analyzer {

    /* ~~~~~~ symbol_table::entry implementation ~~~~~~ */
    const unsigned int symbol_table::entry::GLOBAL_SCOPE = 0;

    symbol_table::entry::entry(unsigned int scope, unsigned int line, const string &name) {
        this->scope = scope;
        this->line = line;
        this->visible = true;
        if(name.empty())
            throw runtime_error("SymbolTable's entry name must not be the empty string");
        this->name = name;
    }

    unsigned int symbol_table::entry::get_scope() const {
        return scope;
    }

    unsigned int symbol_table::entry::get_line() const {
        return line;
    }

    bool symbol_table::entry::is_visible() const {
        return visible;
    }

    string& symbol_table::entry::get_name() {
        return name;
    }

    const string& symbol_table::entry::get_name() const {
        return name;
    }

    void symbol_table::entry::set_visible(bool visible) {
        this->visible = visible;
    }

    /* ~~~~~~ symbol_table::var_entry implementation ~~~~~~ */
    symbol_table::var_entry::var_entry(unsigned int scope, unsigned int line, const string &name, sym_type symbol_type)
        : entry(scope, line, name) {
        this->symbol_type = symbol_type;
    }

    symbol_table::var_entry::sym_type symbol_table::var_entry::get_sym_type() const {
        return this->symbol_type;
    }

    /* ~~~~~~ symbol_table::func_entry implementation ~~~~~~ */
    symbol_table::func_entry::func_entry(unsigned int scope, unsigned int line, const string &name, sym_type symbol_type,
                                         const vector<var_entry> &arg_list)
        : entry(scope, line, name) {
        this->symbol_type = symbol_type;

        for(int i=0; i<arg_list.size(); i++)
            if(arg_list[i].get_sym_type() != var_entry::sym_type::FORMAL_ARG)
                throw runtime_error("In symbol table: Function argument is not of symbol type FORMAL_ARG");
        this->arg_list = arg_list;
    }

    symbol_table::func_entry::sym_type symbol_table::func_entry::get_sym_type() const {
        return this->symbol_type;
    }

    vector<symbol_table::var_entry>& symbol_table::func_entry::get_arg_list() {
        return this->arg_list;
    }

    /* ~~~~~~ symbol_table implementation ~~~~~~ */
    symbol_table::symbol_table() {
        this->sym_tables = vector<
                unordered_map<string, vector<entry>>
        >();
    }

    void symbol_table::insert(const entry &sym_entry) {
        unsigned int scope = sym_entry.get_scope();

        /* Add symbol tables for scopes not yet encountered */
        while(this->sym_tables.size()-1 < scope)  /* Max scope is less than the entry's scope. Add symbol tables */
            this->sym_tables.push_back(unordered_map<string, vector<entry>>());

        /* Check if there is already a visible entry in scope */
        if(!(this->lookup(sym_entry.get_name(), scope).empty()))
            throw runtime_error("A visible entry with the given name (" + sym_entry.get_name() + ") already exists in that" +
                                "scope(" + to_string(sym_entry.get_scope()) + ")");

        /* Insert given entry in the symbol table */
        this->sym_tables[scope][sym_entry.get_name()].push_back(sym_entry);
    }

    vector<symbol_table::entry> symbol_table::lookup(const string &key, unsigned int scope) const {
        if(scope >= this->sym_tables.size())    /* The requested scope does not even have a symbol table */
            return vector<entry>();

        /* Get the symbol table for the given scope */
        const unordered_map<string, vector<entry>>& scope_map = this->sym_tables.at(scope);
        if(scope_map.find(key) == scope_map.end())  /* The key does not exist in the given scope */
            return vector<entry>();

        /* The key exists within the given scope. Get all visible entries */
        const vector<entry>& scope_vector = scope_map.at(key);   /* Vector with all entries which have the given key and are in the given scope */
        vector<entry> v = vector<entry>();

        for(unsigned long i=0; i<scope_vector.size(); i++)
            if(scope_vector.at(i).is_visible())
                v.push_back(scope_vector.at(i));
        return v;
    }

    std::vector<symbol_table::entry> symbol_table::recursive_lookup(const std::string &key, unsigned int scope) const {
        vector<entry> v = vector<entry>();

        while(scope >= entry::GLOBAL_SCOPE) {
            vector<entry> scope_vector = this->lookup(key, scope);
            v.insert(v.end(), scope_vector.begin(), scope_vector.end());
            scope--;
        }
        return v;
    }

    void symbol_table::hide(unsigned int scope) {
        if(scope >= this->sym_tables.size())    /* The requested scope does not even have a symbol table. Nothing to do */
            return;

        /* Symbol table for the given scope exists */
        for(auto map_iterator : this->sym_tables[scope])    /* Iterate over the entries of the symbol table */
            for(auto entries_iterator : map_iterator.second)    /* Iterate over entries with the same key */
                entries_iterator.set_visible(false);
    }
};