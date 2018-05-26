#include "Constants_pool.h"

using namespace virtual_machine;

Constants_pool::Constants_pool(const std::vector<long double> &doubles, const std::vector<std::string> &strings,
	const std::vector<std::string> &libfuncs, const std::vector<Userfunc> &userfuncs){

	/*initialization of number's map*/
	for (int i = 0; i < doubles.size(); i++)
		this->numbers.insert(std::pair<unsigned int, long double>(i, doubles.at(i)));

	/*initialization of string's map*/
	for (int i = 0; i < strings.size(); i++)
		this->strings.insert(std::pair<unsigned int, std::string>(i, strings.at(i)));

	/*initialization of libfunc's map*/
	for (int i = 0; i < libfuncs.size(); i++)
		this->libfuncs.insert(std::pair<unsigned int, std::string>(i, libfuncs.at(i)));

	/*initialization of userfunc's map*/
	for (int i = 0; i < userfuncs.size(); i++){
		std::pair<unsigned int, unsigned int> p = std::pair<unsigned int, unsigned int>(i, userfuncs.at(i).address);
		this->userfuncs.insert(std::pair <std::pair<unsigned int, unsigned int>, Userfunc>(p, userfuncs.at(i)));
	}
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
	return this->userfuncs.at(idx);
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
	return this->userfuncs;
}

unsigned int Constants_pool::register_number(long double value){
	std::map<unsigned int, long double>::iterator it;
	for (it = this->numbers.begin(); it != this->numbers.end(); ++it){
		if (it->second == value)
			return it->first;
	}
	it--; //the last element of map , we need this key + 1 for a new record
	this->numbers.insert(std::pair<unsigned int, long double>(it->first + 1, value));
	return it->first + 1;
}

unsigned int Constants_pool::register_number(unsigned int idx, long double value){
	this->numbers.at(idx) = value;
	return idx;
}

unsigned int Constants_pool::register_string(const std::string &str){
	std::map<unsigned int, std::string>::iterator it;
	for (it = this->strings.begin(); it != this->strings.end(); ++it){
		if (it->second == str)
			return it->first;
	}
	it--; //the last element of map , we need this key + 1 for a new record
	this->strings.insert(std::pair<unsigned int, std::string>(it->first + 1, str));
	return it->first + 1;
}

unsigned int Constants_pool::register_string(unsigned int idx, const std::string &str){
	this->strings.at(idx) = str;
	return idx;
}

unsigned int Constants_pool::register_libfunc(const std::string &libfunc_name){
	std::map<unsigned int, std::string>::iterator it;
	for (it = this->libfuncs.begin(); it != this->libfuncs.end(); ++it){
		if (it->second == libfunc_name)
			return it->first;
	}
	it--; //the last element of map , we need this key + 1 for a new record
	this->libfuncs.insert(std::pair<unsigned int, std::string>(it->first + 1, libfunc_name));
	return it->first + 1;
}

unsigned int Constants_pool::register_libfunc(unsigned int idx, const std::string &libfunc_name){
	this->libfuncs.at(idx) = libfunc_name;
	return idx;
}

unsigned int Constants_pool::register_userfunc(const Userfunc &userfunc){
	std::map<std::pair<unsigned int, unsigned int>, Userfunc>::iterator it;
	for (it = this->userfuncs.begin(); it != this->userfuncs.end(); ++it){
		if (it->first.second == userfunc.address)
			return it->first.first; //idx
	}
	it--; //the last element of map , we need this key + 1 for a new record
	std::pair<unsigned int, unsigned int> p = std::pair<unsigned int, unsigned int>(it->first.first + 1, userfunc.address);
	this->userfuncs.insert(std::pair <std::pair<unsigned int, unsigned int>, Userfunc>(p, userfunc));
	return it->first.first + 1;
}

unsigned int Constants_pool::register_userfunc(unsigned int idx, const Userfunc &userfunc){
	std::map<std::pair<unsigned int, unsigned int>, Userfunc>::iterator it;
	for (it = this->userfuncs.begin(); it != this->userfuncs.end(); ++it){
		if (it->first.first == idx){
			this->userfuncs.erase(it);
			std::pair<unsigned int, unsigned int> p = std::pair<unsigned int, unsigned int>(idx, userfunc.address);
			this->userfuncs.insert(std::pair <std::pair<unsigned int, unsigned int>, Userfunc>(p, userfunc));
			return idx;
		}
	}
	throw std::runtime_error("Invalid index-key for userfuncs map");
}