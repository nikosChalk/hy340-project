

#include <cassert>
#include <fstream>
#include <sstream>
#include "VMcode_generator.h"
#include "../syntax_analyzer/symbol_table.h"
#include "../common_interface/Constants.h"
#include "../common_interface/utilities.h"

using namespace std;
using namespace target_code;
using namespace virtual_machine;
using namespace intermediate_code;
using namespace syntax_analyzer;

const VMcode_generator::Generate_func_map VMcode_generator::generate_func_map = {
        {quad::iopcode::assign, &VMcode_generator::generate_ASSIGN},

        {quad::iopcode::add, &VMcode_generator::generate_ADD}, {quad::iopcode::sub, &VMcode_generator::generate_SUB},
        {quad::iopcode::mul, &VMcode_generator::generate_MUL}, {quad::iopcode::div, &VMcode_generator::generate_DIV},
        {quad::iopcode::mod, &VMcode_generator::generate_MOD}, {quad::iopcode::uminus, &VMcode_generator::generate_UMINUS},

        {quad::iopcode::if_eq, &VMcode_generator::generate_IF_EQ},
        {quad::iopcode::if_noteq, &VMcode_generator::generate_IF_NOTEQ},

        {quad::iopcode::if_lesseq, &VMcode_generator::generate_IF_LESSEQ},
        {quad::iopcode::if_greatereq, &VMcode_generator::generate_IF_GREATEREQ},

        {quad::iopcode::if_less, &VMcode_generator::generate_IF_LESS},
        {quad::iopcode::if_greater, &VMcode_generator::generate_IF_GREATER},

        {quad::iopcode::call, &VMcode_generator::generate_CALL}, {quad::iopcode::param, &VMcode_generator::generate_PARAM},
        {quad::iopcode::ret, &VMcode_generator::generate_RET}, {quad::iopcode::getretval, &VMcode_generator::generate_GETRETVAL},
        {quad::iopcode::funcstart, &VMcode_generator::generate_FUNCSTART}, {quad::iopcode::funcend, &VMcode_generator::generate_FUNCEND},

        {quad::iopcode::tablecreate, &VMcode_generator::generate_TABLECREATE},
        {quad::iopcode::tablegetelem, &VMcode_generator::generate_TABLEGETELEM},
        {quad::iopcode::tablesetelem, &VMcode_generator::generate_TABLESETELEM},

        {quad::iopcode::jump, &VMcode_generator::generate_JUMP},
};

VMcode_generator::VMcode_generator(vector<intermediate_code::quad*> const &quad_vector) {
    this->quad_vector = vector<intermediate_code::quad*>(quad_vector);
    const_pool = Constants_pool();
    vm_instructions = vector<VMinstruction>();
	incomplete_jumps = vector<Incomplete_jump>();
    address_links = map<unsigned int, Address_link>();;   //key == quad address == value.get_quad_label()
	func_stack = stack<vector<unsigned int>>();
	jumps_before_funcenter = stack<unsigned int>();
	curr_proc_quad = 0;
}

void VMcode_generator::generate_target_code() {

    //Start generation process
    for(curr_proc_quad=0; curr_proc_quad<quad_vector.size(); curr_proc_quad++) {
        quad const *cur_quad = quad_vector.at(curr_proc_quad);

        address_links[curr_proc_quad] = Address_link(curr_proc_quad, next_vm_instr_address());
        call_member_func_ptr(*this, VMcode_generator::generate_func_map.at(cur_quad->opcode)) (cur_quad);
    }

    //Generate ending "nop" VM instruction
    //Set the "nop" source line, as the last source line of the program. If the program is empty then set the line to 0
    unsigned int nop_src_line = ( (quad_vector.empty()) ? 0 : (quad_vector.at(quad_vector.size()-1)->lineno) );
    generate_NOP(nop_src_line);

    //Patch incomplete jumps
    for(Incomplete_jump incomp_jump : incomplete_jumps){
        if(incomp_jump.get_quad_target_label() == quad_vector.size()) {//qad jump to EOF <==> jump to the last "nop" VM instruction
            vm_instructions.at(incomp_jump.get_vm_instr_source_label()).result->value = (unsigned int) vm_instructions.size();
        } else {
            unsigned int address_to_patch = incomp_jump.get_vm_instr_source_label();
            unsigned int quad_target_address = incomp_jump.get_quad_target_label();
            vm_instructions.at(address_to_patch).result->value = address_links.at(quad_target_address).get_vm_instr_label();
        }
    }
}

