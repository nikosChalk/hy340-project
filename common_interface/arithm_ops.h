
/**
 * Arithmetic operations implemented in the same way both in the AVM and the acc
 */

#ifndef HY340_PROJECT_ARITHM_OPS_H
#define HY340_PROJECT_ARITHM_OPS_H

#endif //HY340_PROJECT_ARITHM_OPS_H

namespace arithmetic_operations {

    typedef long double (*arithmetic_func_t)(long double leftOperand, long double rightOperand);

    long double add(long double leftOperand, long double rightOperand);

    long double sub(long double leftOperand, long double rightOperand);

    long double mul(long double leftOperand, long double rightOperand);

    /**
     * @throws numeric_error If rightOperand is 0
     */
    long double div(long double leftOperand, long double rightOperand);

    /**
     * @throws numeric_error If rightOperand is 0
     */
    long double mod(long double leftOperand, long double rightOperand);
}