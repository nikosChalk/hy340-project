

#ifndef HY340_PROJECT_CONSTANTS_POOL_H
#define HY340_PROJECT_CONSTANTS_POOL_H


#include <vector>
#include <string>
#include <map>
#include "vm_types.h"

namespace virtual_machine {

    class Constants_pool {
    public:

        /**
         * Creates an empty pool of constants
         */
        Constants_pool();

        /**
         * Creates a constant pool that contains all the constants used by the program.
         * Note that for each vector argument, an internal copy is made.
         * @param doubles The constant vector for the doubles
         * @param strings The constant vector for the strings
         * @param libfuncs The constant vector for the libfuncs
         * @param userfuncs The constant vector for the userfuncs
         */
        Constants_pool(const std::vector<long double> &doubles, const std::vector<std::string> &strings,
                       const std::vector<std::string> &libfuncs, const std::vector<Userfunc> &userfuncs);

        /**
         * Returns the numeric constant at the given index
         * @param idx The index
         * @return The numeric constant
         */
        double long get_numeric(unsigned int idx) const;

        /**
         * Returns the string constant at the given index
         * @param idx The index
         * @return The string constant
         */
        const std::string& get_string(unsigned int idx) const;

        /**
         * Returns the library function constant at the given index
         * @param idx The index
         * @return The library function's name
         */
        const std::string& get_libfunc(unsigned int idx) const;

        /**
         * Returns the user function constant at the given index
         * @param idx The index
         * @return The user function
         */
        const Userfunc& get_userfunc_on_idx(unsigned int idx) const;

        /**
         * Returns the user function constant that its VMinstruction "funcstart" is in the given userfunc_addr
         * @param userfunc_addr The address of the "funcstart" user function
         * @return The user function
         */
        const Userfunc& get_userfunc_on_address(unsigned int userfunc_addr) const;

        /**
         * Returns a copy of the numbers array
         */
        std::map<unsigned int, double long> get_numbers_array() const;
        /**
         * Returns a copy of the strings array
         */
        std::map<unsigned int, std::string> get_string_array() const;

        /**
         * Returns a copy of the library functions array
         */
        std::map<unsigned int, std::string> get_libfunc_array() const;

        /**
         * Returns a copy of the userfuncs array
         */
        std::map<unsigned int, Userfunc> get_userfunc_array() const;



        /**
         * Registers the given number value to the *this constant pool. If the value already exists, no insertion occurs.
         * @param number The constant value to register
         * @return The index (key) in the numbers array in which the value was registered
         */
        unsigned int register_number(long double value);
        /**
         * Same as register_number, but registers the given value at the given idx, overwriting any previous value.
         * @return The idx
         */
        unsigned int register_number(unsigned int idx, long double value);

        /**
         * Registers the given str to the *this constant pool. If the value already exists, no insertion occurs.
         * @param str The constant string to register
         * @return The index (key) in the strings array in which the value was registered
         */
        unsigned int register_string(const std::string &str);
        /**
         * Same as register_string, but registers the given str at the given idx, overwriting any previous value.
         * @return The idx
         */
        unsigned int register_string(unsigned int idx, const std::string &str);

        /**
         * Registers the given library function to the *this constant pool. If the value already exists, no insertion occurs.
         * @param idx The index (key) within the library function array
         * @param libfunc_name The library function to register
         * @return The index (key) in the library function array in which the libfunc_name was registered
         */
        unsigned int register_libfunc(const std::string &libfunc_name);
        /**
         * Same as register_libfunc, but registers the given libfunc_name at the given idx, overwriting any previous value.
         * @return The idx
         */
        unsigned int register_libfunc(unsigned int idx, const std::string &libfunc_name);

        /**
         * Registers the given user function to the *this constant pool. The key for retrieving the registered userfunc
         * can be either the returned idx or the userfunc.address
         * @param userfunc The user function to register
         * @return The index (key) in the user functions array in which the userfunc was registered
         */
        unsigned int register_userfunc(const Userfunc &userfunc);
        /**
         * Same as register_userfunc, but registers the given userfunc at the given idx, overwriting any previous value.
         * @return The idx
         */
        unsigned int register_userfunc(unsigned int idx, const Userfunc &userfunc);


    private:
        std::map<unsigned int, long double> numbers;    //idx used as key
        std::map<unsigned int, std::string> strings;    //idx used as key
        std::map<unsigned int, std::string> libfuncs;   //idx used as key

        //key.first == idx, key.second == address of "funcenter" VM instruction
        std::map<std::pair<unsigned int, unsigned int>, Userfunc> userfuncs;
    };

};


#endif //HY340_PROJECT_CONSTANTS_POOL_H
