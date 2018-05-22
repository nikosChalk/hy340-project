#ifndef HY340_PROJECT_TCODE_GENERATOR_H
#define HY340_PROJECT_TCODE_GENERATOR_H

#include <vector>
#include <string>
#include "../intermediate_code/expr.h"
#include "../intermediate_code/quad.h"
#include "../common_interface/vm_types.h"
#include "types.h"


using namespace std;
using namespace virtual_machine;

namespace target_code{

	class tcode_generator
	{
	public:
		/*initializes the t-code generator*/
		tcode_generator(vector<intermediate_code::quad*> quad_vector);

		/**
		* Appends the instruction to the end of the instruction_vector and changes its label field to match the
		* position of the instruction within the instruction_vector.
		* @param instruction The instruction to emit. Must not be NULL/nullptr
		*/
		void emit_instruction(VMinstruction instruction);

		/**
		* Returns the label of the next instruction which will be emitted
		* @return The label of the next instruction which will be emitted
		*/
		unsigned int next_instruction_label() const;

		/**
		* Returns the number of current quad
		* @return the number of current quad
		*/
		unsigned int curr_processed_quad() const;

		/*functions fill constants arrays of values-functions*/
		unsigned int consts_new_string(string s);
		unsigned int consts_new_number(double n);
		unsigned int libfuncs_new_used(string s);
		unsigned int userfuncs__new_func(syntax_analyzer::symbol_table::entry* sym);

		/*This function translate expressions* to vmargs*/
		void make_operand(intermediate_code::expr* e, VMarg* arg);
		
		/*Helper functions to produce common arguments for generated functions*/
		void make_retval_operand(VMarg* arg);

		/*generate functions*/
		void generate_PARAM(target_code::Tcode_quad* quad);
		void generate_CALL(target_code::Tcode_quad* quad);
		void generate_GETRETVAL(target_code::Tcode_quad* quad);
		void generate_FUNCSTART(target_code::Tcode_quad* quad);
		void generate_RETURN(target_code::Tcode_quad* quad);
		void generate_FUNCEND(target_code::Tcode_quad* quad);
		void generate_UMINUS(target_code::Tcode_quad* quad);
		void generate_NOP();

		/*basic generate function*/
		void generate(vector<target_code::Tcode_quad*> quad_vector);
		/*helper generate functions*/
		void generate(VMopcode opcode, target_code::Tcode_quad* quad);
		void generate_relational(VMopcode opcode, target_code::Tcode_quad* quad);
		
		/*this function fills incomplete jump instruction number in result field*/
		void patch_incomplete_jumps(void);
		/*this function adds an incomplete jump in incomplete jump vector*/
		void add_incomplete_jump(unsigned int instrNo, unsigned int iaddress);
		/*this function fills incomplete jump instruction number in result->value field*/
		void back_patch(vector<unsigned int> list, unsigned int t_value);

	private:
		vector <double> num_Consts;
		vector <string> string_Consts;
		vector <string> named_Lib_Funcs;
		vector <Userfunc*> user_Funcs;
		vector<VMinstruction> instruction_vector;
	    vector<Tcode_quad*> quad_vector;
		vector<Incomplete_Jump*> incomplete_jump_vector;
		unsigned int curr_proc_quad;
		stack<vector<unsigned int>> func_stack;
	};
}

#endif //HY340_PROJECT_TCODE_GENERATOR_H