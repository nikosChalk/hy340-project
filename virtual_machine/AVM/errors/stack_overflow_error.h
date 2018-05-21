

#ifndef HY340_PROJECT_STACK_OVERFLOW_ERROR_H
#define HY340_PROJECT_STACK_OVERFLOW_ERROR_H

#include <stdexcept>

namespace virtual_machine {
    class stack_overflow_error : public std::runtime_error {
    public:

        /**
         * Throws a runtime error caused by some instruction that tried to allocate memory in the Program_stack, but
         * and overflow occured
         * @param info A descriptive message of the error
         */
        stack_overflow_error(std::string &info);

        /**
         * Throws a runtime error caused by some instruction that tried to allocate memory in the Program_stack, but
         * and overflow occured
         * @param info A descriptive message of the error
         */
        stack_overflow_error(const char *info);

        virtual char const *what() const noexcept;
    };
}

#endif //HY340_PROJECT_STACK_OVERFLOW_ERROR_H
