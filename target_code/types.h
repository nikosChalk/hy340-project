

#include "../intermediate_code/quad.h"

#ifndef HY340_PROJECT_TARGET_CODE_TYPES_H
#define HY340_PROJECT_TARGET_CODE_TYPES_H

namespace target_code {

    /**
     * Helper struct for translating branch/jump intermediate code target labels, to labels of the virtual machine code
     */
    class Incomplete_jump {
	public:
		/**
         * Initializes this helper struct with the given attributes
         * @param quad_target_label Label of a quad. This label corresponds to the target label of a branch/jump quad,
         * i.e. the label to which the instruction jumps.
         *
         * @param vm_instr_source_label Label of virtual machine code. This label corresponds to the branch/jump VM instruction
         * that was generated from a branch/jump intermediate code quad with target quad label "quad_target_label"
         */
		Incomplete_jump(unsigned int quad_target_label, unsigned int vm_instr_source_label);

        /**
         * Returns the target jump address of the intermediate code quad
         */
        unsigned int get_quad_target_label() const;

        /**
         * Returns the address of the VM source "jump" instruction
         */
        unsigned int get_vm_instr_source_label() const;

	private:
        unsigned int quad_target_label;
        unsigned int vm_instr_source_label;
    };

    /**
     * Helper struct for bookkeeping an 1-1 relation of quad label <---> VM instruction label
     */
	class Address_link {
    public:

	    /**
	     * Default constructor (Junk)
	     */
	    Address_link() = default;

	    /**
	     * Initializes this helper struct with the given attributes
	     * @param quad_label An intermediate code quad label
	     * @param vm_instr_label The label of the VM instruction to which the given quad_label is translated to.
	     * If the quad is translated to multiple VM instructions, then the first VM instruction's label should be given
	     */
        Address_link(unsigned int quad_label, unsigned int vm_instr_label);

        /**
         * Returns the address of the quad, i.e. the quad label associated with this link
         */
        unsigned int get_quad_label() const;

        /**
         * Returns the address of the VM instructions, i.e. the VM instruction's label that is associated with this link
         */
        unsigned int get_vm_instr_label() const;

    private:
	    unsigned int quad_label;
	    unsigned int vm_instr_label;
	};
}

#endif //HY340_PROJECT_TARGET_CODE_TYPES_H
