

#ifndef HY340_PROJECT_TCODE_GENERATOR_H
#define HY340_PROJECT_TCODE_GENERATOR_H

#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include "types.h"
#include "../intermediate_code/quad.h"
#include "../common_interface/types.h"
#include "../common_interface/Constants_pool.h"

namespace target_code {

	class VMcode_generator {
	public:
        //Declarations of pointer-to-member function types
        using Generate_membfunc_ptr = void (VMcode_generator::*) (intermediate_code::quad const*);
        using Generate_func_map = std::map<unsigned int, Generate_membfunc_ptr >;   //maps all quad::iopcode enumerated values to "generate_" member functions

		/**
		 * Initializes the target code generator and initiates the target code generation process
		 * @param quad_vector The vector containing all the quads
		 */
		explicit VMcode_generator(std::vector<intermediate_code::quad*> const &quad_vector);

		/**
		 * Generates the binary file which is in the same path as the source file. The binary file has the same name as
		 * the source path but with the extension ".abc"
		 * @param file_path The source file path
		 *
		 * @throws ofstream::failure If any error occured during the binary file generation
		 */
		void generate_binary_file(const std::string &file_path);
		/**
		 * Same as generate_binary_file() but with a default "a.abc" binary file name existing in the same directory
		 * where the compiler was invoked
		 */
        void generate_binary_file();

	private:
	    virtual_machine::Constants_pool const_pool; //handles the storage and registration of constants

		std::vector<virtual_machine::VMinstruction> vm_instructions;

		//Mapping of quadno --> VM instruction address. A quadno is mapped to the first VMinstruction that it generates
        std::map<unsigned int, Address_link> address_links; //key == quad address == value.get_quad_label()

		std::vector<Incomplete_jump> incomplete_jumps;
        unsigned int curr_proc_quad;    //The address of the quad that is being currently translated

        //Stack of vectors. Each time we encounter a "funcstart" we push an empty vector and each time we encounter a "funcend"
        //we pop from the stack. The top vector contains VM instruction addresses, that are incomplete jump instructions and
        //they must be patched to jump to the "funcend" VM instruction
        std::stack<std::vector<unsigned int>> func_stack;

        std::ofstream binary_ofs;

        /**
         * Appends the instruction to the end of the vm_instructions vector
         * @param instruction The instruction to emit.
        */
        void emit(virtual_machine::VMinstruction const &instruction);

        /**
        * Returns the label of the next instruction which will be emitted
        */
        unsigned int next_vm_instr_address() const;

		/**
		 * Translates expr* intermediate code type to VMarg, which is a VM type
		 * @param expr The expression to translate
		 * @return An allocated memory with "new" operator, whose contents will is the translated expr.
		 * Note that if expr was NULL/nullptr, then the returned value is nullptr
		 */
		virtual_machine::VMarg* translate_operand(intermediate_code::expr const *expr);

        /**
         * Sets the arg's content to represent the given numeric value as a VMarg
         * Note that an entry is also added to the const_pool
         * @param value The value to translate
		 * @return An allocated memory with "new" operator, whose contents will is the translated expr.
         */
        virtual_machine::VMarg* translate_number(double long value);

        /**
         * Sets the VMarg's content to represent the return value of a function.
         * @return An allocated memory with "new" operator, whose contents will act as the return value of
         * a function when used within a VMinstruction
         */
        static virtual_machine::VMarg* translate_retval();

        /* Generate Functions */
        void generate_ASSIGN(intermediate_code::quad const *quad);

        void generate_ADD(intermediate_code::quad const *quad);
        void generate_SUB(intermediate_code::quad const *quad);
        void generate_MUL(intermediate_code::quad const *quad);
        void generate_DIV(intermediate_code::quad const *quad);
        void generate_MOD(intermediate_code::quad const *quad);

        void generate_UMINUS(intermediate_code::quad const *quad);

        void generate_IF_EQ(intermediate_code::quad const *quad);
        void generate_IF_NOTEQ(intermediate_code::quad const *quad);
        void generate_IF_LESSEQ(intermediate_code::quad const *quad);
        void generate_IF_GREATEREQ(intermediate_code::quad const *quad);
        void generate_IF_LESS(intermediate_code::quad const *quad);
        void generate_IF_GREATER(intermediate_code::quad const *quad);

        void generate_CALL(intermediate_code::quad const *quad);
        void generate_PARAM(intermediate_code::quad const *quad);
        void generate_RET(intermediate_code::quad const *quad);
        void generate_GETRETVAL(intermediate_code::quad const *quad);
        void generate_FUNCSTART(intermediate_code::quad const *quad);
        void generate_FUNCEND(intermediate_code::quad const *quad);

        void generate_TABLECREATE(intermediate_code::quad const *quad);
        void generate_TABLEGETELEM(intermediate_code::quad const *quad);
        void generate_TABLESETELEM(intermediate_code::quad const *quad);

        void generate_JUMP(intermediate_code::quad const *quad);
        void generate_NOP(unsigned int souce_line); //@param source_line The source line that triggered the generate_NOP()

        /* Helper functions that generates a VM instruction with the given opcode that corresponds to the given quad.   */
        /* The VM instruction is emitted through emit() and the parameters are:                                         */
        /* @param vmopcode The VM opcode which the generated instruction will have                                      */
        /* @param quad The quad from which the VM instruction is generated                                              */

        /**
         * Helper function that generates a generic VM instruction with the given opcode that corresponds to the given quad with
         * simple translation of all operands. The VM instruction is emitted through emit()
         */
        void generate(virtual_machine::VMopcode vmopcode, intermediate_code::quad const *quad);

        /**
         * Helper function that generates a VM instruction for quad::iopcode::if_eq, if_noteq, if_greatereq, if_lesseq,
         * if_greater, if_less, jump
         * The arg1, arg2 are simply translated and the result is handled manually. If the VMinstruction where the generated VMinstruction
         * "branch" jumps to hasn't been emitted yet, then an entry is inserted in incomplete_jumps vector
         */
		void generate_relational(virtual_machine::VMopcode vmopcode, intermediate_code::quad const *quad);

        static const Generate_func_map generate_func_map;

        /* Helper functions for writing to the binary file */

        /**
         * Internal function that writes the binary file. This function does not handle internal exceptions thus in case
         * of an exception, the binary_ofs must be closed by the caller
         */
        void write_binary_file(const std::string &file_path);

        /**
         * Writes the given string to the binary_ofs, i.e. performs "string" grammar rule
         */
        void write_onestring(const std::string &string);

        /**
         * Writes the given totals as UNSIGNED INT to the binary_ofs, i.e. performs the "totals" grammar rule
         * @param totals The totals to write, which is CONVERTED to unsigned int
         */
        void write_totals(unsigned long totals);

        void write_vmarg(virtual_machine::VMarg const *vmarg);
	};
}

#endif //HY340_PROJECT_TCODE_GENERATOR_H