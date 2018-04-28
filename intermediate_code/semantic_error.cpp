

#include "semantic_error.h"

using namespace std;
using namespace intermediate_code;


semantic_error::semantic_error(unsigned int lineno)
        : runtime_error("Semantic Error --> line: " + to_string(lineno) + ". ") {}

semantic_error::semantic_error(const string &msg, unsigned int lineno)
        : runtime_error("Semantic Error --> line: " + to_string(lineno) + ". " + msg) {}

char const *semantic_error::what() const throw() {
    return runtime_error::what();
}