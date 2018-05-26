

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
         * Returns the numeric constant at the given index
         * @param idx The index
         * @return The numeric constant
         * @throws std::out_of_range if no entry at the given index exists
         */
        double long get_numeric(unsigned int idx) const;

        /**
         * Returns the string constant at the given index
         * @param idx The index
         * @return The string constant
         * @throws std::out_of_range if no entry at the given index exists
         */
        const std::string& get_string(unsigned int idx) const;

        /**
         * Returns the library function constant at the given index
         * @param idx The index
         * @return The library function's name
         * @throws std::out_of_range if no entry at the given index exists
         */
        const std::string& get_libfunc(unsigned int idx) const;

        /**
         * Returns the user function constant at the given index
         * @param idx The index
         * @return The user function
         * @throws std::out_of_range if no entry at the given index exists
         */
        const Userfunc& get_userfunc_on_idx(unsigned int idx) const;

        /**
         * Returns the user function constant that its VMinstruction "funcstart" is in the given userfunc_addr
         * @param userfunc_addr The address of the "funcstart" user function
         * @return The user function
         * @throws std::out_of_range if no entry at the given index exists
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
         * Returns a copy of the userfuncs array with key the registered index (not the userfunc's address)
         */
        std::map<unsigned int, Userfunc> get_userfunc_array() const;



        /**
         * Registers the given number value to the *this constant pool. If the value already exists, no insertion occurs.
         * @param number The constant value to register
         * @return The index (key) in the numbers array in which the value was registered
         */
        unsigned int register_number(long double value);

        /**
         * Registers the given str to the *this constant pool. If the value already exists, no insertion occurs.
         * @param str The constant string to register
         * @return The index (key) in the strings array in which the value was registered
         */
        unsigned int register_string(const std::string &str);

        /**
         * Registers the given library function to the *this constant pool. If the value already exists, no insertion occurs.
         * @param idx The index (key) within the library function array
         * @param libfunc_name The library function to register
         * @return The index (key) in the library function array in which the libfunc_name was registered
         */
        unsigned int register_libfunc(const std::string &libfunc_name);

        /**
         * Registers the given user function to the *this constant pool. The key for retrieving the registered userfunc
         * can be either the returned idx or the userfunc.address. If the value already exists, no insertion occurs.
         * @param userfunc The user function to register
         * @return The index (key) in the user functions array in which the userfunc was registered
         */
        unsigned int register_userfunc(const Userfunc &userfunc);


    private:
        std::map<unsigned int, long double> numbers;    //idx used as key
        std::map<unsigned int, std::string> strings;    //idx used as key
        std::map<unsigned int, std::string> libfuncs;   //idx used as key

        std::map<unsigned int, Userfunc>  idx_to_userfuncs;  //idx used as key

        //address of "funcenter" VM instruction used as key. Values are pointers to values of idx_to_userfuncs
        std::map<unsigned int, Userfunc const*> addr_to_userfuncs;
    };

};


#endif //HY340_PROJECT_CONSTANTS_POOL_H
