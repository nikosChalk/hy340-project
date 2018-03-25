

/**
 * Header file needed by Bison (parser.y)
 */

#ifndef HY340_PROJECT_TYPES_H
#define HY340_PROJECT_TYPES_H

#include <string>
#include <cstdlib>
#include <vector>
#include "symbol_table.h"

namespace syntax_analyzer {
    typedef void *void_t;
    void_t const void_value = NULL;

    /**
     * Type needed for the Value Stack of the Bison parser (LR parser.)
     */
    typedef struct {
        bool boolVal;   /*BOOL_TRUE, BOOL_FALSE*/
        int intVal;     /*CONST_INT*/
        double realVal; /*CONST_REAL */
        std::string strVal; /*CONST_STR, IDENTIFIER */
        std::vector<std::string> strVector; /*idlist*/
        symbol_table::entry::lvalue_type lvalueType;     /*lvalue*/
        void_t voidVal;
    } value_stack_t;
};

#endif //HY340_PROJECT_TYPES_H
