

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
         * Stores the given quad and changes its label field to match the position of the quad within the
         * quad_vector.
         * @param quad The quad to emit. Must not be NULL/nullptr
         */
        void emit_quad(quad *quad);

    private:
        std::vector<quad*> quad_vector;

        void patch_label(unsigned long quadno, unsigned int label) const;
    };

};


#endif //HY340_PROJECT_ICODE_GENERATOR_H
