/**
 * Contains the names of the built-in library functions
 */

#ifndef HY340_PROJECT_BUILTIN_FUNCS_H
#define HY340_PROJECT_BUILTIN_FUNCS_H

#include <string>
#include <vector>

class Builtin_funcs {
public:
    static const std::string libname_print = std::string("print");
    static const std::string libname_input = std::string("print");
    static const std::string libname_objectmemberkeys = std::string("objectmemberkeys");
    static const std::string libname_objecttotalmembers = std::string("objecttotalmembers");
    static const std::string libname_objectcopy = std::string("print");
    static const std::string libname_totalarguments = std::string("print");
    static const std::string libname_argument = std::string("print");
    static const std::string libname_typeof = std::string("print");
    static const std::string libname_strtonum = std::string("print");
    static const std::string libname_sqrt = std::string("print");
    static const std::string libname_cos = std::string("print");
    static const std::string libname_sin = std::string("print");

    static const std::vector<std::string> LIB_FUNC_NAMES = std::vector<std::string> {
        libname_print, libname_input, libname_objectmemberkeys, libname_objecttotalmembers,
        libname_objectcopy, libname_totalarguments, libname_argument, libname_typeof,
        libname_strtonum, libname_sqrt, libname_cos, libname_sin
    };

private:
    /**
     * Disable constructor
     */
    Builtin_funcs() = default;
};

#endif //HY340_PROJECT_BUILTIN_FUNCS_H
