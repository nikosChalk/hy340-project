#include "tcode_generator.h"
#include "../common_interface/vm_types.h"
#include <iostream>
#include <cassert>

void target_code::tcode_generator::generate(vector<target_code::Tcode_quad*> quad_vector){
	for (target_code::Tcode_quad *q : quad_vector){
		switch (q->tquad->opcode)
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
			target_code::tcode_generator::generate_relational(virtual_machine::VMopcode::jump, q);
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
		this->curr_proc_quad++; /*counter for curr_processed_quad() function*/
	}
	target_code::tcode_generator::generate_NOP();
}

/*helper generate functions*/
void target_code::tcode_generator::generate(VMopcode opcode, target_code::Tcode_quad* quad){
	VMinstruction instruction;
	instruction.opcode = opcode;

	make_operand(quad->tquad->arg1, instruction.arg1);
	make_operand(quad->tquad->arg2, instruction.arg2);
	make_operand(quad->tquad->result, instruction.result);
	quad->taddress = next_instruction_label();
	emit_instruction(instruction);
}

void target_code::tcode_generator::generate_relational(VMopcode opcode, target_code::Tcode_quad* quad){
	VMinstruction instruction;
	instruction.opcode = opcode;

	make_operand(quad->tquad->arg1, instruction.arg1);
	make_operand(quad->tquad->arg2, instruction.arg2);

	instruction.result->type = VMarg::Type::label;


	if (quad->tquad->label < curr_processed_quad())
		instruction.result->value = quad_vector.at(quad->tquad->label)->taddress;
	else
		add_incomplete_jump(next_instruction_label(), quad->tquad->label);
	quad->taddress = next_instruction_label();
	emit_instruction(instruction);
}

void target_code::tcode_generator::generate_PARAM(target_code::Tcode_quad* quad){
	quad->taddress = next_instruction_label();
	VMinstruction instruction;
	instruction.opcode = VMopcode::pusharg;
	make_operand(quad->tquad->arg1, instruction.arg1);
	emit_instruction(instruction);
}

void target_code::tcode_generator::generate_CALL(target_code::Tcode_quad* quad){
	quad->taddress = next_instruction_label();
	VMinstruction instruction;
	instruction.opcode = VMopcode::call;
	make_operand(quad->tquad->arg1, instruction.arg1);
	emit_instruction(instruction);
}

void target_code::tcode_generator::generate_GETRETVAL(target_code::Tcode_quad* quad){
	quad->taddress = next_instruction_label();
	VMinstruction instruction;
	instruction.opcode = VMopcode::assign;
	make_operand(quad->tquad->result, instruction.result);
	make_retval_operand(instruction.arg1);
	emit_instruction(instruction);
}

void target_code::tcode_generator::generate_UMINUS(target_code::Tcode_quad* quad){
	VMinstruction instruction;
	int flag = 0;
	instruction.opcode = VMopcode::mul;
	make_operand(quad->tquad->arg1, instruction.arg1);
	for (int i = 0; i < num_Consts.size(); i++){
		if (num_Consts.at(i) == -1){
			instruction.arg2->value = i;
			instruction.arg2->type = virtual_machine::VMarg::Type::number;
			flag = 1;/*we have a -1 record in constants number array*/
			break;
		}
	}
	if (flag == 0){ /*make a new record in constants number array*/
		instruction.arg2->value = tcode_generator::consts_new_number(-1);
		instruction.arg2->type = virtual_machine::VMarg::Type::number;
	}
	make_operand(quad->tquad->result, instruction.result);
	emit_instruction(instruction);
}