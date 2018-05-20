

#include <cassert>
#include "Memcell.h"

using namespace virtual_machine;

Memcell::Memcell() {
    this->type = undef;
}

void Memcell::clear() {
    if(this->type == Memcell::Type::table) {
        assert(value.table_ptr);
        value.table_ptr = value.table_ptr->decrease_ref_counter();
    }
    this->type = undef;
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

    this->clear();
    this->type = rh->type;
    switch(rh->type) {
        case Type::number:
            this->value.num = rh->value.num;
            return;
        case Type::string:
            this->value.str_ptr = rh->value.str_ptr;
            return;
        case Type::boolean:
            this->value.boolean = rh->value.boolean;
            return;
        case Type::table:
            this->value.table_ptr = rh->value.table_ptr;
            rh->value.table_ptr->increase_ref_counter();
            return;
        case Type::userfunc:
            this->value.userfunc_addr = rh->value.userfunc_addr;
            return;
        case Type::libfunc:
            this->value.libfunc_ptr = rh->value.libfunc_ptr;
            return;
        case Type::address:
            this->value.jump_addr = rh->value.jump_addr;
            return;
        case Type::nil:
        case Type::undef:
            return;

        default:
            assert(false);  //Unreachable statement
    }
}