//
// Created by nikos on 3/20/18.
//

#ifndef HY340_PROJECT_NOTIMPLEMENTEDERROR_H
#define HY340_PROJECT_NOTIMPLEMENTEDERROR_H


#include <stdexcept>

class not_implemented_error : public std::logic_error {
public:
    not_implemented_error();
    explicit not_implemented_error(std::string &msg);
    explicit not_implemented_error(const char *msg);
    virtual char const* what() const noexcept;
};


#endif //HY340_PROJECT_NOTIMPLEMENTEDERROR_H
