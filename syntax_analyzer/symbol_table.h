

#ifndef HY340_PROJECT_SYMBOL_TABLE_H
#define HY340_PROJECT_SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "scope_handler.h"

namespace syntax_analyzer {

    /**
     * Symbol table which is construted b the syntax analyzer (Bison)
     */
    class symbol_table {
    public:

        /**
         * An abstract symbol table enty (symbol table record)
         */
        class entry {
        public:
            enum sym_type {GLOBAL, LOCAL, FORMAL_ARG, USER_FUNC, LIB_FUNC};
            enum lvalue_type{VAR, FUNC};

            /**
             * Returns the string representation of the symbol_type
             * @param symbol_type The symbole type
             * @return The symbol type's string representation
             */
            static std::string sym_type_to_string(sym_type symbol_type);

            /**
             * Performs clean up
             */
            virtual ~entry();

            /**
             * Returns the scope of this entry.
             * @return The entry's scope
             */
            unsigned int get_scope() const;

            /**
             * Returns the line of this entry
             * @return The entry's line in the source file
             */
            unsigned int get_line() const;

            /**
             * Returns if this symbol is visible within a a symbol_table
             * @return
             */
            bool is_visible() const;

            /**
             * Returns the name of this symbol
             * @return The symbol's name
             */
            std::string& get_name();

            /**
             * Returns the name of this symbol
             * @return The symbol's name
             */
            const std::string& get_name() const;

            /**
             * Returns the variable's type
             * @return The variable's type
             */
            sym_type get_sym_type() const;

            /**
             * Sets the symbol either visible or invisible within the symbol table.
             * @param visible True to set it visible when lookup() is invoked on a symbol table. False to set it invisible.
             */
            void set_visible(bool visible);

            /**
             * Returns the type of this symbol table entry as if it were an lvalue
             * @return The type of this symbol
             */
            lvalue_type get_lvalue_type() const;

        protected:
            /**
             * Creates a new symbol table entry with the given attributes which is by default visible
             * @param scope The scope of the entry
             * @param line The line within the source file where it was encountered
             * @param name The entry's name. Must not be the empty string
             * @param symbol_type The variable's type.
             * @throws std::runtime_error if name is the empty string
             */
            entry(unsigned int scope, unsigned int line, const std::string &name, sym_type symbol_type);

        private:
            entry();    /* Disable default constructor */

            unsigned int scope, line;
            bool visible;
            std::string name;
            sym_type symbol_type;
        };

        /**
         * Symbol table entries for variables
         */
        class var_entry:public entry {
        public:

            /**
             * Same as entry class
             * @param scope If scope is scope_handler::GLOBAL_SCOPE, then symbol_type is ignored and this entry has GLOBAL symbol_type
             * @param symbol_type Must be GLOBAL, LOCAL or FORMAL_ARG.
             * @param ss The variable's scope space. (Opws orizetai sto frondhsthrio)
             * @param ss_offset The variable's offset within its scope. (Opws orizetai sto frondhsthrio)
             * @throws std::runtime_error if symbol_type is not one of the above
             */
            var_entry(unsigned int scope, unsigned int line, const std::string &name, sym_type symbol_type,
                      scope_handler::scope_space ss, unsigned int ss_offset);

            /**
             * Returns the scope space of the variable
             * @return The scope space of the variable
             */
            scope_handler::scope_space get_scope_space() const;

            /**
             * Returns the variable's offset within its scope
             * @return The variable's offset within its scope
             */
            unsigned int get_scope_space_offset() const;

        private:
            unsigned int ss_offset;
            scope_handler::scope_space ss;
        };

        /**
         * Symbol table entries for functions
         */
        class func_entry:public entry {
        public:

            /**
             * Same as entry class.
             * Note that total_locals fields are set to zero (0).
             * @param name. Same as entry except that if the name is empty, then a generic name is instead used
             * @param symbol_type Must be either USER_FUNC or LIB_FUNC
             * @param iaddress The quad number of this function, i.e. its address within the intermediate code
             * @param ret_val Whether or not this function has a return value. Default is false.
             * @throws std::runtime_error if symbol_type has wrong value
             */
            func_entry(unsigned int scope, unsigned int line, const std::string &name, sym_type symbol_type,
                       unsigned int iaddress, bool ret_val=false);

