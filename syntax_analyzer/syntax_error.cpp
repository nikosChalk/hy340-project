

#include "syntax_error.h"

using namespace std;
using namespace syntax_analyzer;


syntax_error::syntax_error(unsigned int lineno)
        : runtime_error("Syntax Error --> line: " + to_string(lineno) + ". ") {}

syntax_error::syntax_error(const string &msg, unsigned int lineno)
        : runtime_error("Syntax Error --> line: " + to_string(lineno) + ". " + msg) {}

char const *syntax_error::what() const throw() {
    return runtime_error::what();
}