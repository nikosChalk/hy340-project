

#ifndef HY340_PROJECT_SEMANTIC_ERROR_H
#define HY340_PROJECT_SEMANTIC_ERROR_H

#include <stdexcept>

namespace intermediate_code {

    class semantic_error : public std::runtime_error {
    public:
        explicit semantic_error(unsigned int lineno);
        semantic_error(const std::string &msg, unsigned int lineno);
        virtual char const* what() const noexcept;
    };
};


#endif //HY340_PROJECT_SEMANTIC_ERROR_H
