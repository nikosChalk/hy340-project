

#include "arithm_ops.h"
#include "errors/numeric_error.h"

long double arithmetic_operations::add(long double leftOperand, long double rightOperand) {
    return leftOperand+rightOperand;
}

long double arithmetic_operations::sub(long double leftOperand, long double rightOperand) {
    return leftOperand-rightOperand;
}

long double arithmetic_operations::mul(long double leftOperand, long double rightOperand) {
    return leftOperand*rightOperand;
}

long double arithmetic_operations::div(long double leftOperand, long double rightOperand) {
    if(rightOperand == 0)
        throw numeric_error("Division by 0");

    return leftOperand/rightOperand;
}

long double arithmetic_operations::mod(long double leftOperand, long double rightOperand) {
    long long int rh = (long long int)rightOperand;
    if(rh == 0)
        throw numeric_error("Modulo by 0");

    return (long long int)leftOperand % rh;
}