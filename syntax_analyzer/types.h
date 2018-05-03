

/**
 * Header file needed by Bison (parser.y)
 */

#ifndef HY340_PROJECT_SYNTAX_ANALYZER_TYPES_H
#define HY340_PROJECT_SYNTAX_ANALYZER_TYPES_H

#include <string>
#include <cstdlib>
#include <vector>
#include "symbol_table.h"
#include "../intermediate_code/types.h"

namespace syntax_analyzer {
    typedef void *void_t;
    void_t const void_value = NULL;

    /**
     * Type needed for the Value Stack of the Bison parser (LR parser.)
     */
    typedef struct {
        long intVal;                     /*CONST_INT*/
        unsigned int unsignedIntVal;    /*funcbody*/
        long double realVal;                 /*CONST_REAL */

        std::string strVal;                 /*CONST_STR, IDENTIFIER, funcname */
        std::vector<std::string> strVector; /*idlist, tmp_idlist*/

        symbol_table::func_entry *funcEntryPtr;         /* funcdef, funcprefix */

        intermediate_code::expr *exprPtr;   /*lvalue, member, primary, assignexpr, call, term, objectdef, const*/
        std::deque<intermediate_code::expr*> dequeExpr; //double ended queue. Used for elist and tmp_elist
        std::deque<std::pair<intermediate_code::expr*, intermediate_code::expr*>> dequeExprPair;    //indexed, tmp_indexed

        std::pair<intermediate_code::expr*, intermediate_code::expr*> exprPair; //indexedelem

        intermediate_code::call_suffix *callSuffixPtr;  //methodcall, normcall, callsuffix
		intermediate_code::for_prefix *forPrefixPtr;	//forprefix

        std::vector<unsigned int> uIntVector;   /*break, continue*/

        void_t voidVal;
    } value_stack_t;
};

#endif //HY340_PROJECT_SYNTAX_ANALYZER_TYPES_H
