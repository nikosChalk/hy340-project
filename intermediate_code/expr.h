

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

        type type;
        syntax_analyzer::symbol_table::entry *sym_entry;
        expr *index;            //used by TABLE_ITEM_E exprs to indicate their index within the table
        struct act_as_union {   //c++ does not allow objects within unions
            long double number;
            std::string str;
            bool boolean;
        } const_val;    //field used only be make_const static functions

        /**
         * Creates a new expr for the given sym_entry during the reduction of the grammar rule "lvalue -> ..."
         * Caller is responsible to free the returned object.
         *
         * @param sym_entry The symbol table entry which is defined during the reduction of the grammar rule
         * "lvalue -> id | local id | ::id". Must not be NULL/nullptr
         * @return A pointer to the new expr which should be stored as the semantic value of the rule.
         */
        static expr* make_lvalue_expr(syntax_analyzer::symbol_table::entry *sym_entry);

        /**
         * Creates a new generic expr with the given type and nullptr sym_entry and index
         * @return A pointer to the new expr
         */
        static expr* make_expr(type type);

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
         * Checks at compile time, whether or not this expr can participate in an relational operation.
         * @return True if it can participate. False otherwise.
         */
        bool can_participate_in_relop() const;

    private:
        /**
         * Creates a new expr with null sym_entry and index.
         * Making of instances of this class should be done only through the static methods
         */
        expr();
    };
}

#endif //HY340_PROJECT_EXPR_H
