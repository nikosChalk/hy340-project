

#ifndef HY340_PROJECT_ICODE_GENERATOR_H
#define HY340_PROJECT_ICODE_GENERATOR_H

#include <vector>
#include "quad.h"
#include "../syntax_analyzer/hidden_var_handler.h"

namespace intermediate_code {

    class icode_generator {
    public:

        /**
         * Initializes the i-code generator
         */
        icode_generator();

        /**
         * Returns the quad vector that the icode_generator has currently emitted
         */
        const std::vector<quad*> get_quad_vector() const;

        /**
         * Appends the quad to the end of the quad_vector and changes its label field to match the
         * position of the quad within the quad_vector.
         * @param quad The quad to emit. Must not be NULL/nullptr
         */
        void emit_quad(quad *quad);

        /**
         * Appends the quad to the end of the quad_vector and changes its label field to the given label.
         * @param quad The quad to emit. Must not be NULL/nullptr
         * @param label The label that the quad should have
         */
        void emit_quad(quad *quad, unsigned int label);

        /**
         * Changes the quad_vector[quadno]->label field to the given label
         * @param quadno The quad's number to patch. Must be less than next_quad_label().
         * @param label The new label to set.
         */
        void patch_label(unsigned int quadno, unsigned int label);

        /**
         * For each entry quadno in the given quadno_vector, it changes the quad_vector[quadno]->label field to the given label
         * @param quadno_vector The vector containing the quads' number to patch. Each entry must be less than next_quad_label().
         * @param label The new label to set.
         */
        void patch_label(const std::vector<unsigned int> &quadno_vector, unsigned int label);

        /**
         * Returns the label of the next quad which will be emitted
         * @return The label of the next quad which will be emitted
         */
        unsigned int next_quad_label() const;

        /**
         * Returns a string representation of this object
         * @return The string representation
         */
        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream &os, const icode_generator &igen);

    private:
        std::vector<quad*> quad_vector;
    };

};


#endif //HY340_PROJECT_ICODE_GENERATOR_H
