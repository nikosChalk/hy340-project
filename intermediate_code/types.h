

#ifndef HY340_PROJECT_INTERMEDIATE_TYPES_H
#define HY340_PROJECT_INTERMEDIATE_TYPES_H

#include "../syntax_analyzer/symbol_table.h"

namespace intermediate_code {

    /**
     * Available opcodes for the intermediate code instruction set
     */
    enum iopcode {
        assign,     add,        sub,            mul,            div,
        mod,        uminus,     logical_and,    logical_or,     logical_not,
        if_eq,      if_noteq,   if_lesseq,      if_greatereq,   if_less,
        if_greater, call,       param,          ret,            getretval,
        funcstart,  funcend,    tablecreate,
        tablegetelem,   tablesetelem,   /*arg1 = table, arg2 = index*/
		jump
    };

    enum expression_type {
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

		NIL_E
    };

    class expr {
    public:
        expression_type type;
        syntax_analyzer::symbol_table::entry *sym_entry;
        expr *index;            //used by TABLE_ITEM_E exprs to indicate their index within the table
        struct act_as_union {   //c++ does not allow objects within unions
            double number;
            std::string str;
            bool boolean;
            bool is_nil;
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
        static expr* make_expr(expression_type type);

        /**
         * Creates a new expr for the given constant string. Caller is responsible to free the returned object.
         * @param str The string
         * @return A pointer to the new expr
         */
        static expr* make_const_str(std::string const &str);

    private:
        /**
         * Creates a new expr with null sym_entry and index
         */
        expr();
    };

    /**
     * Representation of an intermediate code instruction
     */
    struct quad {
        iopcode opcode;
        expr *result;
        expr *arg1;
        expr *arg2;

        unsigned int label; //Position within the quad array
        unsigned int lineno;

        /**
         * Creates a quad with the given attributes and with a default label equal to 0.
         * @param lineno The line in the source file which triggered the creation of the quad
         */
        quad(iopcode opcode, expr *result, expr *arg1, expr *arg2, unsigned int lineno, unsigned int label=0);
    };
};


#endif //HY340_PROJECT_INTERMEDIATE_TYPES_H
