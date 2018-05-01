#ifndef HY340_PROJECT_FOR_PREFIX_H
#define HY340_PROJECT_FOR_PREFIX_H

#include "expr.h"

namespace intermediate_code {

	class for_prefix{
	public:
		/*constructor*/
		for_prefix(unsigned int test, unsigned int enter);
		/*get function for private field test*/
		unsigned int get_for_prefix_test();
		/*get function for private field enter*/
		unsigned int get_for_prefix_enter();

	private:
		unsigned int test;
		unsigned int enter;
	};
}

#endif //HY340_PROJECT_FOR_PREFIX_H