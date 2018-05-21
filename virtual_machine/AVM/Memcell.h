
#ifndef HY340_PROJECT_MEMCELL_H
#define HY340_PROJECT_MEMCELL_H

#include "Table.h"
#include <map>

namespace virtual_machine {

    class Memcell {
    public:
        enum Type {
            number,     //uses field num.
            string,     //used field str_ptr.
            boolean,    //uses field boolean
            table,      //uses field table_ptr
            userfunc,   //uses field userfunc_addr
            libfunc,    //uses field libfunc_ptr
            nil,        //no fields used
            undef       //no fields used
        };
        /**
         * Returns the string representation of the given type
         * @return The string representation
         */
        static std::string type_to_string(Type type);

        Type type;

        /**
         * Memcell's value based on type. The value is not a reference to an index of the constant pool but an actual value.
         */
        union {
            long double     num;
            char const      *str_ptr;    //TODO: VALIDATION: if string manipulation is allowed, const must be removed and malloced string must be used. NOT with "std::string.c_str();"
            bool            boolean;
            Table           *table_ptr;
            unsigned int    userfunc_addr;  //address (label) of "funcstart" vm instruction
            char const      *libfunc_ptr;
        } value;

        /**
         * Initializes the memcell with type undef
         */
        Memcell();

        /**
         * Clears the memcell by setting its type to Type::undef.
         * If the memcell was of type table, then the table's reference counter is also decreased
         */
        void clear();

        /**
         * Sets the content (type and value) of this object to the content of the right hand (rh).
         * Effectively this function does: *this = *rh, without overloading the assignment/move
         * operator for convenience.
         * @param rh The right hand side. Must not be NULL/nullptr
         */
        void assign(Memcell const *rh);

        /**
         * Returns the boolean semantic value of the memcell depending on its type
         * @return The boolean value of the Memcell as defined by the semantics of alpha
         */
        bool promote_to_bool() const;

        /**
         * Compares *this with other based on their type and the semantics of the language
         * @param other The other to compare with
         * @return True if *this == other. False otherwise
         *
         * @throws std::runtime_error if *this or other have type==Type::undef or if they
         * are uncomparable (as defined in the language semantics)
         */
        bool operator==(const Memcell &other) const;

        /**
         * Compares *this with other. See operator==() for more information
         * @param other The other to compare
         * @return Returns !(*this == other)
         */
        bool operator!=(const Memcell &other) const;

        /**
         * Returns the string representation of this object
         * @return The string representation
         */
        std::string to_string() const;

    private:

        /* Functions that convert the value of the Memcell to bool, based on a given type */

        /**
         * Returns true if the value.num not zero. False otherwise
         */
        bool number_tobool() const;
        /**
         * Returns true if the value.str_ptr is not the empty string (null). False otherwise
         */
        bool string_tobool() const;

        /**
         * Returns true if the value.boolean is true. False otherwise
         */
        bool boolean_tobool() const;

        /**
         * Always returns true
         */
        bool table_tobool() const;

        /**
         * Always returns true
         */
        bool userfunc_tobool() const;

        /**
         * Always returns true
         */
        bool libfunc_tobool() const;

        /**
         * Always returns false
         */
        bool nil_tobool() const;

        /**
         * Will cause an assertion(false) as undef cannot be promoted to bool
         */
        bool undef_tobool() const;

        typedef bool (Memcell::*tobool_func_t)() const;
        static const std::map<unsigned int, tobool_func_t> tobool_func_table = {
                {Type::number, number_tobool}, {Type::string, string_tobool}, {Type::boolean, boolean_tobool},
                {Type::table, table_tobool}, {Type::userfunc, userfunc_tobool}, {Type::libfunc, libfunc_tobool},
                {Type::nil, nil_tobool}, {Type::undef, undef_tobool}
        };

        /* Functions that compare the value of the Memcell with another value, based on a given SAME type, for equality check */

        /**
         * Returns true if the value.num are equal. False otherwise
         */
        bool number_eqcheck(const Memcell &other) const;

        /**
         * Returns true if the value.str_ptr strings are equal (as strings, not as pointers). False otherwise
         */
        bool string_eqcheck(const Memcell &other) const;

        /**
         * Returns true if the value.boolean are equal. False otherwise
         */
        bool boolean_eqcheck(const Memcell &other) const;

        /**
         * Returns true if the value.table_ptr are equal (as pointers). False otherwise
         */
        bool table_eqcheck(const Memcell &other) const;

        /**
         * Returns true if the value.userfunc_addr are equal (as addresses). False otherwise
         */
        bool userfunc_eqcheck(const Memcell &other) const;

        /**
         * Returns true if the value.libfunc_ptr are equal (as strings, not pointers). False otherwise
         */
        bool libfunc_eqcheck(const Memcell &other) const;

        /**
         * Returns true
         */
        bool nil_eqcheck(const Memcell &other) const;

        /**
         * Will cause an assertion(false) as undef cannot be compared to undef
         */
        bool undef_eqcheck(const Memcell &other) const;

        typedef bool (Memcell::*eqcheck_func_t)(const Memcell &other) const;
        static const std::map<unsigned int, eqcheck_func_t> eqcheck_func_table = {
                {Type::number, number_eqcheck}, {Type::string, string_eqcheck}, {Type::boolean, boolean_eqcheck},
                {Type::table, table_eqcheck}, {Type::userfunc, userfunc_eqcheck}, {Type::libfunc, libfunc_eqcheck},
                {Type::nil, nil_eqcheck}, {Type::undef, undef_eqcheck}
        };
    };
}


#endif //HY340_PROJECT_MEMCELL_H
