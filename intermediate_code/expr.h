

#ifndef HY340_PROJECT_EXPR_H
#define HY340_PROJECT_EXPR_H

#include "../syntax_analyzer/symbol_table.h"

namespace intermediate_code {
    class expr {
    public:
        enum type {
            VAR_E,
            TABLE_ITEM_E,

            PROGRAM_FUNC_E,
            LIBRARY_FUNC_E,

            ARITHM_E,
            BOOL_E,
            ASSIGN_E,
            NEW_TABLE_E,

            CONST_NUM_E,
            CONST_BOOL_E,
            CONST_STR_E,
            CONST_NIL_E
        };

        /**
         * Represents an extension for the expr that is used for short-circuit evaluation of boolean expressions
         */
        class sc_extension {
        public:

            /**
             * Initializes truelist and falselist are initialized as empty.
             */
            sc_extension();

            /**
             * Appends each entry of the given truelist to the entries of this->truelist
             * @param truelist The truelist to append
             */
            void append_to_truelist(const std::vector<unsigned int> &truelist);

            /**
             * Appends the given quadno to the entries of this->truelist
             * @param quadno The quadno to append
             */
            void append_to_truelist(unsigned int quadno);

            /**
             * Appends each entry of the given falselist to the entries of this->falselist
             * @param falselist The truelist to append
             */
            void append_to_falselist(const std::vector<unsigned int> &falselist);

            /**
             * Appends the given quadno to the entries of this->falselist
             * @param quadno The quadno to append
             */
            void append_to_falselist(unsigned int quadno);

            /**
             * Returns the truelist. The truelist is a vector of quad numbers of incomplete instructions: "if_relop var1, var2, ??" where ??
             * is the unknown label to jump to.
             * @return The truelist
             */
            const std::vector<unsigned int>& get_truelist() const;

            /**
             * Returns the falselist. The falselist is a vector of quad numbers of incomplete instructions: "jump ??" where ?? is the unknown
             * label to jump to. Those jumps are after the instructions "if_relop ..." mentioned in the get_truelist()
             * @return The falselist
             */
            const std::vector<unsigned int>& get_falselist() const;

        private:

            std::vector<unsigned int> truelist;
            std::vector<unsigned int> falselist;
        };

        type expr_type;
        syntax_analyzer::symbol_table::entry *sym_entry;
        expr *index;            //used by TABLE_ITEM_E exprs to indicate their index within the table
        struct act_as_union {   //c++ does not allow objects within unions
            long double number;
            std::string str;
            bool boolean;
        } const_val;    //field used only be make_const static functions
        sc_extension short_circ_extn;   //used by grammar rules where relational and logical operators are present

        /**
         * Creates a new expr for the given sym_entry during the reduction of the grammar rule "lvalue -> id | local id | ::id"
         * Caller is responsible to free the returned object.
         *
         * @param sym_entry The symbol table entry which is defined during the reduction of the grammar rule
         * "lvalue -> id | local id | ::id". Must not be NULL/nullptr
         * @return A pointer to the new expr which should be stored as the semantic value of the rule.
         */
        static expr* make_lvalue_expr(syntax_analyzer::symbol_table::entry *sym_entry);

        /**
         * Creates a new expr of type::TABLE_ITEM_E. This is used whenever we want a table's element as an expr*
         * Caller is responsible to free the returned object.
         *
         * @param sym_entry A symbol table entry pointing to the table
         * @param index The index that the returned expr has within the table.
         * @return A pointer to the new expr
         */
        static expr* make_table_item(syntax_analyzer::symbol_table::entry *sym_entry, expr *index);

        /**
         * Creates a new expr of type::TABLE_ITEM_E. This is used whenever we want a table's element as an expr*
         * Caller is responsible to free the returned object.
         *
         * @param sym_entry A symbol table entry pointing to the table
         * @param id The string index that the returned expr has within the table. (e.g. "x" if we have "table.x")
         * @return A pointer to the new expr
         */
        static expr* make_table_item(syntax_analyzer::symbol_table::entry *sym_entry, const std::string &id);

        /**
         * Creates a new generic expr with the given type and nullptr sym_entry and index
         * @return A pointer to the new expr
         */
        static expr* make_expr(type t);

        /**
         * Creates a new expr for the given constant num. Caller is responsible to free the returned object.
         * @param num The number
         * @return A pointer to the new expr
         */
        static expr* make_const_num(long double num);

        /**
         * Creates a new expr for the given constant string. Caller is responsible to free the returned object.
         * @param str The string
         * @return A pointer to the new expr
         */
        static expr* make_const_str(std::string const &str);

        /**
         * Creates a new expr for the given constant boolean. Caller is responsible to free the returned object.
         * @param value The boolean value
         * @return A pointer to the new expr
         */
        static expr* make_const_bool(bool value);

        /**
         * Creates a new expr for constant "nil". Caller is responsible to free the returned object.
         * @return A pointer to the new expr
         */
        static expr* make_const_nil();

        /**
         * Checks at compile time, whether or not this expr can participate in an arithmetic operation.
         * @return True if it can participate. False otherwise.
         */
        bool can_participate_in_arithmop() const;

        /**
         * Checks at compile time, whether or not this expr can participate in an relational operation, that is an
         * expr op expr where op is >, >=, <, <=
         * @return True if it can participate. False otherwise.
         */
        bool can_participate_in_relop() const;

        /**
         * Returns true if this is a expr::type::BOOL_E whose truelist or falselist is not empty and thus these lists must be patched
         * @return True if this is a expr::type::BOOL_E whose truelist or falselist is not empty. False otherwise
         */
        const bool must_be_patched() const;

        /**
         * Returns a string representation of this object
         * @return The string representation
         */
        std::string to_string() const;

    private:
        /**
         * Creates a new expr with null sym_entry and index.
         * Making of instances of this class should be done only through the static methods
         */
        expr();
    };
}

#endif //HY340_PROJECT_EXPR_H
