//
// Created by nikos on 3/24/18.
//

#ifndef HY340_PROJECT_SYNTAX_ERROR_H
#define HY340_PROJECT_SYNTAX_ERROR_H


#include <stdexcept>

namespace syntax_analyzer {

    class syntax_error : public std::runtime_error {
    public:
        explicit syntax_error(unsigned int lineno);
        syntax_error(const std::string &msg, unsigned int lineno);
        virtual char const* what() const noexcept;
    };
};


#endif //HY340_PROJECT_SYNTAX_ERROR_H
