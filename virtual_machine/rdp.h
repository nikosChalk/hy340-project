#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include "../target_code/target_instruction.h"

class rdp
{
public:
	/*rdp parser constructor*/
	rdp();

	/*constants arrays and instruction array*/
	std::vector <double> num_Consts;
	unsigned int total_Num_Consts;
	std::vector <std::string> string_Consts;
	unsigned int total_String_Consts;
	std::vector <std::string> named_Lib_Funcs;
	unsigned int total_Named_Lib_Funcs;
	std::vector <target_code::target_instruction::user_func> user_Funcs;
	unsigned int total_User_Funcs;
	std::vector <target_code::target_instruction::instruction> instr_code_vector;
	unsigned int total_Instractions;

	/*helper function*/
	target_instruction::vmopcode opcode_type(unsigned int);
	target_instruction::vmarg_t vmarg_type(unsigned int);

	/*basic parsing function*/
	void RDP_Parser(void);
};


