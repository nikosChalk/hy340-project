#include "../intermediate_code/quad.h"

#ifndef HY340_PROJECT_TYPES_H
#define HY340_PROJECT_TYPES_H

namespace target_code {
    struct Incomplete_Jump{
        unsigned int instrNo;
        unsigned int iaddress;
        /**
        * Default initialization. (Junk)
        */
        Incomplete_Jump();
        /**
        * Initializes the Incomplete_Jump representation with the given attributes
        * @param instrNo //TODO: fill it
        * @param iaddress //TODO: fill it
        */
        Incomplete_Jump(unsigned int instrNo, unsigned int iaddress);
    };
	
	struct Tcode_quad{
		intermediate_code::quad* tquad;
		unsigned int taddress;
		/**
        * Default initialization. (Junk)
        */
        Tcode_quad();
        /**
        * Initializes the Tcode_quad representation with the given attributes
        * @param tquad //TODO: fill it
        * @param taddress //TODO: fill it
        */
		Tcode_quad(intermediate_code::quad* tquad, unsigned int taddress);
	};
}

#endif //HY340_PROJECT_TYPES_H
