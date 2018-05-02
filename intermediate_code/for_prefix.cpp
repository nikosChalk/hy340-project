
#include <cassert>
#include "for_prefix.h"

using namespace std;
using namespace intermediate_code;

for_prefix::for_prefix(unsigned int cond_first_quadno, unsigned int comparator_quadno) {
	assert(cond_first_quadno <= comparator_quadno);  //sanity check

	this->cond_first_quadno = cond_first_quadno;
	this->comparator_quadno = comparator_quadno;
}
unsigned int for_prefix::get_cond_first_quadno() const {
	return this->cond_first_quadno;
}
unsigned int for_prefix::get_comparator_quadno() const {
	return this->comparator_quadno;
}