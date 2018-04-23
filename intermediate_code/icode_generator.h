

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
         * Emits the given quad
         * @param quad The quad to emit. Must not be NULL/nullptr
         */
        void emit_quad(quad *quad);

    private:
        std::vector<quad*> quad_vector;

        unsigned long total_quads() const;

        unsigned long next_quad_label() const;  /*TODO:???????? what is ths?*/

        void patch_label(unsigned long quadno, unsigned int label) const;
    };

};


#endif //HY340_PROJECT_ICODE_GENERATOR_H
