

#include <cassert>
#include <cstring>
#include <sstream>
#include <iostream>
#include "Memcell.h"
#include "Table.h"
#include "../utilities.h"

using namespace std;
using namespace virtual_machine;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Initialization of non-class members          */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static const std::map<unsigned int, std::string> type_to_str_map = {
        {Memcell::Type::number, "number"}, {Memcell::Type::string, "string"}, {Memcell::Type::boolean, "boolean"},
        {Memcell::Type::table, "table"}, {Memcell::Type::userfunc, "userfunc"}, {Memcell::Type::libfunc, "libfunc"},
        {Memcell::Type::nil, "nil"}, {Memcell::Type::undef, "undef"}
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Initialization of Memcell static class members   */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

const Memcell::Assign_func_map Memcell::assign_func_map = {
        {Type::number, &Memcell::assign_number}, {Type::string, &Memcell::assign_string}, {Type::boolean, &Memcell::assign_boolean},
        {Type::table, &Memcell::assign_table}, {Type::userfunc, &Memcell::assign_userfunc}, {Type::libfunc, &Memcell::assign_libfunc},
        {Type::nil, &Memcell::assign_nil}, {Type::undef, &Memcell::assign_undef}
};

const Memcell::Clear_func_map  Memcell::clear_func_map = {
        {Type::number, nullptr}, {Type::string, &Memcell::string_clear}, {Type::boolean, nullptr},
        {Type::table, &Memcell::table_clear}, {Type::userfunc, nullptr}, {Type::libfunc, nullptr},
        {Type::nil, nullptr}, {Type::undef, nullptr}
};

const Memcell::Tobool_func_map Memcell::tobool_func_map  = {
        {Type::number, &Memcell::number_tobool}, {Type::string, &Memcell::string_tobool}, {Type::boolean, &Memcell::boolean_tobool},
        {Type::table, &Memcell::table_tobool}, {Type::userfunc, &Memcell::userfunc_tobool}, {Type::libfunc, &Memcell::libfunc_tobool},
        {Type::nil, &Memcell::nil_tobool}, {Type::undef, &Memcell::undef_tobool}
};

const Memcell::Eqcheck_func_map Memcell::eqcheck_func_map  = {
        {Type::number, &Memcell::number_eqcheck}, {Type::string, &Memcell::string_eqcheck}, {Type::boolean, &Memcell::boolean_eqcheck},
        {Type::table, &Memcell::table_eqcheck}, {Type::userfunc, &Memcell::userfunc_eqcheck}, {Type::libfunc, &Memcell::libfunc_eqcheck},
        {Type::nil, &Memcell::nil_eqcheck}, {Type::undef, &Memcell::undef_eqcheck}
};

const Memcell::Tostr_func_map Memcell::tostr_func_map  = {
        {Type::number, &Memcell::number_tostr}, {Type::string, &Memcell::string_tostr}, {Type::boolean, &Memcell::boolean_tostr},
        {Type::table, &Memcell::table_tostr}, {Type::userfunc, &Memcell::userfunc_tostr}, {Type::libfunc, &Memcell::libfunc_tostr},
        {Type::nil, &Memcell::nil_tostr}, {Type::undef, &Memcell::undef_tostr}
};

std::string Memcell::type_to_string(Memcell::Type type) {
    return type_to_str_map.at(type);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Implementation of Memcell class              */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

Memcell::Memcell() {
    this->type = undef;
}

void Memcell::clear() {
    Clear_membfunc_ptr clear_func = Memcell::clear_func_map.at(this->type);

    if(clear_func != nullptr) {
        call_member_func_ptr(*this, clear_func)();
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
    call_member_func_ptr(*this, Memcell::assign_func_map.at(this->type))(rh);
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
    return call_member_func_ptr(*this, Memcell::tobool_func_map.at(this->type))();
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
    return call_member_func_ptr(*this, Memcell::eqcheck_func_map.at(this->type))(other);
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

/* To string functions */

std::string Memcell::to_string(const Constants_pool &const_pool) const {
    return call_member_func_ptr(*this, tostr_func_map.at(this->type))(const_pool);
}

std::string Memcell::number_tostr(const Constants_pool &unused) const {
    assert(this->type == Memcell::Type::number);
    return std::to_string(this->value.num);
}

std::string Memcell::string_tostr(const virtual_machine::Constants_pool &unused) const {
    assert(this->type == Memcell::Type::string);
    return std::string(this->value.str_ptr);
}

std::string Memcell::boolean_tostr(const Constants_pool &unused) const {
    assert(this->type == Memcell::Type::boolean);
    return (this->value.boolean) ? std::string("true") : std::string("false");
}

std::string Memcell::table_tostr(const Constants_pool &const_pool) const {
    assert(this->type == Memcell::Type::table);
    return this->value.table_ptr->to_string(const_pool);
}

std::string Memcell::userfunc_tostr(const Constants_pool &const_pool) const {
    assert(this->type == Memcell::Type::userfunc);
    return const_pool.get_userfunc_on_address(this->value.userfunc_addr).name;
}

std::string Memcell::libfunc_tostr(const Constants_pool &unused) const {
    assert(this->type == Memcell::Type::libfunc);
    return std::string(this->value.libfunc_ptr);
}

std::string Memcell::nil_tostr(const Constants_pool &unused) const {
    assert(this->type == Memcell::Type::nil);
    return std::string("nil");
}

std::string Memcell::undef_tostr(const Constants_pool &unused) const {
    assert(this->type == Memcell::Type::undef);
    return std::string("undef");
}
