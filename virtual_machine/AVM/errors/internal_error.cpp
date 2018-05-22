

#include "internal_error.h"

using namespace std;
using namespace virtual_machine;

internal_error::internal_error(string info)
        : runtime_error(info) { }

internal_error::internal_error(const char *info)
        : runtime_error(info) { }

char const* internal_error::what() const throw() {
    return runtime_error::what();
}