            /**
             * Returns the quad number of the function, i.e. its address within the intermediate code
             * @return The quad number
             */
            unsigned int get_iaddress() const;

            /**
             * Returns the number of total local variables that this function uses
             * @return The number of total local variables
             */
            unsigned int get_total_locals() const;

            /**
             * Returns whether or not this function has a return value
             * @return True if it has. False otherwise
             */
            bool has_ret_val() const;

            /**
             * Set this function's ret_val to true, i.e. register that this function has a return value
             */
            void set_ret_val();

            /**
             * Sets the number of total local variables that this function uses
             * @param total_locals The number of total local variables
             */
            void set_total_locals(unsigned int total_locals);

        private:
            static unsigned int generic_names;

            /**
             * Quad number of the function, i.e. its address within the intermediate code
             */
            unsigned int iaddress;

            /**
             * Number of
             */
            unsigned int total_locals;

            /**
             * True if this function has a return value. False otherwise.
             */
            bool ret_val;
        };


        /**
         * Creates a new symbol table with only the library functions at scope_handler::GLOBAL_SCOPE
         */
        symbol_table();

        /**
         * Inserts the given symbol table entry to the symbol_table with key equal to entry.get_name()
         * and scope equal to entry.get_scope()
         * @param entry The entry to insert
         * @throws std::runtime_error if the entry already exists in its scope and that entry is visible
         */
        void insert(entry *entry);

        /**
         * Searches the given key in the given scope. Note that symbol table entries which have been set to invisible,
         * are ignored.
         * @param key An entry.get_name() string
         * @param scope The scope to search the given key
         * @return A vector with visible symbol table entries for each match. If no visible value was found with the given key,
         * an empty vector is returned.
         */
        std::vector<entry*> lookup(const std::string &key, unsigned int scope) const;

        /**
         * Same as lookup, except that it searches from the given scope, up to the global scope (scope_handler::GLOBAL_SCOPE)
         * @param key An entry.get_name() string
         * @param scope The scope to search the given key. Searching starts from scope and goes up to global scope (inclusive)
         * @return A vector with visible symbol table entries for each match. If no visible value was found with the given key,
         * an empty vector is returned.
         */
        std::vector<entry*> recursive_lookup(const std::string &key, unsigned int scope) const;

        /**
         * Checks whether or not there is a visible symbol_table entry for which the given key can refer/has access to
         *
         * Vlepe prosvasimothta symvolwn gia to pws orizetai oti ena symvolo (key) borei na exei prosvash (reference)
         * se kapoio hdh dhlwmeno symvolo (metavlhth or function)
         *
         * Note that if the key does not match with a visible value within the symbol table, then that key is
         * also considered unaccessible (returns false)
         * @param key The key to check for accessible entries
         * @param key_scope The key's scope
         * @param active_func_scope The scope of the function which contains the id with name "key". If no function
         * is active, scope_handler::GLOBAL_SCOPE should be passed.
         * @return The first found accessible symbol
         * @throws std::runtime_error if no accessible entry was found
         */
        entry* exists_accessible_symbol(const std::string &key, unsigned int key_scope, unsigned int active_func_scope) const;

        /**
         * Sets all the symbol table entries in the given scope to invisible.
         * @param scope The scope where all the symbol table entries will be set to invisible.
         */
        void hide(unsigned int scope);

        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream &os, const symbol_table &x);

    private:
        /**
         * For each scope we have a symbol table. sym_tables[i] gives the symbol table for scope i.
         * A given symbol_table is just a hash table with names as keys and entry* as values.
         */
        std::vector<
                std::unordered_map<std::string, std::vector<entry*>>
        > sym_tables;
    };
}


#endif //HY340_PROJECT_SYMBOL_TABLE_H
