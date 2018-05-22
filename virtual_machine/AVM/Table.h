

#ifndef HY340_PROJECT_TABLE_H
#define HY340_PROJECT_TABLE_H

#include <unordered_map>
#include "Memcell.h"

#define MAX_HASH_SIZE 211

namespace virtual_machine {

    class Table {
    public:

        /**
         * Initializes an empty table with reference counter 1.
         */
        Table();

        /**
         * Increases the reference counter of this table by 1
         */
        void increase_ref_counter();


        /**
         * Decreases the reference counter of this object by 1.
         * If the reference counter reaches 0, "delete this;" is executed, thus also invoking the
         * destructor. Note that the above code statement requires the following
         * predicate to hold true:
         *
         * "This object was allocated via new (not by new[], nor by placement new,
         * nor a local object on the stack, nor a global, nor a member of another object;
         * but by plain ordinary new)"
         *
         * @return nullptr if "delete this;" was executed. Otherwise returns "this"
         */
        Table* decrease_ref_counter();
        /*
         * TODO: Information for the implementer of decrease_ref_counter()
         * Vlepe stack overflow answer: https://stackoverflow.com/a/7039749
         */


        /**
         * Destroys the table.
         * The table's elements are iterated in order to check for other tables.
         * If an element is a table then its reference counter is decreased through
         * decrease_ref_counter(), since this object will be deleted and will no longer refer
         * to that table element.
         */
        ~Table();

        /**
         * Returns the value of the table at the given index
         * @param idx The index within the table. Note that the only types supported for indexing
         * are Memcell::Type number, string
         * @return The value of the table at the given index. If no element was found with the given idx as key,
         * then nullptr is returned
         */
        Memcell* get_elem(Memcell const *idx) const;

        /**
         * Sets the value of the table at the given index to the given val
         * @param idx The index within the table. Note that the only types supported for indexing
         * are Memcell::Type number, string
         * @param val The value to set at the given idx
         */
        void set_elem(Memcell const *idx, Memcell *val);  //TODO: do not forget the assertion of max size for each hash map
        //TODO: gia to implementation: To Type::nil val afairei stoixeia. (33/36)
        //TODO: epishs xrhsimopoioume avm_assign kai avm_memclear!

    private:
        unsigned int ref_counter;
        std::unordered_map<std::string, Memcell> stringsMap;    //used by statements e.g. "table.key"
        std::unordered_map<double, Memcell> numericMap;         //used by statements e.. "table[5]"
    };
}


#endif //HY340_PROJECT_TABLE_H
