

#include <cassert>
#include <cstring>
#include <vector>
#include <functional>
#include <sstream>
#include "Table.h"
#include "Memcell.h"
#include "../../common_interface/utilities.h"

using namespace std;
using namespace virtual_machine;

Table::Table() {
    ref_counter = 1;
    numericMap = map<long double, Memcell>();
    stringMap = map<std::string, Memcell>();
    tableMap = map<Table*, Memcell>();
    userfuncMap = map<unsigned int, Memcell>();
    libfuncMap = map<char const*, Memcell>();
    boolMap = unordered_map<bool, Memcell>();

    nil_pair = make_pair(Memcell(), false);
}

Table::Table(Table const *other) {
	this->ref_counter = 1;
	this->numericMap = map<long double, Memcell>(other->numericMap);

    this->stringMap = map<string, Memcell>(other->stringMap);
	for (auto &pair : this->stringMap)
		pair.second.value.str_ptr = strdup(pair.second.value.str_ptr);

	this->tableMap = map<Table*, Memcell>(other->tableMap);
	for (auto &pair : this->tableMap)
		pair.second.value.table_ptr->increase_ref_counter();

	this->userfuncMap = map<unsigned int, Memcell>(other->userfuncMap);
	this->libfuncMap = map<char const*, Memcell>(other->libfuncMap);
	this->boolMap = unordered_map<bool, Memcell>(other->boolMap);
	this->nil_pair = std::pair<Memcell, bool>(other->nil_pair);
}

const Table::Getelem_func_map Table::getelem_func_map = {
        {Memcell::Type::number, &Table::getelem_number_key}, {Memcell::Type::string, &Table::getelem_string_key},
        {Memcell::Type::boolean, &Table::getelem_boolean_key}, {Memcell::Type::table, &Table::getelem_table_key},
        {Memcell::Type::userfunc, &Table::getelem_userfunc_key}, {Memcell::Type::libfunc, &Table::getelem_libfunc_key},
        {Memcell::Type::nil, &Table::getelem_nil_key}, {Memcell::Type::undef, &Table::getelem_undef_key},
};

const Table::Unregister_func_map Table::unregister_func_map = {
        {Memcell::Type::number, &Table::unregister_number_key}, {Memcell::Type::string, &Table::unregister_string_key},
        {Memcell::Type::boolean, &Table::unregister_boolean_key}, {Memcell::Type::table, &Table::unregister_table_key},
        {Memcell::Type::userfunc, &Table::unregister_userfunc_key}, {Memcell::Type::libfunc, &Table::unregister_libfunc_key},
        {Memcell::Type::nil, &Table::unregister_nil_key}, {Memcell::Type::undef, &Table::unregister_undef_key},
};

void Table::increase_ref_counter() {
    ref_counter++;
}

Table* Table::decrease_ref_counter() {
    assert(ref_counter>0);  //If ref_counter becomes 0 then the table must be destroyed

    ref_counter--;
    if(ref_counter == 0) {
        delete this;    //call destructor. For more info, vlepe stack overflow answer: https://stackoverflow.com/a/7039749
        return nullptr;
    } else {
        return this;
    }
}

//Clears the given map. key is it->first, value is it->second
#define clear_map(map)  for(auto it=(map).begin(); it!=(map).end(); it++) \
                            it->second.clear()
Table::~Table() {

    //TODO: validate that this works

    //Clear all memcells from all maps
    clear_map(numericMap);
    clear_map(stringMap);
    clear_map(tableMap);
    clear_map(userfuncMap);
    clear_map(libfuncMap);
    clear_map(boolMap);

    //Clear enumerated memcells
    if(nil_pair.second)
        nil_pair.first.clear();
}


Memcell* Table::get_elem(Memcell const *idx) {
    assert(idx);
    return (call_member_func_ptr(*this, Table::getelem_func_map.at(idx->type))(idx, false));
}

