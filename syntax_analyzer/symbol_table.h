

#ifndef HY340_PROJECT_SYMBOL_TABLE_H
#define HY340_PROJECT_SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>

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
            /**
             * Variable to indicate global scoping
             */
            static const unsigned int GLOBAL_SCOPE;

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
             * Sets the symbol either visible or invisible within the symbol table.
             * @param visible True to set it visible when lookup() is invoked on a symbol table. False to set it invisible.
             */
            void set_visible(bool visible);

        protected:
            /**
             * Creates a new symbol table entry with the given attributes which is by default visible
             * @param scope The scope of the entry
             * @param line The line within the source file where it was encountered
             * @param name The entry's name. Must not be the empty string
             * @throws std::runtime_error if name is the empty string
             */
            entry(unsigned int scope, unsigned int line, const std::string &name);

        private:
            unsigned int scope, line;
            bool visible;
            std::string name;
        };

        /**
         * Symbol table entries for variables
         */
        class var_entry:public entry {
        public:
            enum sym_type {GLOBAL, LOCAL, FORMAL_ARG};

            /**
             * Same as entry class
             * @param symbol_type The variable's type.
             */
            var_entry(unsigned int scope, unsigned int line, const std::string &name, sym_type symbol_type);

            /**
             * Returns the variable's type
             * @return The variable's type
             */
            sym_type get_sym_type() const;

        private:
            sym_type symbol_type;
        };

        /**
         * Symbol table entries for functions
         */
        class func_entry:public entry {
        public:
            enum sym_type {USER_FUNC, LIB_FUNC};

            /**
             * Same as entry class.
             * @param symbol_type The function's type
             * @param arg_list The function's argument list. Each of the var_entries must have a symbol_type equal to FORMAL_ARG
             * @throws std::runtime_error if the arg_list constriction is not met
             */
            func_entry(unsigned int scope, unsigned int line, const std::string &name, sym_type symbol_type, const std::vector<var_entry> &arg_list);

            /**
             * The function's type
             * @return The function's type
             */
            sym_type get_sym_type() const;

            /**
             * Returns a reference to a vector containing symbol table entries for its FORMAL arguments
             * @return The reference to the above vector
             */
            std::vector<var_entry>& get_arg_list();
        private:
            sym_type symbol_type;
            std::vector<var_entry> arg_list;
        };


        /**
         * Creates a new symbol table with no entries
         */
        symbol_table();

        /**
         * Inserts the given symbol table entry to the symbol_table with key equal to entry.get_name()
         * and scope equal to entry.get_scope()
         * @param entry The entry to insert
         * @throws std::runtime_error if the entry already exists in its scope and that entry is visible
         */
        void insert(const entry &entry);

        /**
         * Searches the given key in the given scope. Note that symbol table entries which have been set to invisible,
         * are ignored.
         * @param key An entry.get_name() string
         * @param scope The scope to search the given key
         * @return A vector with visible symbol table entries for each match. If no visible value was found with the given key,
         * an empty vector is returned.
         */
        std::vector<entry> lookup(const std::string &key, unsigned int scope) const;

        /**
         * Sets all the symbol table entries in the given scope to invisible.
         * @param scope The scope where all the symbol table entries will be set to invisible.
         */
        void hide(unsigned int scope);

    private:
        /**
         * For each scope we have a symbol table. sym_tables[i] gives the symbol table for scope i.
         * A given symbol_table is just a hash table with names as keys and entry* as values.
         */
        std::vector<
                std::unordered_map<std::string, std::vector<entry>>
        > sym_tables;

    };
}


#endif //HY340_PROJECT_SYMBOL_TABLE_H
