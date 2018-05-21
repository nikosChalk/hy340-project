

#ifndef HY340_PROJECT_ALPHA_RUNTIME_ERROR_H
#define HY340_PROJECT_ALPHA_RUNTIME_ERROR_H

#include <stdexcept>

namespace virtual_machine {
    class alpha_runtime_error : public std::runtime_error {
    public:

        /**
         * Throws a runtime error caused by some instruction within the virtual machine
         * @param info A descriptive message of the error
         * @param source_line The source line that triggered the error
         */
        alpha_runtime_error(std::string &info, unsigned int source_line);

        /**
         * Throws a runtime error caused by some instruction within the virtual machine
         * @param info A descriptive message of the error
         * @param source_line The source line that triggered the error
         */
        alpha_runtime_error(const char *info, unsigned int source_line);

        virtual char const *what() const noexcept;
    };
}


#endif //HY340_PROJECT_ALPHA_RUNTIME_ERROR_H
