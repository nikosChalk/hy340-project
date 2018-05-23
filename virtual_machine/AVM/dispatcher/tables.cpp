

#include <cassert>
#include "../AVM.h"
#include "../Memcell.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

void AVM::execute_newtable(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::newtable);

    Memcell *lv = translate_operand(instr.arg1);
    assert(lv && (program_stack.is_in_stack(lv) || lv == &retval));

    lv->clear();    //clear content since we will store here a new table
    lv->type = Memcell::Type::table;
    lv->value.table_ptr = new Table();
}

void AVM::execute_tablegetelem(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::tablegetelem);

    Memcell *lv = translate_operand(instr.result);  // lv = table[idx]
    Memcell *table = translate_operand(instr.arg1); //table is always a variable stored either in the stack or as a return value
    Memcell *idx = translate_operand(instr.arg2, &ax);

    assert(lv && (program_stack.is_in_stack(lv) || lv==&retval));
    assert(table && (program_stack.is_in_stack(table) || table == &retval));
    assert(idx);



    //Check if we have indeed a table
    if(table->type != Memcell::Type::table)
        throw internal_error("Illegal use of type " + Memcell::type_to_string(table->type) + " as table!");

    //Check if we are accessing with undef index and generate a warning in that case
    if(idx->type == Memcell::Type::undef)
        print_warning("Accessing table element with undef index", instr.source_line);

    //Get content
    Memcell *content = table->value.table_ptr->get_elem(idx);
    if(content) {   //element with the given index was found
        lv->assign(content);    //clearing of lv is handled internally
    } else {        //no element was found with the given index.
        lv->clear();    //Clear lvalue's old content manually
        lv->type = Memcell::Type::nil;  //default value when the element does not exist
        print_warning(table->to_string(const_pool) + "[" + idx->to_string(const_pool) + "] not found! Assigning nil instead.", instr.source_line);
    }
}

void AVM::execute_tablesetelem(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::tablesetelem);

    Memcell *table = translate_operand(instr.arg1);
    Memcell *idx = translate_operand(instr.arg2, &ax);
    Memcell *content = translate_operand(instr.result, &bx);    //table[idx] = content

    assert(table && program_stack.is_in_stack(table));  //table is always a variable stored ONLY in stack. Vlepe Dialeksh 15 slide 33/36 apo typwmenes shmeiwseis gia ton logo
    assert(idx && content);

    //Check if we have indeed a table
    if(table->type != Memcell::Type::table)
        throw internal_error("Illegal use of type " + Memcell::type_to_string(table->type) + " as table!");

    //Check if we are accessing with undef index and generate a warning in that case
    if(idx->type == Memcell::Type::undef)
        print_warning("Accessing table element with undef index", instr.source_line);

    //Check if we are setting undef content
    if(content->type == Memcell::Type::undef)
        print_warning("Assigning to table element from undef content", instr.source_line);

    //Set content
    table->value.table_ptr->set_elem(idx, content);
}



