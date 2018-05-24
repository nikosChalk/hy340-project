
#ifndef HY340_PROJECT_MEMCELL_H
#define HY340_PROJECT_MEMCELL_H

#include "Constants_pool.h"
#include <map>

namespace virtual_machine {
    class Table;    //Forward declaration to avoid circular dependency

    class Memcell {
    public:
        //Declarations of pointer-to-member function types
        using Assign_membfunc_ptr = void (Memcell::*)(Memcell const *other);
        using Clear_membfunc_ptr = void (Memcell::*) (void);
        using Tobool_membfunc_ptr = bool (Memcell::*) (void) const;
        using Eqcheck_membfunc_ptr = bool (Memcell::*) (const Memcell &other) const;
        using Tostr_membfunc_ptr = std::string (Memcell::*)(Constants_pool const&) const;

        //Type declarations of mapping: int --> member function
        using Assign_func_map = std::map<unsigned int, Assign_membfunc_ptr>;
        using Clear_func_map = std::map<unsigned int, Clear_membfunc_ptr>;
        using Tobool_func_map = std::map<unsigned int, Tobool_membfunc_ptr>;
        using Eqcheck_func_map = std::map<unsigned int, Eqcheck_membfunc_ptr>;
        using Tostr_func_map = std::map<unsigned int, Tostr_membfunc_ptr>;

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
         * Caller should NOT call rh->clear() beforehand as this can be destructive behaviour! (e.g. when this == rh)
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
         * As this Memcell's value may be stored within either program stack, constants pool or retval register, depending
         * on its type, those parameters are needed
         * @param program_stack The program stack
         * @param const_pool The constants' pool
         * @param retval The retval register
         * @return The string representation
         */
        std::string to_string(Constants_pool const& const_pool) const;

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

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "assign_"  member function
         */
        static const Assign_func_map assign_func_map;


        /* Functions that clear the content of the Memcell, based on a given fixed type */

        /**
         * De-allocates the string
         */
        void string_clear();

        /**
         * Decreases the table's reference counter
         */
        void table_clear();

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "clear_"  member function
         * A value of nullptr indicates that there is no member function to call because no clean up action is needed
         */
        static const Clear_func_map clear_func_map;

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

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "_tobool"  member function
         */
        static const Tobool_func_map tobool_func_map;


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

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "_eqcheck"  member function
         */
        static const Eqcheck_func_map eqcheck_func_map;

        /* Functions that convert this Memcell to a string, based on a given fixed type */

        /**
          * Returns the string representation of value.num
          */
        std::string number_tostr(Constants_pool const &unused) const;

        /**
         * Returns the string representation of value.str_ptr
         */
        std::string string_tostr(Constants_pool const &unused) const;

        /**
         * Returns the string representation of value.boolean
         */
        std::string boolean_tostr(Constants_pool const &unused) const;

        /**
         * Returns the string representation of value.table_ptr
         */
        std::string table_tostr(Constants_pool const &const_pool) const;

        /**
         * Returns the string representation of value.userfunc_addr, i.e. that is the name of the function
         */
        std::string userfunc_tostr(Constants_pool const &const_pool) const;

        /**
         * Returns the string representation of value.libfunc_ptr
         */
        std::string libfunc_tostr(Constants_pool const &unused) const;

        /**
         * Returns the string representation for type.nil
         */
        std::string nil_tostr(Constants_pool const &unused) const;

        /**
         * Returns the string representation for type.undef
         */
        std::string undef_tostr(Constants_pool const &unused) const;

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "_tostr"  member function
         */
        static const Tostr_func_map tostr_func_map;
        
    };
}


#endif //HY340_PROJECT_MEMCELL_H
