

#include <map>
#include <cassert>
#include "../AVM.h"
#include "../errors/alpha_runtime_error.h"

using namespace std;
using namespace virtual_machine;

typedef bool (*comparator_func_t)(long double leftOperand, long double rightOperand);

static bool comparator_jle(long double leftOperand, long double rightOperand) {
    return leftOperand<=rightOperand;
}
static bool comparator_jge(long double leftOperand, long double rightOperand) {
    return leftOperand>=rightOperand;
}
static bool comparator_jlt(long double leftOperand, long double rightOperand) {
    return leftOperand<rightOperand;
}
static bool comparator_jgt(long double leftOperand, long double rightOperand) {
    return leftOperand>rightOperand;
}

static std::map<unsigned int, comparator_func_t> operator_to_func_map = {
        {VMopcode::jle, comparator_jle}, {VMopcode::jge, comparator_jge},
        {VMopcode::jlt, comparator_jlt}, {VMopcode::jgt, comparator_jgt}
};

void AVM::execute_relational(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::jle || instr.opcode == VMopcode::jge ||
           instr.opcode == VMopcode::jlt || instr.opcode == VMopcode::jgt);
    assert(instr.result->type == VMarg::Type::label);

    Memcell *leftOperand = translate_operand(instr.arg1, &ax);
    Memcell *rightOperand = translate_operand(instr.arg2, &bx);

    assert(leftOperand && rightOperand);

    if(leftOperand->type != Memcell::Type::number || rightOperand->type != Memcell::Type::number)
        throw alpha_runtime_error("Not numeric operator in relational operation", instr.source_line);

    comparator_func_t comparator_func = operator_to_func_map.at(instr.opcode);
    if( (*comparator_func)(leftOperand->value.num, rightOperand->value.num) )   //if true, change pc. Else do nothing
        pc = instr.result->value;
}