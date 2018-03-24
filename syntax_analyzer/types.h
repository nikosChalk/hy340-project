

/**
 * Header file needed by Bison (parser.y)
 */

#ifndef HY340_PROJECT_TYPES_H
#define HY340_PROJECT_TYPES_H

#include <string>
#include <cstdlib>
#include <vector>

namespace syntax_analyzer {
    typedef void *void_t;
    void_t const void_value = NULL;

    /**
     * Type needed for the Value Stack of the Bison parser (LR parser.)
     */
    typedef struct {
        bool boolVal;
        int intVal;
        double realVal;
        std::string strVal;
        std::vector<std::string> strVector;
        void_t voidVal;
    } value_stack_t;

};

#endif //HY340_PROJECT_TYPES_H