void Table::set_elem(const Memcell *idx, const Memcell *val) {
    assert(idx && val);
    Memcell *to_set_elem = call_member_func_ptr(*this, Table::getelem_func_map.at(idx->type))(idx, true);

    if(to_set_elem) {   //for idx->type == Memcell::Type::undef, nullptr is returned even with auto_create.

        if(val->type == Memcell::Type::nil) {   //check if this is a removal operation
            to_set_elem->clear();       //we clear the cell as it could already exist
            call_member_func_ptr(*this, Table::unregister_func_map.at(idx->type))(idx); //If yes, unregister the associated value
        } else {
            to_set_elem->assign(val);   //Otherwise, perform table[idx] = val;. Assign calls to_set_elem->clear() internally
        }
    }
}

Table* Table::get_keys() const {
	unsigned int key_counter = 0;

	Table *table = new Table(); //table that will be returned and that contains the keys of *this
    Memcell *tmp = new Memcell();   //temporary helper variable


	/*numericMap*/
    tmp->type = Memcell::Type::number;
	for (auto const &pair : this->numericMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.num = pair.first;    //long double

        lvalue.assign(tmp);
	}

	/*stringMap*/
    tmp->type = Memcell::Type::string;
	for (auto const &pair : this->stringMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.str_ptr = strdup(pair.first.c_str());

        lvalue.assign(tmp);         //will create internally an strdup of "pair.first"
        free(tmp->value.str_ptr);
	}

	/*tableMap*/
    tmp->type = Memcell::Type::table;
	for (auto const &pair : this->tableMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.table_ptr = pair.first;

        lvalue.assign(tmp); //will also increase reference counter of table "pair.first"
	}

	/*userfuncMap*/
    tmp->type = Memcell::Type::userfunc;
	for (auto const &pair : this->userfuncMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.userfunc_addr = pair.first;

        lvalue.assign(tmp);
	}
	/*libfuncMap*/
    tmp->type = Memcell::Type::libfunc;
	for (auto const &pair : this->libfuncMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.libfunc_ptr = pair.first;

        lvalue.assign(tmp);
	}

	/*boolMap*/
    tmp->type = Memcell::Type::boolean;
	for (auto const &pair : this->boolMap) {
        Memcell &lvalue = table->numericMap[key_counter++];
        tmp->value.boolean = pair.first;

        lvalue.assign(tmp);
	}

	/*nil_pair*/
	if(this->nil_pair.second) {
        tmp->type = Memcell::Type::nil;

        Memcell &lvalue = table->numericMap[key_counter++];
        lvalue.assign(tmp);
	}

	delete tmp;
    return table;
}

unsigned int Table::get_nr_elements() const {
	unsigned int retval = (unsigned int) (
	        numericMap.size() +
            stringMap.size() +
            tableMap.size() +
            userfuncMap.size() +
            libfuncMap.size() +
            boolMap.size()
    );

	if(nil_pair.second)
		retval++;
	return retval;
}

/* getelem_ functions */

Memcell* Table::getelem_number_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::number);
    double long dl_key = key->value.num;

    if(auto_create)
        return &numericMap[dl_key]; //If the value ath the index does not exist, it automatically creates a new value and returns a reference to it.

    auto search = numericMap.find(dl_key);
    return (search != numericMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_string_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::string);
    string str_key = string(key->value.str_ptr);

    if(auto_create)
        return &stringMap[str_key];

    auto search = stringMap.find(str_key);
    return (search != stringMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_boolean_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::boolean);
    bool bool_key = key->value.boolean;

    if(auto_create)
        return &boolMap[bool_key];

    auto search = boolMap.find(bool_key);
    return (search != boolMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_table_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::table);
    Table *table_key = key->value.table_ptr;

    if(auto_create)
        return &tableMap[table_key];

    auto search = tableMap.find(table_key);
    return (search != tableMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_userfunc_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::userfunc);
    unsigned int userfunc_key = key->value.userfunc_addr;

    if(auto_create)
        return &userfuncMap[userfunc_key];

    auto search = userfuncMap.find(userfunc_key);
    return (search != userfuncMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_libfunc_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::libfunc);
    char const *libfunc_key = key->value.libfunc_ptr;

    if(auto_create)
        return &libfuncMap[libfunc_key];

    auto search = libfuncMap.find(libfunc_key);
    return (search != libfuncMap.end()) ? &search->second : nullptr;
}

