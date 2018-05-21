#include "rdp.h"

using namespace std;
using namespace virtual_machine;

/*RDP constructor*/
rdp::rdp()
{
	num_Consts = vector<double>();
	total_Num_Consts = 0;
	string_Consts = vector <string>();
	total_String_Consts = 0;
	named_Lib_Funcs = vector <string>();
	total_Named_Lib_Funcs = 0;
	user_Funcs = vector <Userfunc>();
	total_User_Funcs = 0;
	instr_code_vector = vector <VMinstruction>();
	total_Instractions = 0;
}

VMopcode rdp::opcode_type(unsigned int type){
	switch (type) {
	    case VMopcode::assign:

        case VMopcode::add:
        case VMopcode::sub:
        case VMopcode::mul:
        case VMopcode::div:
        case VMopcode::mod:

        case VMopcode::jeq:
        case VMopcode::jne:
        case VMopcode::jle:
        case VMopcode::jge:
        case VMopcode::jlt:
        case VMopcode::jgt:

        case VMopcode::call:
	    case VMopcode::pusharg:
	    case VMopcode::funcenter:
	    case VMopcode::funcexit:

	    case VMopcode::newtable:
	    case VMopcode::tablegetelem:
	    case VMopcode::tablesetelem:

	    case VMopcode::jump:
	    case VMopcode::nop:
            return static_cast<VMopcode>(type);
        default:
            cerr << "Invalid opcode !!!\n";
            assert(0);
	}
}

VMarg::Type rdp::vmarg_type(unsigned int type){
	switch(type) {
        case VMarg::Type::label:
        case VMarg::Type::program:
        case VMarg::Type::formal:
        case VMarg::Type::local:
        case VMarg::Type::number:
        case VMarg::Type::string:
        case VMarg::Type::boolean:
        case VMarg::Type::nil:
        case VMarg::Type::userfunc:
        case VMarg::Type::libfunc:
        case VMarg::Type::retval:
            return static_cast<VMarg::Type >(type);
        default:
            cerr << "Invalid vm argument type !!!\n";
            assert(0);
	}
}

void rdp::RDP_Parser() {
	string tmp_string;
	double tmp_double;
	unsigned int tmp_uint;
	Userfunc tmp_user_func;
	VMinstruction tmp_instr;


	ifstream binary_f;

	binary_f.open("binary.abc", ios::in | ios::binary);
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));

	/*check magicnumber*/
	if (tmp_uint != 340200501){
		cerr << "Error !!! no alpha binary code file";
		assert(0);
	}

	/*load constants arrays (strings)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	this->total_String_Consts = tmp_uint;
	for (unsigned int i = 0; i < this->total_String_Consts; i++){
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_string, tmp_uint); /*a string with tmp_uint bytes*/
		this->string_Consts.push_back(tmp_string); /*push string in vector*/
	}

	/*load constants arrays (numbers)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	this->total_Num_Consts = tmp_uint;
	for (unsigned int i = 0; i < this->total_Num_Consts; i++){
		binary_f.read((char *)&tmp_double, sizeof(double));
		this->num_Consts.push_back(tmp_double); /*push number in vector*/
	}

	/*load constants arrays (userfunctions)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	this->total_User_Funcs = tmp_uint;
	for (unsigned int i = 0; i < this->total_User_Funcs; i++){
		binary_f.read((char *)&tmp_user_func.address, sizeof(unsigned int));
		binary_f.read((char *)&tmp_user_func.nr_locals, sizeof(unsigned int));
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_user_func.name, tmp_uint); /*a string with tmp_uint bytes*/
		this->user_Funcs.push_back(tmp_user_func); /*push user_function in vector*/
	}

	/*load constants arrays (libraryfunctions)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	this->total_Named_Lib_Funcs = tmp_uint;
	for (unsigned int i = 0; i < this->total_Named_Lib_Funcs; i++){
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_string, tmp_uint); /*a string with tmp_uint bytes*/
		this->named_Lib_Funcs.push_back(tmp_string); /*push string in vector*/
	}

	/*load code instructions*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	this->total_Instractions = tmp_uint;
	for (unsigned int i = 0; i < this->total_Instractions; i++){
		/*opcode*/
		binary_f.read((char *)&tmp_uint,sizeof(char));
		tmp_instr.opcode = opcode_type(tmp_uint);
		/*result*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.result->type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.result->value = tmp_uint;
		/*arg1*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.arg1->type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.arg1->value = tmp_uint;
		/*arg2*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.arg2->type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.arg2->value = tmp_uint;
		/*scrline*/
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.source_line = tmp_uint;
		this->instr_code_vector.push_back(tmp_instr);
	}

	binary_f.close();
	return;
}