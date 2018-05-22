

#ifndef HY340_PROJECT_TABLE_H
#define HY340_PROJECT_TABLE_H

#include <map>
#include <unordered_map>
#include <string>
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

        /**
         * Destroys the table.
         * The table's elements are iterated in order to check for other tables or malloced strings.
         * For each element of the table, Memcell::clear() is called
         */
        ~Table();

        /**
         * Returns the value of the table at the given index
         * @param idx The index within the table. The idx->type can by of any value.
         * @return The value of the table at the given index. If no element was found with the given idx as key,
         * then nullptr is returned
         */
        Memcell* get_elem(Memcell const *idx);

        /**
         * Sets the value of the table at the given index to the given val
         * @param idx The index within the table. Note that the only types supported for indexing
         * are Memcell::Type number, string
         * @param val The value to set at the given idx
         */
        void set_elem(Memcell const *idx, Memcell const *val);  //TODO: do not forget the assertion of max size for each hash map
        //TODO: gia to implementation: To Type::nil val afairei stoixeia. (33/36)
        //TODO: epishs xrhsimopoioume avm_assign kai avm_memclear!
        //TODO: throw error gia undef?
        //TODO: to mapping einai poly pithano na allaksw logo mias set.

    private:
        unsigned int ref_counter;
        std::map<double, Memcell> numericMap;           //used by statements e.g. table[5];
        std::map<std::string, Memcell> stringMap;       //used by statements e.g. table.key or table["key"];
        std::map<Table*, Memcell> tableMap;             //used by statements e.g. x = [5, 3]; table[x]; or table[ [] ];
        std::map<unsigned int, Memcell> userfuncMap;    //used by statements e.g. function f() {}; table[f];
        std::map<std::string, Memcell> libfuncMap;      //used by statements e.g. table[cos];

        /* The Memcell::Type nil, boolean do no need an std::map as their values are enumerable                     */
        /* Thus we used a pair of <Memcell, bool>, where the boolean value indicates if this memcell has been set.  */
        /* True indicates that it has been set and false indicates that it is not used. (never assigned or cleared) */

        std::pair<Memcell, bool> nil_pair;              //used by statements e.g. table[nil];
        std::pair<Memcell, bool> true_pair, false_pair; //used by statements e.g. table[true]; or table[false];

        /* Functions that return the Memcell associated with the given key, based on a FIXED key->type. */
        /* If no value is associated with the given key, then nullptr is returned.                      */

        Memcell* getelem_number_key(Memcell const *key);
        Memcell* getelem_string_key(Memcell const *key);
        Memcell* getelem_boolean_key(Memcell const *key);
        Memcell* getelem_table_key(Memcell const *key);
        Memcell* getelem_userfunc_key(Memcell const *key);
        Memcell* getelem_libfunc_key(Memcell const *key);
        Memcell* getelem_nil_key(Memcell const *key);

        /**
         * Always returns nullptr as the key itself is invalid
         */
        Memcell* getelem_undef_key(Memcell const *key);

        typedef Memcell* (Table::*getelem_func_t)(Memcell const *key);
        static const std::map<unsigned int, getelem_func_t> getelem_func_map = {
                {Memcell::Type::number, getelem_number_key}, {Memcell::Type::string, getelem_string_key},
                {Memcell::Type::boolean, getelem_boolean_key}, {Memcell::Type::table, getelem_table_key},
                {Memcell::Type::userfunc, getelem_userfunc_key}, {Memcell::Type::libfunc, getelem_libfunc_key},
                {Memcell::Type::nil, getelem_nil_key}, {Memcell::Type::undef, getelem_undef_key},
        };

        /* Functions that set the Memcell associated with the given key, based on a FIXED key->type to the given value  */
        /* Note that if val->type == Memcell::Type::nil, the element's content is cleared instead.                      */

        void setelem_number_key(Memcell const *key, Memcell const *val);
        void setelem_string_key(Memcell const *key, Memcell const *val);
        void setelem_boolean_key(Memcell const *key, Memcell const *val);
        void setelem_table_key(Memcell const *key, Memcell const *val);
        void setelem_userfunc_key(Memcell const *key, Memcell const *val);
        void setelem_libfunc_key(Memcell const *key, Memcell const *val);
        void setelem_nil_key(Memcell const *key, Memcell const *val);

        /**
         * Does nothing as the key itself is invalid (No element is altered)
         */
        void setelem_undef_key(Memcell const *key, Memcell const *val);

        typedef void (Table::*setelem_func_t)(Memcell const *key, Memcell const *val);
        static const std::map<unsigned int, setelem_func_t> setelem_func_map = {
                {Memcell::Type::number, setelem_number_key}, {Memcell::Type::string, setelem_string_key},
                {Memcell::Type::boolean, setelem_boolean_key}, {Memcell::Type::table, setelem_table_key},
                {Memcell::Type::userfunc, setelem_userfunc_key}, {Memcell::Type::libfunc, setelem_libfunc_key},
                {Memcell::Type::nil, setelem_nil_key}, {Memcell::Type::undef, setelem_undef_key},
        };
    };
}


#endif //HY340_PROJECT_TABLE_H
