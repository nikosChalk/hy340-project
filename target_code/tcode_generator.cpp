#include <cassert>
#include "tcode_generator.h"

using namespace std;
using namespace virtual_machine;
using namespace target_code;

tcode_generator::tcode_generator(vector<intermediate_code::quad*> quad_vector){
	num_Consts = vector<double>();
	total_Num_Consts = 0;
	string_Consts = vector <string>();
	total_String_Consts = 0;
	named_Lib_Funcs = vector <string>();
	total_Named_Lib_Funcs = 0;
	user_Funcs = vector <Userfunc>();
	total_User_Funcs = 0;
	this->instruction_vector = vector<VMinstruction*>();
	total_Instractions = 0;
	this->quad_vector = quad_vector;
}

void tcode_generator::emit_instruction(VMinstruction *instruction){
	assert(instruction);
	instruction_vector.push_back(instruction);
}

unsigned int tcode_generator::next_instruction_label() const {
	return (unsigned int)instruction_vector.size();
}
