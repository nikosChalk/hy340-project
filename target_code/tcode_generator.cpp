#include <cassert>
#include "tcode_generator.h"

using namespace std;
using namespace virtual_machine;
using namespace target_code;

tcode_generator::tcode_generator(){
	instruction_vector = vector<VMinstruction*>();
}

void tcode_generator::emit_instruction(VMinstruction *instruction){
	assert(instruction);
	instruction_vector.push_back(instruction);
}

unsigned int tcode_generator::next_instruction_label() const {
	return (unsigned int)instruction_vector.size();
}