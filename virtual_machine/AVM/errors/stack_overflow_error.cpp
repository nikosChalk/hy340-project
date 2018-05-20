

#include "stack_overflow_error.h"

using namespace std;
using namespace virtual_machine;

stack_overflow_error::stack_overflow_error(string &info)
        : runtime_error(info) { }

stack_overflow_error::stack_overflow_error(const char *info)
        : runtime_error(info) { }

char const* stack_overflow_error::what() const throw() {
    return runtime_error::what();
}