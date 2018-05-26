

#include <cassert>
#include "../../common_interface/Constants.h"
#include "RDP.h"

using namespace std;
using namespace virtual_machine;

RDP::RDP()
    : lookahead(*this)
{
    const_pool = Constants_pool();
    instructions = vector<VMinstruction>();
    total_program_vars = 0;

    ifs.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);   //set when an exception should be thrown
}

inline void RDP::match(Token::Type token_type) { lookahead.match(token_type); }

inline Token::Value RDP::consume(Token::Type token_type) { lookahead.consume(token_type); }

void RDP::parse(const std::string &file_path) {
    ifs.open(file_path, ifstream::in | ifstream::binary);   //open binary file

    //Parse the binary file
    try {
        rule_binaryfile();  //binaryfile is the starting non-terminal symbol
    } catch(exception &ex) {    //catch any exception in order to cleanup
        ifs.close();
        throw ex;
    }

    ifs.close();
}

Constants_pool RDP::get_const_pool() const {
    return const_pool;
}
const std::vector<VMinstruction> RDP::get_instructions() const {
    return instructions;
}
unsigned int RDP::get_nr_total_program_vars() const {
    return total_program_vars;
}

void RDP::rule_binaryfile() {
    match(Token::Type::UNSIGNED_INT);
    rule_magicnumber();
    rule_arrays();
    rule_code();
    rule_nr_program_vars();
}

void RDP::rule_magicnumber() {
    if(consume(Token::Type::UNSIGNED_INT).uint != Constants::MAGICNUMBER);
        throw ifstream::failure("Invalid magic number in binary file!");
}

void RDP::rule_arrays() {
    match(Token::Type::UNSIGNED_INT);

    vector<string> readStrings = rule_strings();
    for(unsigned int i=0; i<readStrings.size(); i++)
        const_pool.register_string(readStrings.at(i));

    rule_numbers();
    rule_userfunctions();
    rule_libfunctions();
}

vector<string> RDP::rule_strings() {
    match(Token::Type::UNSIGNED_INT);
    const unsigned int total_strings = rule_total();
    vector<string> v;

    for(int i=0; i<total_strings; i++)
        v.push_back(rule_onestring());
}

unsigned int RDP::rule_total() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}

void RDP::rule_nr_program_vars() {
    total_program_vars = consume(Token::Type::UNSIGNED_INT).uint;
}

std::string RDP::rule_onestring() {
    match(Token::Type::UNSIGNED_INT);
    const unsigned int str_len = rule_strsize();
    char c_str[str_len+1];

    for(int i=0; i<str_len+1; i++)
        c_str[i] = consume(Token::Type::CHAR).ch;
    return string(c_str);
}

unsigned int RDP::rule_strsize() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}

void RDP::rule_numbers() {
    match(Token::Type::UNSIGNED_INT);
    unsigned int total_numbers = rule_total();

    for(unsigned int i=0; i<total_numbers; i++)
        const_pool.register_number(consume(Token::Type::DOUBLE_LONG).doublel);
}

void RDP::rule_userfunctions() {
    match(Token::Type::UNSIGNED_INT);
    unsigned int total_userfuncs = rule_total();

    for(unsigned int i=0;i<total_userfuncs; i++)
        const_pool.register_userfunc(rule_oneuserfunc());
}

Userfunc RDP::rule_oneuserfunc() {
    match(Token::Type::UNSIGNED_INT);
    return Userfunc(rule_address(), rule_nr_locals(), rule_id());
}

unsigned int RDP::rule_address() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}

unsigned int RDP::rule_nr_locals() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}

string RDP::rule_id() {
    match(Token::Type::UNSIGNED_INT);
    return rule_onestring();
}

void RDP::rule_libfunctions() {
    match(Token::Type::UNSIGNED_INT);
    vector<string> libfuncs = rule_strings();

    for(unsigned int i=0; i<libfuncs.size(); i++)
        const_pool.register_libfunc(libfuncs.at(i));
}

void RDP::rule_code() {
    match(Token::Type::UNSIGNED_INT);
    const unsigned int total_instructions = rule_total();

    for(unsigned int i=0; i<total_instructions; i++)
        instructions.push_back(rule_oneinstruction());
}

VMinstruction RDP::rule_oneinstruction() {
    match(Token::Type::UNSIGNED_SHORT);
    return VMinstruction(rule_vmopcode(), rule_vmarg(), rule_vmarg(), rule_vmarg(), rule_srcline());
}

VMopcode RDP::rule_vmopcode() {
    return static_cast<VMopcode>(consume(Token::Type::UNSIGNED_SHORT).ushort);
}

VMarg* RDP::rule_vmarg() {
    match(Token::Type::BOOLEAN);

    bool used = rule_vmarg_used();
    VMarg::Type type = rule_vmarg_type();
    unsigned int val = rule_vmarg_value();

    if(used)
        return new VMarg(type, val);
    else
        return nullptr;
}

bool RDP::rule_vmarg_used() {
    return consume(Token::Type::BOOLEAN).boolean;
}

VMarg::Type RDP::rule_vmarg_type() {
    return static_cast<VMarg::Type>(consume(Token::Type::UNSIGNED_SHORT).ushort);
}

unsigned int RDP::rule_vmarg_value() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}

unsigned int RDP::rule_srcline() {
    return consume(Token::Type::UNSIGNED_INT).uint;
}