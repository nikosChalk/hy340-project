

#ifndef HY340_PROJECT_ICODE_GENERATOR_H
#define HY340_PROJECT_ICODE_GENERATOR_H

#include <vector>
#include "types.h"

namespace intermediate_code {

    class icode_generator {
    public:

        /**
         * Initializes the i-code generator
         */
        icode_generator();

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
        void patch_label(unsigned long quadno, unsigned int label);

        /**
         * Returns the label of the next quad which will be emitted
         * @return The label of the next quad which will be emitted
         */
        unsigned int next_quad_label() const;

    private:
        std::vector<quad*> quad_vector;
    };

};


#endif //HY340_PROJECT_ICODE_GENERATOR_H
