/**
 * Contains the names of the built-in library functions
 */

#ifndef HY340_PROJECT_BUILTIN_FUNCS_H
#define HY340_PROJECT_BUILTIN_FUNCS_H

#include <string>
#include <vector>

class Builtin_funcs {
public:
    static const std::vector<std::string> LIB_FUNC_NAMES = std::vector<std::string> {

        "print", "input", "objectmemberkeys", "objecttotalmembers", "objectcopy",
        "totalarguments", "argument", "typeof", "strtonum", "sqrt", "cos", "sin"
    };

private:
    /**
     * Disable constructor
     */
    Builtin_funcs() = default;
};

#endif //HY340_PROJECT_BUILTIN_FUNCS_H
