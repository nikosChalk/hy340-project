#ifndef HY340_PROJECT_TCODE_GENERATOR_H
#define HY340_PROJECT_TCODE_GENERATOR_H

#include <vector>
#include <string>
#include "../intermediate_code/expr.h"
#include "../intermediate_code/quad.h"

using namespace std;

namespace target_code{

	class tcode_generator
	{
	public:
		/*initializes the t-code generator*/
		tcode_generator();

		/**
		* Appends the instruction to the end of the instruction_vector and changes its label field to match the
		* position of the instruction within the instruction_vector.
		* @param instruction The instruction to emit. Must not be NULL/nullptr
		*/
		void emit_instruction(target_instruction::instruction *instruction);

		/**
		* Returns the label of the next instruction which will be emitted
		* @return The label of the next instruction which will be emitted
		*/
		unsigned int next_instruction_label() const;

		/*constants arrays of values-functions*/
		unsigned int consts_new_string(string s);
		unsigned int consts_new_number(double n);
		unsigned int libfuncs_new_used(string s);
		unsigned int userfuncs__new_func(syntax_analyzer::symbol_table::entry* sym);
		vector <double> num_Consts;
		unsigned int total_Num_Consts;
		vector <string> string_Consts;
		unsigned int total_String_Consts;
		vector <string> named_Lib_Funcs;
		unsigned int total_Named_Lib_Funcs;
		vector <target_code::target_instruction::user_func> user_Funcs;
		unsigned int total_User_Funcs;

		/*This function translate expressions* to vmargs*/
		void make_operand(intermediate_code::expr* e, target_code::target_instruction::vmarg* arg);

		/*Helper functions to produce common arguments for generated functions*/
		void make_number_operand(target_code::target_instruction::vmarg* arg, double val);
		void make_bool_operand(target_code::target_instruction::vmarg* arg, unsigned int val);
		void make_retval_operand(target_code::target_instruction::vmarg* arg);

		/*generate functions*/
		void generate_ADD(intermediate_code::quad*);
		void generate_SUB(intermediate_code::quad*);
		void generate_MUL(intermediate_code::quad*);
		void generate_DIV(intermediate_code::quad*);
		void generate_MOD(intermediate_code::quad*);
		void generate_NEWTABLE(intermediate_code::quad*);
		void generate_TABLEGETELEM(intermediate_code::quad*);
		void generate_TABLESETELEM(intermediate_code::quad*);
		void generate_ASSIGN(intermediate_code::quad*);
		void generate_NOP(intermediate_code::quad*);
		void generate_JUMP(intermediate_code::quad*);
		void generate_IF_EQ(intermediate_code::quad*);
		void generate_IF_NOTEQ(intermediate_code::quad*);
		void generate_IF_GREATER(intermediate_code::quad*);
		void generate_IF_GREATEREQ(intermediate_code::quad*);
		void generate_IF_LESS(intermediate_code::quad*);
		void generate_IF_LESSEQ(intermediate_code::quad*);
		void generate_NOT(intermediate_code::quad*);
		void generate_OR(intermediate_code::quad*);
		void generate_PARAM(intermediate_code::quad*);
		void generate_CALL(intermediate_code::quad*);
		void generate_GETRETVAL(intermediate_code::quad*);
		void generate_FUNCSTART(intermediate_code::quad*);
		void generate_RETURN(intermediate_code::quad*);
		void generate_FUNCEND(intermediate_code::quad*);

		/*basic generate function*/
		void generate(void);
		/*helper generate functions*/
		void generate_arithmetic(target_code::target_instruction::vmopcode opcode, intermediate_code::quad* quad);
		void generate_relational(target_code::target_instruction::vmopcode opcode, intermediate_code::quad* quad);

	private:
		vector<target_instruction::instruction*> instruction_vector;
	};
}

#endif //HY340_PROJECT_TCODE_GENERATOR_H