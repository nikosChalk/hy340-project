

#include "types.h"

using namespace target_code;

Incomplete_jump::Incomplete_jump(unsigned int quad_target_label, unsigned int vm_instr_source_label)
        : quad_target_label(quad_target_label), vm_instr_source_label(vm_instr_source_label)
{ }

unsigned int Incomplete_jump::get_quad_target_label() const {
    return quad_target_label;
}

unsigned int Incomplete_jump::get_vm_instr_source_label() const {
    return vm_instr_source_label;
}

Address_link::Address_link(unsigned int quad_label, unsigned int vm_instr_label)
        : quad_label(quad_label), vm_instr_label(vm_instr_label)
{ }

unsigned int Address_link::get_quad_label() const {
    return quad_label;
}

unsigned int Address_link::get_vm_instr_label() const {
    return vm_instr_label;
}