vector<VMinstruction> VMcode_generator::get_vm_instr_vector() const {
    return vm_instructions;
}
Constants_pool VMcode_generator::get_const_pool() const {
    return const_pool;
}

void VMcode_generator::emit(VMinstruction const &instruction) {
	vm_instructions.push_back(instruction);
}
unsigned int VMcode_generator::next_vm_instr_address() const {
	return (unsigned int) vm_instructions.size();
}

VMarg* VMcode_generator::translate_operand(intermediate_code::expr const *expr){
    if(!expr)
        return nullptr;

    VMarg *arg = new VMarg();
	switch (expr->expr_type) {

	    //Handle cases that use variables for storage
        //Arg will have a type of either program, formal, local
		case intermediate_code::expr::type::TABLE_ITEM_E:
		case intermediate_code::expr::type::ARITHM_E:
		case intermediate_code::expr::type::ASSIGN_E:
		case intermediate_code::expr::type::NEW_TABLE_E:
		case intermediate_code::expr::type::VAR_E: {
            assert(expr->sym_entry);

            symbol_table::var_entry const *sym_var_entry = dynamic_cast<symbol_table::var_entry*>(expr->sym_entry);
            assert(sym_var_entry);

            //Set offset
            arg->value = sym_var_entry->get_scope_space_offset();

            //Set scope space
            switch(sym_var_entry->get_scope_space()) {
                case scope_handler::PROGRAM_SS:
                    arg->type = VMarg::Type::program;
                    break;
                case scope_handler::FUNCTION_LOCALS_SS:
                    arg->type = VMarg::Type::local;
                    break;
                case scope_handler::FORMAL_ARGUMENTS_SS:
                    arg->type = VMarg::Type::formal;
                    break;
                default:
                    assert(0);  //unreachable statement
            } //end of sub-switch
            return arg;
        } //end of case

		//Handle constants
		case expr::type::CONST_BOOL_E:
		    arg->type = VMarg::Type::boolean;
			arg->value = ((expr->const_val.boolean) ? 1 : 0);
            break;
		case expr::type::CONST_STR_E:
            arg->type = VMarg::Type::string;
			arg->value = const_pool.register_string(expr->const_val.str);
            break;
		case expr::type::CONST_NUM_E:
            arg->type = VMarg::Type::number;
			arg->value = const_pool.register_number(expr->const_val.number);
            break;
		case expr::type::CONST_NIL_E:
			arg->type = VMarg::Type::nil;
            break;

		/* Functions */
		case expr::type::PROGRAM_FUNC_E:
        case expr::type::LIBRARY_FUNC_E: {
		    assert(expr->sym_entry);
		    symbol_table::func_entry const *sym_func_entry = dynamic_cast<symbol_table::func_entry*>(expr->sym_entry);
		    assert(sym_func_entry);

		    switch(expr->expr_type) {
                case expr::type::PROGRAM_FUNC_E:
                    arg->type = VMarg::Type::userfunc;
                    arg->value = const_pool.register_userfunc(Userfunc(

                            //first VM instruction generated by quad "funcstart" is "jump" while the second one is "funcenter". Thus we do a +1
                            address_links.at(sym_func_entry->get_iaddress()).get_vm_instr_label()+1,
                            sym_func_entry->get_total_locals(),
                            sym_func_entry->get_name()
                    ));
                    break;

                case expr::type::LIBRARY_FUNC_E:
                    arg->type = VMarg::Type::libfunc;
                    arg->value = const_pool.register_libfunc(sym_func_entry->get_name());
                    break;
                default:
                    assert(false);  //Unreachable statement
		    }   //end of sub-switch
            return arg;
        }   //end of case

        case expr::type::BOOL_E:    //Due to short-circuit evaluation, we do not emit any quad with expr::type BOOL_E
		default:
			assert(false);
	}
    return arg;
}

VMarg* VMcode_generator::translate_retval() {
    VMarg *arg = new VMarg();
	arg->type = VMarg::Type::retval;
    return arg;
}

VMarg* VMcode_generator::translate_number(double long value) {
    VMarg *arg = new VMarg();
    arg->type = VMarg::Type::number;
    arg->value = const_pool.register_number(value);
    return arg;
}