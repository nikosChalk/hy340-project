#include "tcode_generator.h"
#include "../common_interface/vm_types.h"
#include <iostream>
#include <cassert>

void target_code::tcode_generator::generate(vector<intermediate_code::quad*> quad_vector){
	for (intermediate_code::quad *q : quad_vector){
		switch (q->opcode)
		{
		case intermediate_code::quad::iopcode::add:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::add, q);
		case intermediate_code::quad::iopcode::sub:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::sub, q);
		case intermediate_code::quad::iopcode::mul:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::mul, q);
		case intermediate_code::quad::iopcode::div:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::div, q);
		case intermediate_code::quad::iopcode::mod:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::mod, q);
		case intermediate_code::quad::iopcode::tablecreate:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::newtable, q);
		case intermediate_code::quad::iopcode::tablegetelem:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::tablegetelem, q);
		case intermediate_code::quad::iopcode::tablesetelem:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::tablesetelem, q);
		case intermediate_code::quad::iopcode::assign:
			target_code::tcode_generator::generate(virtual_machine::VMopcode::assign, q);
		case intermediate_code::quad::iopcode::jump:
			target_code::tcode_generator::generate_relational (virtual_machine::VMopcode::jump, q);
		case intermediate_code::quad::iopcode::if_eq:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jeq, q);
		case intermediate_code::quad::iopcode::if_noteq:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jne, q);
		case intermediate_code::quad::iopcode::if_greater:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jgt, q);
		case intermediate_code::quad::iopcode::if_greatereq:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jge, q);
		case intermediate_code::quad::iopcode::if_less:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jlt, q);
		case intermediate_code::quad::iopcode::if_lesseq:
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jle, q);
		case intermediate_code::quad::iopcode::param:
			target_code::tcode_generator::generate_PARAM(q);
		case intermediate_code::quad::iopcode::call:
			target_code::tcode_generator::generate_CALL(q);
		case intermediate_code::quad::iopcode::getretval:
			target_code::tcode_generator::generate_GETRETVAL(q);
		case intermediate_code::quad::iopcode::funcstart:
			target_code::tcode_generator::generate_FUNCSTART(q);
		case intermediate_code::quad::iopcode::funcend:
			target_code::tcode_generator::generate_FUNCEND(q);
		case intermediate_code::quad::iopcode::ret:
			target_code::tcode_generator::generate_RETURN(q);
		case intermediate_code::quad::iopcode::uminus:
			target_code::tcode_generator::generate_UMINUS(q);
		default:
			cerr << "Invalid opcode !!!\n";
			assert(0);
		}
	}
	target_code::tcode_generator::generate_NOP();
}

/*simplirosi generate functions*/

void target_code::tcode_generator::generate_UMINUS(intermediate_code::quad* quad){

}