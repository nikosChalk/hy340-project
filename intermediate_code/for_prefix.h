#ifndef HY340_PROJECT_FOR_PREFIX_H
#define HY340_PROJECT_FOR_PREFIX_H

#include "expr.h"

namespace intermediate_code {

    /**
     * Class used for the semantic value of the "forprefix" non-terminal symbol
     */
	class for_prefix{
	public:
		/**
		 * Initializes the for prefix with the given fields
		 * @param cond_first_quadno The quad number of the first quad of the for's condition "expr"
		 * @param comparator_quadno The quad number of the intermediate code instruction "if_eq $cond_expr TRUE, then goto body"
		 */
		for_prefix(unsigned int cond_first_quadno, unsigned int comparator_quadno);

        /**
         * Returns the quad number of the first quad of the for's condition "expr"
         * @return The quad number
         */
		unsigned int get_cond_first_quadno() const;

		/**
		 * Returns the quad number of the intermediate code instruction "if_eq $cond_expr TRUE, then goto body"
		 * @return The quad number
		 */
		unsigned int get_comparator_quadno() const;

	private:
		unsigned int cond_first_quadno;
		unsigned int comparator_quadno;
	};
}

#endif //HY340_PROJECT_FOR_PREFIX_H