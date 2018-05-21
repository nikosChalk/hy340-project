#include <cassert>
#include "tcode_generator.h"
#include "target_instruction.h"

using namespace std;
using namespace target_code;

tcode_generator::tcode_generator(){
	instruction_vector = vector<target_instruction::instruction*>();
}

void tcode_generator::emit_instruction(target_instruction::instruction *instruction){
	assert(instruction);
	instruction_vector.push_back(instruction);
}

unsigned int tcode_generator::next_instruction_label() const {
	return (unsigned int)instruction_vector.size();
}