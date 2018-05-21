

#include <iostream>
#include "alpha_runtime_error.h"


using namespace std;
using namespace virtual_machine;

#define MESSAGE(info_str, source_line) ("AVM Runtime Error: " + (info_str) + "\n" \
                                        + "\t Caused by source line: " + std::to_string(source_line) + "\n" \
                                        + "Execution aborted.\n")

alpha_runtime_error::alpha_runtime_error(string info, unsigned int source_line)
        : runtime_error(MESSAGE(info, source_line)) { }

alpha_runtime_error::alpha_runtime_error(const char *info, unsigned int source_line)
        : runtime_error(MESSAGE(string(info), source_line)) { }

char const* alpha_runtime_error::what() const throw() {
    return runtime_error::what();
}