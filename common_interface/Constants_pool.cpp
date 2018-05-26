

#include <cassert>
#include "Constants_pool.h"

using namespace std;
using namespace virtual_machine;

/**
 * Asserts that the given (possibly auto generated key) does not exist within the given map
 */
#define assert_non_existent_key(map, key) assert((map).find(key) == (map).end())

Constants_pool::Constants_pool() {
    numbers = map<unsigned int, long double>();
    strings = map<unsigned int, string>();
    libfuncs = map<unsigned int, string>();
    idx_to_userfuncs = map<unsigned int, Userfunc>();
    addr_to_userfuncs = map<unsigned int, Userfunc const*>();
}

double long Constants_pool::get_numeric(unsigned int idx) const{
	return this->numbers.at(idx);
}

const std::string& Constants_pool::get_string(unsigned int idx) const{
	return this->strings.at(idx);
}

const std::string& Constants_pool::get_libfunc(unsigned int idx) const{
	return this->libfuncs.at(idx);
}

const Userfunc& Constants_pool::get_userfunc_on_idx(unsigned int idx) const{
	return this->idx_to_userfuncs.at(idx);
}
const Userfunc& Constants_pool::get_userfunc_on_address(unsigned int userfunc_addr) const {
    return *(this->addr_to_userfuncs.at(userfunc_addr));
}

std::map<unsigned int, double long> Constants_pool::get_numbers_array() const{
	return this->numbers;
}

std::map<unsigned int, std::string> Constants_pool::get_string_array() const{
	return this->strings;
}

std::map<unsigned int, std::string> Constants_pool::get_libfunc_array() const{
	return this->libfuncs;
}

std::map<unsigned int, Userfunc> Constants_pool::get_userfunc_array() const{
	return this->idx_to_userfuncs;
}

unsigned int Constants_pool::register_number(long double value){
    for(auto const &pair : this->numbers)   //Check if value already exists
        if(pair.second == value)
            return pair.first;

    //Value does not exist, insert it
    unsigned int key = (unsigned int)numbers.size();
    assert_non_existent_key(numbers, key);      //assert that the auto generated key does not exist
	numbers[key] = value;
    return key;
}

unsigned int Constants_pool::register_string(const std::string &str){
    for(auto const &pair : this->strings)
        if(pair.second == str)
            return pair.first;

    unsigned int key = (unsigned int)strings.size();
    assert_non_existent_key(strings, key);      //assert that the auto generated key does not exist
    strings[key] = str;
    return key;
}

unsigned int Constants_pool::register_libfunc(const std::string &libfunc_name){
    for(auto const &pair : libfuncs)
        if(pair.second == libfunc_name)
            return pair.first;

    unsigned int key = (unsigned int)libfuncs.size();
    assert_non_existent_key(libfuncs, key);      //assert that the auto generated key does not exist
    libfuncs[key] = libfunc_name;
    return key;
}

unsigned int Constants_pool::register_userfunc(const Userfunc &userfunc){
    for(auto const &pair : idx_to_userfuncs)
        if(pair.second == userfunc)
            return pair.first;

    unsigned int key = (unsigned int)idx_to_userfuncs.size();
    assert_non_existent_key(idx_to_userfuncs, key);      //assert that the auto generated key does not exist
    assert_non_existent_key(addr_to_userfuncs, userfunc.address);
    idx_to_userfuncs[key] = userfunc;
    addr_to_userfuncs[userfunc.address] = &idx_to_userfuncs.at(key);
    return key;
}