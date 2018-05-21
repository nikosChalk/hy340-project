

#ifndef HY340_PROJECT_NUMERIC_ERROR_H
#define HY340_PROJECT_NUMERIC_ERROR_H

#include <stdexcept>

namespace arithmetic_operations {
    class numeric_error : public std::runtime_error {
    public:

        /**
         * Throws a runtime error caused considering an unsupported numeric operation
         * @param info A descriptive message of the error
         */
        explicit numeric_error(std::string &info);

        /**
         * Throws a runtime error caused considering an unsupported numeric operation
         * @param info A descriptive message of the error
         */
        explicit numeric_error(const char *info);

        virtual char const *what() const noexcept;
    };
};


#endif //HY340_PROJECT_NUMERIC_ERROR_H