Memcell* Table::getelem_nil_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::nil);

    if(auto_create)
        nil_pair.second = true;
    return (nil_pair.second) ? &nil_pair.first : nullptr;
}

Memcell* Table::getelem_undef_key(const Memcell *key, bool auto_create) {
    assert(key->type == Memcell::Type::undef);
    return nullptr;
}

/* unregister functions */
#define unregister(map, key) ((map).erase(key))
void Table::unregister_number_key(const Memcell *key) {
    assert(key->type == Memcell::Type::number);
    unregister(numericMap, key->value.num);
}

void Table::unregister_string_key(const Memcell *key) {
    assert(key->type == Memcell::Type::string);
    unregister(stringMap, string(key->value.str_ptr));
}

void Table::unregister_boolean_key(const Memcell *key) {
    assert(key->type == Memcell::Type::boolean);
    unregister(boolMap, key->value.boolean);
}

void Table::unregister_table_key(const Memcell *key) {
    assert(key->type == Memcell::Type::table);
    unregister(tableMap, key->value.table_ptr);
}

void Table::unregister_userfunc_key(const Memcell *key) {
    assert(key->type == Memcell::Type::userfunc);
    unregister(userfuncMap, key->value.userfunc_addr);
}

void Table::unregister_libfunc_key(const Memcell *key) {
    assert(key->type == Memcell::Type::libfunc);
    unregister(libfuncMap, key->value.libfunc_ptr);
}

void Table::unregister_nil_key(const Memcell *key) {
    assert(key->type == Memcell::Type::nil);
    nil_pair.second = false;
}

void Table::unregister_undef_key(const Memcell *key) {
    assert(key->type == Memcell::Type::undef);
    return; //no action
}

#define second_tostr() (it->second.to_string(const_pool))
string Table::to_string(Constants_pool const &const_pool) const {
    stringstream ss;
    const string prefix = "\t{ ";
    const string midfix = " : ";
    const string suffix = " },";

    ss << "[" << endl;

    //Numeric map
    for(auto it=numericMap.cbegin(); it != numericMap.cend(); it++)
        ss << prefix << it->first << midfix << second_tostr() << suffix << endl;

    //String map
    for(auto it=stringMap.cbegin(); it != stringMap.cend(); it++)
        ss << prefix << it->first << midfix << second_tostr() << suffix << endl;

    //Table map. This can lead to endless recursive calls that we want to definately avoid
    for(auto it=tableMap.cbegin(); it != tableMap.cend(); it++) {
        ss << prefix;

        //Do not recursively print self. (e.g. when we have table[table] = 5; print(table);)
        if(it->first == this)
            ss << "__self__";
        else
            ss << it->first->to_string(const_pool);
        ss << midfix;

        //Again, do not recursively print self. (e.g. when we have table[5] = table; print(table);)
        if(it->second.type == Memcell::Type::table && it->second.value.table_ptr == this)
            ss << "__self__";
        else
            ss << second_tostr();

        ss  << suffix << endl;
    }

    //Userfunc map
    for(auto it=userfuncMap.cbegin(); it != userfuncMap.cend(); it++)
        ss << prefix << const_pool.get_userfunc_on_address(it->first).name << midfix << second_tostr() << suffix << endl;

    //Libfunc map
    for(auto it=libfuncMap.cbegin(); it != libfuncMap.cend(); it++)
        ss << prefix << std::string(it->first) << midfix << second_tostr() << suffix << endl;

    //Bool map
    for(auto it=boolMap.cbegin(); it != boolMap.cend(); it++)
        ss << prefix << ((it->first) ? "true" : "false") << midfix << second_tostr() << suffix << endl;

    //Nil
    if(nil_pair.second)
        ss << prefix << "nil" << midfix << nil_pair.first.to_string(const_pool) << suffix << endl;

    //Reference counter
    ss << "\tReference Counter: " << ref_counter << endl;
    ss << "]" << endl;
    return ss.str();
}

