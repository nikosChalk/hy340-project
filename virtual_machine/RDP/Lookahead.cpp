

#include <cassert>
#include <sstream>
#include "RDP.h"

using namespace std;
using namespace virtual_machine;

RDP::Lookahead::Lookahead(RDP &outer_rdp)
        : outer_rdp(outer_rdp)
{ /*nop*/ }

void RDP::Lookahead::match(Token::Type token_type) {
    if(this->is_empty) {
        next_token(token_type);
    } else
    if(token_type == this->type) {    //match success
        return;
    } else {
        stringstream ss;
        ss << "Error: Requested token type '" << Token::type_to_str(token_type) << "'";
        ss << "while expecting token type '" << Token::type_to_str(token_type) << "'" << endl;
        throw ifstream::failure(ss.str());
    }
}

Token::Value RDP::Lookahead::consume(Token::Type token_type) {
    match(token_type);

    this->is_empty = true;
    return this->value; //return the old value
}

void RDP::Lookahead::next_token(Token::Type token_type) {
    assert(this->is_empty);

    this->type = token_type;
    this->is_empty = false;
    switch(token_type) {
        case Type::UNSIGNED_INT:
            outer_rdp.ifs >> this->value.uint;
            return;
        case Type::DOUBLE_LONG:
            outer_rdp.ifs >> this->value.doublel;
            return;
        case Type::CHAR:
            outer_rdp.ifs.get(this->value.ch);
            return;
        case Type::UNSIGNED_SHORT:
            outer_rdp.ifs >> this->value.ushort;
            return;
        default:
            assert(false);  //unreachable statement
    }
}