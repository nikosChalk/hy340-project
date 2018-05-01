
#include "for_prefix.h"

using namespace std;
using namespace intermediate_code;

for_prefix::for_prefix(unsigned int test, unsigned int enter){
	this->test = test;
	this->enter = enter;
}
unsigned int for_prefix::get_for_prefix_test(){
	return this->test;
}
unsigned int for_prefix::get_for_prefix_enter(){
	return this->enter;
}