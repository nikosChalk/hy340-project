

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
        * @param nr_locals The total number of local variables used by this function
        * @param name The function's name
        */
        Incomplete_Jump(unsigned int instrNo, unsigned int iaddress);
    };
}

#endif //HY340_PROJECT_TYPES_H
