#include <cassert>
#include "tcode_generator.h"
#include "../syntax_analyzer/symbol_table.h"

using namespace std;
using namespace virtual_machine;
using namespace target_code;

tcode_generator::tcode_generator(vector<intermediate_code::quad*> quad_vector){
	this->num_Consts = vector<double>();
	this->string_Consts = vector <string>();
	this->named_Lib_Funcs = vector <string>();
	this->user_Funcs = vector <Userfunc*>();
	this->instruction_vector = vector<VMinstruction*>();
	this->quad_vector = quad_vector;
	this->incomplete_jump_vector = vector<Incomplete_Jump*>();
}

void tcode_generator::emit_instruction(VMinstruction *instruction){
	assert(instruction);
	instruction_vector.push_back(instruction);
}

unsigned int tcode_generator::next_instruction_label() const {
	return (unsigned int)instruction_vector.size();
}

void tcode_generator::make_operand(intermediate_code::expr* e, VMarg* arg){
	switch (e->expr_type)
	{
	case intermediate_code::expr::type::TABLE_ITEM_E:
	case intermediate_code::expr::type::ARITHM_E:
	case intermediate_code::expr::type::BOOL_E:
	case intermediate_code::expr::type::NEW_TABLE_E:
	case intermediate_code::expr::type::VAR_E:
		assert(e->sym_entry);
		arg->value = dynamic_cast<syntax_analyzer::symbol_table::var_entry*>(e->sym_entry)->get_scope_space_offset();
		switch (dynamic_cast<syntax_analyzer::symbol_table::var_entry*>(e->sym_entry)->get_scope_space)
		{
		case syntax_analyzer::scope_handler::PROGRAM_SS:
			arg->type = virtual_machine::VMarg::Type::program;
			break;
		case syntax_analyzer::scope_handler::FUNCTION_LOCALS_SS:
			arg->type = virtual_machine::VMarg::Type::local;
			break;
		case syntax_analyzer::scope_handler::FORMAL_ARGUMENTS_SS:
			arg->type = virtual_machine::VMarg::Type::formal;
			break;
		default:
			assert(0);
		}
		break;
	/* Constants */
	case intermediate_code::expr::type::CONST_BOOL_E:
		arg->value = e->const_val.boolean; /*TODO:validate this*/
		arg->type = virtual_machine::VMarg::Type::boolean;
		break;
	case intermediate_code::expr::type::CONST_STR_E:
		arg->value = tcode_generator::consts_new_string(e->const_val.str);
		arg->type = virtual_machine::VMarg::Type::string;
		break;
	case intermediate_code::expr::type::CONST_NUM_E:
		arg->value = tcode_generator::consts_new_number(e->const_val.number);
		arg->type = virtual_machine::VMarg::Type::number;
		break;
	case intermediate_code::expr::type::CONST_NIL_E:
		arg->type = virtual_machine::VMarg::Type::nil;
		break;
	/* Functions */
	case intermediate_code::expr::type::PROGRAM_FUNC_E:
		arg->type = virtual_machine::VMarg::Type::userfunc;
		arg->value = tcode_generator::userfuncs__new_func(e->sym_entry);
		break;
	case intermediate_code::expr::type::LIBRARY_FUNC_E:
		arg->type = virtual_machine::VMarg::Type::libfunc;
		arg->value = tcode_generator::libfuncs_new_used(e->sym_entry->get_name());
		break;
	default:
		assert(0);
	}
}

void tcode_generator::make_retval_operand(VMarg* arg){
	arg->type = virtual_machine::VMarg::Type::retval;
}

unsigned int tcode_generator::consts_new_string(string s){
	string_Consts.push_back(s);
	return string_Consts.size() - 1;
}
unsigned int tcode_generator::consts_new_number(double n){
	num_Consts.push_back(n);
	return num_Consts.size() - 1;
}
unsigned int tcode_generator::libfuncs_new_used(string s){
	named_Lib_Funcs.push_back(s);
	return named_Lib_Funcs.size() - 1;
}
unsigned int tcode_generator::userfuncs__new_func(syntax_analyzer::symbol_table::entry* sym){
	user_Funcs.push_back(
		new Userfunc(
		dynamic_cast<syntax_analyzer::symbol_table::func_entry*>(sym)->get_iaddress(),
		dynamic_cast<syntax_analyzer::symbol_table::func_entry*>(sym)->get_total_locals(),
		dynamic_cast<syntax_analyzer::symbol_table::func_entry*>(sym)->get_name()
		)
	);
	return user_Funcs.size() - 1;
}

void tcode_generator::patch_incomplete_jumps(void){
	/*TODO:fill*/
}
void tcode_generator::add_incomplete_jump(unsigned int instrNo, unsigned int iaddress){
	incomplete_jump_vector.push_back(new Incomplete_Jump(instrNo,iaddress));
}

