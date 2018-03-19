

/**
 * Header file needed by Bison (parser.y)
 */

#ifndef HY340_PROJECT_TYPES_H
#define HY340_PROJECT_TYPES_H

#include <string>
#include <cstdlib>

namespace syntax_analyzer {
    typedef void *void_t;
    void_t const void_value = NULL;

/**
 * Type needed for the Value Stack of the Bison parser (LR parser.)
 */
    union YYSTYPE {
        bool boolVal;
        int intVal;
        double realVal;
        std::string strVal;
        void_t voidVal;
    };

};

#endif //HY340_PROJECT_TYPES_H
