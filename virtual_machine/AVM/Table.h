

#ifndef HY340_PROJECT_TABLE_H
#define HY340_PROJECT_TABLE_H

#include <map>
#include <unordered_map>
#include <string>
#include "../../common_interface/Constants_pool.h"
#include "Memcell.h"

namespace virtual_machine {

    class Table {
    public:
        //Type declarations of pointer-to-member function types
        using Getelem_membfunc_ptr = Memcell* (Table::*)(Memcell const *key, bool auto_create);
        using Unregister_membfunc_ptr = void (Table::*)(Memcell const *key);

        //Type declarations of mapping: int --> member functions
        using Getelem_func_map = std::map<unsigned int, Getelem_membfunc_ptr>;
        using Unregister_func_map = std::map<unsigned int, Unregister_membfunc_ptr>;

        /**
         * Initializes an empty table with reference counter 1.
         */
        Table();

		Table(Table const *other);

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
         * @param idx The index within the table. The idx->type can by of any value. idx must not be NULL/nullptr
         * @return The value of the table at the given index. If no element was found with the given idx as key,
         * then nullptr is returned
         */
        Memcell* get_elem(Memcell const *idx);

        /**
         * Sets the value of the table at the given index to the given val
         * The assignment occurs through the Memcell::assign() and the content is cleared beforehand through Memcell::clear()
         * Note that for val->type == Memcell::Type::nil, the content at the given idx is cleared instead. Thus further get_elem(idx) would return nullptr
         * @param idx The index within the table. The idx->type can by of any value. idx must not be NULL/nullptr
         * @param val The value to set at the given idx. Must not be NULL/nullptr
         */
        void set_elem(Memcell const *idx, Memcell const *val);

		Table* get_all_keys() const;

		unsigned int get_nr_keys() const;

        /**
         * Returns the string representation of this object
         * @param const_pool The constans' pool, since for Memcells of type userfunc, we need to translate the userfunc_addr
         * to the function's name
         * @return The string representation of this object
         */
        std::string to_string(Constants_pool const &const_pool) const;

    private:
        unsigned int ref_counter;
        std::map<long double, Memcell> numericMap;      //used by statements e.g. table[5];
        std::map<std::string, Memcell> stringMap;       //used by statements e.g. table.key or table["key"];
        std::map<Table*, Memcell> tableMap;       //used by statements e.g. x = [5, 3]; table[x]; or table[ [] ];
        std::map<unsigned int, Memcell> userfuncMap;    //used by statements e.g. function f() {}; table[f];
        std::map<std::string, Memcell> libfuncMap;      //used by statements e.g. table[cos];
        std::unordered_map<bool, Memcell> boolMap;      //used by statements e.g. table[true]; or table[false];

        /* For the Memcell::Type::nil, we do no need an std::map as it is jut one Memcell                           */
        /* Thus we use a pair of <Memcell, bool>, where the boolean value indicates if this memcell has been set.   */
        /* True indicates that it has been set and false indicates that it is not used. (never assigned or cleared) */
        std::pair<Memcell, bool> nil_pair;              //used by statements e.g. table[nil];

        /* Functions that return the Memcell associated with the given key, based on a FIXED key->type. */
        /* If auto_create is true, then if the Memcell with the given key does not exist, it is         */
        /* constructed at the given index with type undef and is returned.                              */
        /*                                                                                              */
        /* If auto_create is false, then this is a simple lookup operation. If no value is associated   */
        /* with the given key, then nullptr is returned.                                                */

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::number
         */
        Memcell* getelem_number_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::string
         */
        Memcell* getelem_string_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::boolean
         */
        Memcell* getelem_boolean_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::table
         */
        Memcell* getelem_table_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::userfunc
         */
        Memcell* getelem_userfunc_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::libfunc
         */
        Memcell* getelem_libfunc_key(Memcell const *key, bool auto_create);

        /**
         * Returns the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::nil
         */
        Memcell* getelem_nil_key(Memcell const *key, bool auto_create);

        /**
         * Always returns nullptr as the key itself is invalid
         * auto_create parameter is ignored
         */
        Memcell* getelem_undef_key(Memcell const *key, bool auto_create);

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "getelem_"  member functions
         */
        static const Getelem_func_map getelem_func_map;

        /* Functions that remove the Memcell associated with the given key, based on a FIXED key->type. */
        /* Note that these functions do not handle calling Memcell::clear() before removal, they just   */
        /* remove the link that points to the Memcell so it will be no longer accessible                */
        /* Also note that calling unregister() for a key not associated with any Table, is safe         */

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::number
         */
        void unregister_number_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::string
         */
        void unregister_string_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::boolean
         */
        void unregister_boolean_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::table
         */
        void unregister_table_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::userfunc
         */
        void unregister_userfunc_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::libfunc
         */
        void unregister_libfunc_key(Memcell const *key);

        /**
         * Unregisters the Memcell associated with the given key, which is considered to have key->type == Memcell::Type::nil
         */
        void unregister_nil_key(Memcell const *key);

        /**
         * Performs no action as the key itself is invalid
         */
        void unregister_undef_key(Memcell const *key);

        /**
         * Map that maps all Memcell::Type enumerated values to the corresponding "unregister_"  member functions
         */
        static const Unregister_func_map unregister_func_map;
    };
}


#endif //HY340_PROJECT_TABLE_H
