

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include "Memcell.h"

using namespace std;
using namespace virtual_machine;

static const std::map<unsigned int, std::string> type_to_str_map = {
        {Memcell::Type::number, "number"}, {Memcell::Type::string, "string"}, {Memcell::Type::boolean, "boolean"},
        {Memcell::Type::table, "table"}, {Memcell::Type::userfunc, "userfunc"}, {Memcell::Type::libfunc, "libfunc"},
        {Memcell::Type::nil, "nil"}, {Memcell::Type::undef, "undef"}
};

std::string Memcell::type_to_string(Memcell::Type type) {
    return type_to_str_map.at(type);
}

Memcell::Memcell() {
    this->type = undef;
}

void Memcell::clear() {
    clear_func_t clear_func = Memcell::clear_func_table.at(this->type);

    if(clear_func != nullptr) {
        (this->*clear_func)();
        this->type = Type::undef;
    }
}

void Memcell::assign(const virtual_machine::Memcell *rh) {
    assert(rh);

    if(this == rh)  //Check if they are the same object. If yes, it is destructive to assign
        return;

    //Check if both are tables pointing to the same table
    if(this->type == Type::table && rh->type == Type::table &&
       this->value.table_ptr == rh->value.table_ptr) {
        return; //nothing to do in this case
    }

    this->clear();          //clear old data, releasing any acquired resources
    this->type = rh->type;
    ( this->*(Memcell::assign_func_table.at(this->type)) )(rh);
}

/* Assign functions */
void Memcell::assign_number(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::number);
    this->value.num = other->value.num;
}

void Memcell::assign_string(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::string);
    this->value.str_ptr = strdup(other->value.str_ptr);
}

void Memcell::assign_boolean(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::boolean);
    this->value.boolean = other->value.boolean;
}

void Memcell::assign_table(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::table);
    this->value.table_ptr = other->value.table_ptr;
    other->value.table_ptr->increase_ref_counter(); //since this->value is invalid when this method is call, it is safe  to increase reference counter
}

void Memcell::assign_userfunc(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::userfunc);
    this->value.userfunc_addr = other->value.userfunc_addr;
}

void Memcell::assign_libfunc(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::libfunc);
    this->value.libfunc_ptr = other->value.libfunc_ptr; //no duplicate string should be generated
}

void Memcell::assign_nil(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::nil);
    //Nothing to do here. noop
}

void Memcell::assign_undef(const Memcell *other) {
    assert(this->type == other->type && other->type == Type::undef);
}

/* Content clearing functions */
void Memcell::string_clear() {
    assert(this->type == Type::string);
    assert(this->value.str_ptr);

    free(this->value.str_ptr);
    this->value.str_ptr = nullptr;
}

void Memcell::table_clear() {
    assert(this->type == Type::table);
    assert(this->value.table_ptr);

    this->value.table_ptr->decrease_ref_counter();
    this->value.table_ptr = nullptr;
}

/* Bool Promotion */

bool Memcell::promote_to_bool() const {
    return ( this->*(Memcell::tobool_func_table.at(this->type)) )();
}

bool Memcell::number_tobool() const {
    assert(this->type == Type::number);
    return (this->value.num != 0);
}

bool Memcell::string_tobool() const {
    assert(this->type == Type::string);
    return !(std::string(this->value.str_ptr).empty());
}

bool Memcell::boolean_tobool() const {
    assert(this->type == Type::boolean);
    return this->value.boolean;
}

bool Memcell::table_tobool() const {
    assert(this->type == Type::table);
    return true;
}

bool Memcell::userfunc_tobool() const {
    assert(this->type == Type::userfunc);
    return true;
}

bool Memcell::libfunc_tobool() const {
    assert(this->type == Type::libfunc);
    return true;
}

bool Memcell::nil_tobool() const {
    assert(this->type == Type::nil);
    return false;
}

bool Memcell::undef_tobool() const {
    assert(this->type == Type::undef);
    assert(false);  //this function should never be called, since caller is trying to promote undef to bool.
}

/* Equality checks */

bool Memcell::operator==(const Memcell &other) const {
    if(this->type == Memcell::undef || other.type == Memcell::undef)
        throw std::runtime_error("Undefined operand in equality operation");

    //check if at least one is boolean
    if(this->type == Memcell::Type::boolean || other.type == Memcell::Type::boolean)
        return (this->promote_to_bool() == other.promote_to_bool());                   //If yes, perform true test

    //check if at least one is nil
    if(this->type == Memcell::Type::nil || other.type == Memcell::Type::nil)
        return (this->type == Memcell::Type::nil && other.type == Memcell::Type::nil);  //If yes, both nil ==> true, false otherwise

    //Check if both have the same type. If not, throw error. If yes, perform comparison based on type
    if(this->type != other.type) {
        stringstream ss;
        ss << "Illegal comparison of types: " << Memcell::type_to_string(this->type) << " == " << Memcell::type_to_string(other.type);
        throw std::runtime_error(ss.str());
    }

    //Now both have the same type. Perform equality check with dispatching
    assert(this->type == other.type);
    return (this->*(Memcell::eqcheck_func_table.at(this->type)) )(other);
}

bool Memcell::operator!=(const virtual_machine::Memcell &other) const {
    return !(*this == other);
}

bool Memcell::number_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::number);
    return this->value.num == other.value.num;
}

bool Memcell::string_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::string);
    return std::string(this->value.str_ptr) == std::string(other.value.str_ptr);
}

bool Memcell::boolean_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::boolean);
    return this->value.boolean == other.value.boolean;
}

bool Memcell::table_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::table);
    return this->value.table_ptr == other.value.table_ptr;
}

bool Memcell::userfunc_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::userfunc);
    return this->value.userfunc_addr == other.value.userfunc_addr;
}

bool Memcell::libfunc_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::libfunc);
    return std::string(this->value.libfunc_ptr) == std::string(other.value.libfunc_ptr);
}

bool Memcell::nil_eqcheck(const Memcell &other) const {
    assert(this->type == other.type && other.type == Type::nil);
    return true;
}

bool Memcell::undef_eqcheck(const virtual_machine::Memcell &other) const {
    assert(this->type == other.type && other.type == Type::undef);
    assert(false);
}