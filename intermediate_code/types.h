

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
        funcstart,  funcend,    tablecreate,    tablegetelem,   tablesetelem
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

    struct expr {
        expression_type type;
        syntax_analyzer::symbol_table::entry *sym_entry;
        expr *index;
        union {
            double number;
            std::string str;
            bool boolean;
        } const_val;

        static expr* make_lvalue_expr(syntax_analyzer::symbol_table::entry *sym_entry);
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
