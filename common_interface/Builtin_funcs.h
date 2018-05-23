/**
 * Contains the names of the built-in library functions
 */

#ifndef HY340_PROJECT_BUILTIN_FUNCS_H
#define HY340_PROJECT_BUILTIN_FUNCS_H

#include <string>
#include <vector>

class Builtin_funcs {
public:

    /**
     * Disable constructor
     */
    Builtin_funcs() = delete;

    static const std::string libname_print;
    static const std::string libname_input;
    static const std::string libname_objectmemberkeys;
    static const std::string libname_objecttotalmembers;
    static const std::string libname_objectcopy;
    static const std::string libname_totalarguments;
    static const std::string libname_argument;
    static const std::string libname_typeof;
    static const std::string libname_strtonum;
    static const std::string libname_sqrt;
    static const std::string libname_cos;
    static const std::string libname_sin;

    /**
     * Vector that contains all of the above library function names
     */
    static const std::vector<std::string> LIB_FUNC_NAMES;
};

#endif //HY340_PROJECT_BUILTIN_FUNCS_H
