

#include "numeric_error.h"

using namespace std;
using namespace arithmetic_operations;

numeric_error::numeric_error(string &info)
        : runtime_error(info) { }

numeric_error::numeric_error(const char *info)
        : runtime_error(info) { }

char const* numeric_error::what() const throw() {
    return runtime_error::what();
}