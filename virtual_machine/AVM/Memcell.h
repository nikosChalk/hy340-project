
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
            char            *str_ptr;       //must be malloced and freed
            bool            boolean;
            Table           *table_ptr;
            unsigned int    userfunc_addr;  //address (label) of "funcstart" vm instruction
            char const      *libfunc_ptr;   //no need to malloc/free. These pointers point to strings of Constant_pool
        } value;

        /**
         * Initializes the memcell with type undef
         */
        Memcell();

        /**
         * Clears the memcell by setting its type to Type::undef.
         * If the memcell was of type table, then the table's reference counter is also decreased
         * If the memcell was of type string, then the string is freed
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
         *
         * @throws std::runtime_error
         */
        bool operator!=(const Memcell &other) const;

        /**
         * Returns the string representation of this object
         * @return The string representation
         */
        std::string to_string() const;

    private:

        /* Functions that assign another Memcell to *this through deep copy, based on a given fixed same type           */
        /* this->type is considered valid, while this->value is considered invalid and is set through these functions   */

        /**
         * Sets value.num to other.value.num
         */
        void assign_number(const Memcell *other);

        /**
         * Sets value.str_ptr to string copy of other.value.str_ptr. Pointers will point to different memory with the same content
         */
        void assign_string(const Memcell *other);

        /**
         * Sets value.boolean to other.value.boolean
         */
        void assign_boolean(const Memcell *other);

        /**
         * Sets value.table_ptr to other.value.table_ptr and increases the table's reference counter if need be.
         */
        void assign_table(const Memcell *other);

        /**
         * Sets value.userfunc_addr to other.value.userfunc_addr
         */
        void assign_userfunc(const Memcell *other);

        /**
         * Sets value.libfunc_ptr to other.value.libfunc_ptr. Both pointers will point to the same memory address
         */
        void assign_libfunc(const Memcell *other);

        /**
         * No action is needed.
         */
        void assign_nil(const Memcell *other);

        /**
         * No action is taken
         */
        void assign_undef(const Memcell *other);

        typedef void (Memcell::*assign_func_t)(const Memcell *other);
        static const std::map<unsigned int, assign_func_t> assign_func_table = {
                {Type::number, assign_number}, {Type::string, assign_string}, {Type::boolean, assign_boolean},
                {Type::table, assign_table}, {Type::userfunc, assign_userfunc}, {Type::libfunc, assign_libfunc},
                {Type::nil, assign_nil}, {Type::undef, assign_undef}
        };


        /* Functions that clear the content of the Memcell, based on a given fixed type */

        /**
         * De-allocates the string
         */
        void string_clear();

        /**
         * Decreases the table's reference counter
         */
        void table_clear();

        typedef void (Memcell::*clear_func_t)();
        static const std::map<unsigned int, clear_func_t> clear_func_table = {
                {Type::number, nullptr}, {Type::string, string_clear}, {Type::boolean, nullptr},
                {Type::table, table_clear}, {Type::userfunc, nullptr}, {Type::libfunc, nullptr},
                {Type::nil, nullptr}, {Type::undef, nullptr}
                //Values "nullptr" indicate that no clean up action is needed
        };

        /* Functions that convert the value of the Memcell to bool, based on a given fixed type */

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
