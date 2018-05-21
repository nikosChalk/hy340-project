

#ifndef HY340_PROJECT_CONSTANTS_POOL_H
#define HY340_PROJECT_CONSTANTS_POOL_H


#include <vector>
#include <string>
#include <bits/unordered_map.h>
#include "../../common_interface/vm_types.h"

namespace virtual_machine {

    class Constants_pool {
    public:

        /**
         * Creates a constant pool that contains all the constants used by the program.
         * Note that for each vector argument, an internal copy is made.
         * @param doubles The constant vector for the doubles
         * @param strings The constant vector for the strings
         * @param libfuncs The constant vector for the libfuncs
         * @param userfuncs The constant vector for the userfuncs
         */
        Constants_pool(const std::vector<double> &doubles, const std::vector<std::string> &strings,
                       const std::vector<std::string> &libfuncs, const std::vector<Userfunc> &userfuncs);

        //TODO: double get_double(unsigned int idx) const;
        //Synithws thelw access se ENA element kai oxi se diathoxika

        /**
         * Returns the numeric constant at the given index
         * @param idx The index
         * @return The numeric constant
         */
        double get_numeric(unsigned int idx) const;

        /**
         * Returns the string constant at the given index
         * @param idx The index
         * @return The string constant
         */
        const std::string& get_string(unsigned int idx) const;

        /**
         * Returns the library function constant at the given index
         * @param idx The index
         * @return The library function constant
         */
        const std::string& get_libfunc(unsigned int idx) const;

        /**
         * Returns the user function constant at the given index
         * @param idx The index
         * @return The user function constant
         */
        const Userfunc& get_userfunc(unsigned int idx) const;



    private:
        std::vector<double> doubles;
        std::vector<std::string> strings;
        std::vector<std::string> libfuncs;
        std::unordered_map<unsigned int, Userfunc> userfuncs;
    };

};


#endif //HY340_PROJECT_CONSTANTS_POOL_H
