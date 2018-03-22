//
// Created by nikos on 3/20/18.
//

#include "not_implemented_error.h"

using namespace std;

not_implemented_error::not_implemented_error() : logic_error("Not Implemented yet!") { }
not_implemented_error::not_implemented_error(string &msg) : logic_error(msg) { }

char const* not_implemented_error::what() const throw() {
    return logic_error::what();
}
