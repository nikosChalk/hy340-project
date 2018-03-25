

#include <stdexcept>
#include <sstream>
#include <cassert>
#include "symbol_table.h"
#include "../not_implemented_error.h"


using namespace std;

namespace syntax_analyzer {

    /* ~~~~~~ symbol_table::entry implementation ~~~~~~ */
    const unsigned int symbol_table::entry::GLOBAL_SCOPE = 0;
    unsigned int symbol_table::func_entry::generic_names = 0;

    std::string symbol_table::entry::sym_type_to_string(sym_type symbol_type) {
        if(symbol_type == GLOBAL)
            return string("global variable");
        else if(symbol_type == FORMAL_ARG)
            return string("formal argument");
        else if(symbol_type == LOCAL)
            return string("local variable");
        else if(symbol_type == USER_FUNC)
            return string("user function");
        else if(symbol_type == LIB_FUNC)
            return string("library function");
        else
            assert(false);
    }

    symbol_table::entry::entry(unsigned int scope, unsigned int line, const string &name, sym_type symbol_type) {
        this->scope = scope;
        this->line = line;
        this->visible = true;
        if(name.empty())
            throw runtime_error("SymbolTable's entry name must not be the empty string");
        this->name = name;
        this->symbol_type = symbol_type;
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

    symbol_table::entry::sym_type symbol_table::entry::get_sym_type() const {
        return this->symbol_type;
    }

    void symbol_table::entry::set_visible(bool visible) {
        this->visible = visible;
    }

    symbol_table::entry::lvalue_type symbol_table::entry::get_lvalue_type() const {
        if(this->symbol_type == sym_type::LIB_FUNC || this->symbol_type == sym_type::USER_FUNC)
            return lvalue_type::FUNC;
        else
            return lvalue_type::VAR;
    }

    /* ~~~~~~ symbol_table::var_entry implementation ~~~~~~ */
    symbol_table::var_entry::var_entry(unsigned int scope, unsigned int line, const string &name, sym_type symbol_type)
        : entry(scope, line, name, (scope == GLOBAL_SCOPE ? GLOBAL :  symbol_type)) {
        if(symbol_type != GLOBAL && symbol_type != LOCAL && symbol_type != FORMAL_ARG)
            throw std::runtime_error("Invalid symbol type");
    }

    /* ~~~~~~ symbol_table::func_entry implementation ~~~~~~ */
    symbol_table::func_entry::func_entry(unsigned int scope, unsigned int line, const string &name, sym_type symbol_type)
        : entry(scope, line, (name.empty() ? ("__gfunc" + std::to_string(generic_names++)) : name), symbol_type) {
        if(symbol_type != LIB_FUNC && symbol_type != USER_FUNC)
            throw std::runtime_error("Invalid symbol type");
    }

    /* ~~~~~~ symbol_table implementation ~~~~~~ */
    symbol_table::symbol_table() {
        this->sym_tables = vector<
                unordered_map<string, vector<entry>>
        >();

        vector<string> lib_func_names = {"print", "input", "objectmemberkeys", "objecttotalmembers", "objectcopy",
                            "totalarguments", "argument", "typeof", "strtonum", "sqrt", "cos", "sin"};

        this->sym_tables.push_back(unordered_map<string, vector<entry>>());
        for(const string &lib_func_name : lib_func_names)
            this->sym_tables.at(entry::GLOBAL_SCOPE)[lib_func_name].push_back(func_entry(
                    entry::GLOBAL_SCOPE, 0, lib_func_name, entry::LIB_FUNC
            ));
    }

    void symbol_table::insert(const entry &sym_entry) {
        unsigned int scope = sym_entry.get_scope();

        /* Add symbol tables for scopes not yet encountered */
        while(this->sym_tables.size() <= scope)  /* Max scope is less than the entry's scope. Add symbol tables */
            this->sym_tables.push_back(unordered_map<string, vector<entry>>());

        /* Check if there is already a visible entry in scope */
        if(!(this->lookup(sym_entry.get_name(), scope).empty()))
            throw runtime_error("A visible entry with the given name (" + sym_entry.get_name() + ") already exists in that" +
                                "scope(" + std::to_string(sym_entry.get_scope()) + ")");

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

    vector<symbol_table::entry> symbol_table::recursive_lookup(const string &key, unsigned int scope) const {
        vector<entry> v = vector<entry>();

        while(scope >= entry::GLOBAL_SCOPE) {
            vector<entry> scope_vector = this->lookup(key, scope);
            v.insert(v.end(), scope_vector.begin(), scope_vector.end());
            if(scope != 0)
                scope--;
            else
                break;
        }
        return v;
    }

    symbol_table::entry::lvalue_type symbol_table::exists_accessible_symbol(const string &key, unsigned int scope) const {
        bool reached_funcdef = false;

        //Search scopes until any formal args
        while(scope>0 && !reached_funcdef) {
            if(sym_tables.size() > scope) { //Check if this scope's symbol table exists
                vector<entry> scope_entries = this->lookup(key, scope);
                if (!scope_entries.empty()) {
                    return scope_entries.at(0).get_lvalue_type();
                } else {
                    //Check if we reached the formal args ==> reached function declaration
                    for (const auto &scope_pairs : this->sym_tables.at(scope))
                        for (const auto &pair_value : scope_pairs.second)
                            if (pair_value.get_sym_type() == symbol_table::entry::FORMAL_ARG)
                                reached_funcdef = true;
                }
            }
            scope--;
        }

        //Not found in outer scopes without crossing a function. Check global scope
        vector<entry> global_entries = this->lookup(key, entry::GLOBAL_SCOPE);
        if(!global_entries.empty())
            return global_entries.at(0).get_lvalue_type();

        //No accessible symbol found
        throw std::runtime_error("No accessible symbol found");
    }

    void symbol_table::hide(unsigned int scope) {
        if(scope >= this->sym_tables.size())    /* The requested scope does not even have a symbol table. Nothing to do */
            return;

        /* Symbol table for the given scope exists */
        for(auto &map_iterator : this->sym_tables.at(scope))    /* Iterate over the entries of the symbol table */
            for(auto &entries_iterator : map_iterator.second)    /* Iterate over entries with the same key */
                entries_iterator.set_visible(false);
    }

    std::string symbol_table::to_string() const {
        stringstream ss;

        for(unsigned int i=0; i<this->sym_tables.size(); i++) {
            ss << "~~~~~~~~~~~~~~~~      SCOPE #" + std::to_string(i) + "      ~~~~~~~~~~~~~~~~" << endl;
            for(const auto &pair : this->sym_tables[i]) {
                for(const auto &sym_entry : pair.second) {
                    ss << "\"" << sym_entry.get_name() << "\" [" << entry::sym_type_to_string(sym_entry.get_sym_type()) << "] "
                        << "(line " << sym_entry.get_line() << ") (scope " << sym_entry.get_scope() << ")" << endl;
                }
            }
            ss << endl;
        }
        return ss.str();
    }

    ostream& operator<<(ostream &os, const symbol_table &x) {
        return os << x.to_string();
    }
};