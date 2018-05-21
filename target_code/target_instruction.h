#ifndef HY340_PROJECT_INSTRUCTION_H
#define HY340_PROJECT_INSTRUCTION_H

namespace target_code{

	class target_instruction{
	public:
		enum vmarg_t{
			label_a = 0,
			global_a = 1,
			formal_a = 2,
			local_a = 3,
			number_a = 4,
			string_a = 5,
			bool_a = 6,
			nil_a = 7,
			userfunc_a = 8,
			libfunc_a = 9,
			retval_a = 10
		};

		enum vmopcode{
			assign_v, add_v, sub_v,
			mul_v, div_v, mod_v,
			uminus_v, and_v, or_v,
			not_v, jeq_v, jne_v,
			jle_v, jge_v, jlt_v,
			jgt_v, call_v, pusharg_v,
			funcenter_v, funcexit_v, newtable_v,
			tablegetelem_v, tablesetelem_v, nop_v
		};

		struct vmarg{
			vmarg_t type;
			unsigned int val;
		};
		/**
		* Representation of an target code instruction
		*/
		struct instruction {
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
	};
}

#endif //HY340_PROJECT_INSTRUCTION_H