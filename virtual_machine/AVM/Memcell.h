
#ifndef HY340_PROJECT_MEMCELL_H
#define HY340_PROJECT_MEMCELL_H

#include "Table.h"

namespace virtual_machine {

    struct Memcell {
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

        Type type;

        /**
         * Memcell's value based on type. The value is not a reference to an index of the constant pool but an actual value.
         */
        union {
            double          num;
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
         * Returns the string representation of this object
         * @return The string representation
         */
        const std::string& to_string() const;
    };
}


#endif //HY340_PROJECT_MEMCELL_H
