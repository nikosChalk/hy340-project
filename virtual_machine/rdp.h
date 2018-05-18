#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>

class rdp
{
public:
	/*rdp parser constructor*/
	rdp();

	enum vmopcode{
		assign_v,		add_v,			sub_v,
		mul_v,			div_v,			mod_v,
		uminus_v,		and_v,			or_v,
		not_v,			jeq_v,			jne_v,
		jle_v,			jge_v,			jlt_v,
		jgt_v,			call_v,			pusharg_v,
		funcenter_v,	funcexit_v,		newtable_v,
		tablegetelem_v, tablesetelem_v, nop_v
	};

	enum vmarg_t{
		label_a    = 0,
		global_a   = 1,
		formal_a   = 2,
		local_a    = 3,
		number_a   = 4,
		string_a   = 5,
		bool_a     = 6,
		nil_a      = 7,
		userfunc_a = 8,
		libfunc_a  = 9,
		retval_a   = 10
	};

	struct vmarg{
		vmarg_t type;
		unsigned int val;
	};

	struct instruction{
		vmopcode     opcode;
		vmarg	     result;
		vmarg		 arg1;
		vmarg		 arg2;
		unsigned int srcLine;
	};

	struct user_func {
		unsigned int address;
		unsigned int local_size;
		std::string id;
	};

	/*constants arrays and instruction array*/
	std::vector <double> num_Consts;
	unsigned int total_Num_Consts;
	std::vector <std::string> string_Consts;
	unsigned int total_String_Consts;
	std::vector <std::string> named_Lib_Funcs;
	unsigned int total_Named_Lib_Funcs;
	std::vector <user_func> user_Funcs;
	unsigned int total_User_Funcs;
	std::vector <instruction> instr_code_vector;
	unsigned int total_Instractions;

	/*helper function*/
	vmopcode opcode_type(unsigned int);
	vmarg_t  vmarg_type(unsigned int);

	/*basic parsing function*/
	void RDP_Parser(void);
};


