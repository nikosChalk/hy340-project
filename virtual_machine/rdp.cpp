#include "rdp.h"

using namespace std;

/*Global variable for rdp*/
rdp parser;
/*RDP constructor*/
rdp::rdp()
{
	num_Consts = vector<double>();
	total_Num_Consts = 0;
	string_Consts = vector <string>();
	total_String_Consts = 0;
	named_Lib_Funcs = vector <string>();
	total_Named_Lib_Funcs = 0;
	user_Funcs = vector <struct user_func>();
	total_User_Funcs = 0;
	instr_code_vector = vector <instruction>();
	total_Instractions = 0;
}

rdp::vmopcode opcode_type(unsigned int type){
	switch (type)
	{
	case 0:
		return rdp::vmopcode::assign_v;
	case 1:
		return rdp::vmopcode::add_v;
	case 2:
		return rdp::vmopcode::sub_v;
	case 3:
		return rdp::vmopcode::mul_v;
	case 4:
		return rdp::vmopcode::div_v;
	case 5:
		return rdp::vmopcode::mod_v;
	case 6:
		return rdp::vmopcode::uminus_v;
	case 7:
		return rdp::vmopcode::and_v;
	case 8:
		return rdp::vmopcode::or_v;
	case 9:
		return rdp::vmopcode::not_v;
	case 10:
		return rdp::vmopcode::jeq_v;
	case 11:
		return rdp::vmopcode::jne_v;
	case 12:
		return rdp::vmopcode::jle_v;
	case 13:
		return rdp::vmopcode::jge_v;
	case 14:
		return rdp::vmopcode::jlt_v;
	case 15:
		return rdp::vmopcode::jgt_v;
	case 16:
		return rdp::vmopcode::call_v;
	case 17:
		return rdp::vmopcode::pusharg_v;
	case 18:
		return rdp::vmopcode::funcenter_v;
	case 19:
		return rdp::vmopcode::funcexit_v;
	case 20:
		return rdp::vmopcode::newtable_v;
	case 21:
		return rdp::vmopcode::tablegetelem_v;
	case 22:
		return rdp::vmopcode::tablesetelem_v;
	case 23:
		return rdp::vmopcode::nop_v;
	default:
		cerr << "Invalid opcode !!!\n";
		assert(0);
	}
	return (rdp::vmopcode)0;
}

rdp::vmarg_t vmarg_type(unsigned int type){
	switch (type)
	{
	case 0:
		return rdp::vmarg_t::label_a;
	case 1:
		return rdp::vmarg_t::global_a;
	case 2:
		return rdp::vmarg_t::formal_a;
	case 3:
		return rdp::vmarg_t::local_a;
	case 4:
		return rdp::vmarg_t::number_a;
	case 5:
		return rdp::vmarg_t::string_a;
	case 6:
		return rdp::vmarg_t::bool_a;
	case 7:
		return rdp::vmarg_t::nil_a;
	case 8:
		return rdp::vmarg_t::userfunc_a;
	case 9:
		return rdp::vmarg_t::libfunc_a;
	case 10:
		return rdp::vmarg_t::retval_a;
	default:
		cerr << "Invalid vm argument type !!!\n";
		assert(0);
	}
	return (rdp::vmarg_t)0;
}

void RDP_Parser(void){
	string tmp_string;
	double tmp_double;
	unsigned int tmp_uint;
	rdp::user_func tmp_user_func;
	rdp::instruction tmp_instr;

	ifstream binary_f;
	parser = rdp::rdp();

	binary_f.open("binary.abc", ios::in | ios::binary);
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));

	/*check magicnumber*/
	if (tmp_uint != 340200501){
		cerr << "Error !!! no alpha binary code file";
		assert(0);
	}

	/*load constants arrays (strings)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	parser.total_String_Consts = tmp_uint;
	for (unsigned int i = 0; i < parser.total_String_Consts; i++){
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_string, tmp_uint); /*a string with tmp_uint bytes*/
		parser.string_Consts.push_back(tmp_string); /*push string in vector*/
	}

	/*load constants arrays (numbers)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	parser.total_Num_Consts = tmp_uint;
	for (unsigned int i = 0; i < parser.total_Num_Consts; i++){
		binary_f.read((char *)&tmp_double, sizeof(double));
		parser.num_Consts.push_back(tmp_double); /*push number in vector*/
	}

	/*load constants arrays (userfunctions)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	parser.total_User_Funcs = tmp_uint;
	for (unsigned int i = 0; i < parser.total_User_Funcs; i++){
		binary_f.read((char *)&tmp_user_func.address, sizeof(unsigned int));
		binary_f.read((char *)&tmp_user_func.local_size, sizeof(unsigned int));
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_user_func.id, tmp_uint); /*a string with tmp_uint bytes*/
		parser.user_Funcs.push_back(tmp_user_func); /*push user_function in vector*/
	}

	/*load constants arrays (libraryfunctions)*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	parser.total_Named_Lib_Funcs = tmp_uint;
	for (unsigned int i = 0; i < parser.total_Named_Lib_Funcs; i++){
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		binary_f.read((char *)&tmp_string, tmp_uint); /*a string with tmp_uint bytes*/
		parser.named_Lib_Funcs.push_back(tmp_string); /*push string in vector*/
	}

	/*load code instructions*/
	binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
	parser.total_Instractions = tmp_uint;
	for (unsigned int i = 0; i < parser.total_Instractions; i++){
		/*opcode*/
		binary_f.read((char *)&tmp_uint,sizeof(char));
		tmp_instr.opcode = opcode_type(tmp_uint);
		/*result*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.result.type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.result.val = tmp_uint;
		/*arg1*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.arg1.type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.arg1.val = tmp_uint;
		/*arg2*/
		binary_f.read((char *)&tmp_uint, sizeof(char));
		tmp_instr.arg2.type = vmarg_type(tmp_uint);
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.arg2.val = tmp_uint;
		/*scrline*/
		binary_f.read((char *)&tmp_uint, sizeof(unsigned int));
		tmp_instr.srcLine = tmp_uint;
		parser.instr_code_vector.push_back(tmp_instr);
	}

	binary_f.close();
	return;
}