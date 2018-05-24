

#include <cassert>
#include "Token.h"

using namespace std;
using namespace virtual_machine;

Token::Token() {
    is_empty = true;
}

string Token::type_to_str(Token::Type type) {
    switch(type) {
        case Type::UNSIGNED_INT:    return "unsigned int";
        case Type::DOUBLE_LONG:     return "double long";
        case Type::CHAR:            return "char";
        case Type::UNSIGNED_SHORT:  return "unsigned short";
        default:
            assert(false);  //unreachable statement
    }
}