

#include <cassert>
#include <map>
#include "../AVM.h"
#include "../errors/alpha_runtime_error.h"
#include "../../../common_interface/arithm_ops.h"
#include "../Memcell.h"

using namespace std;
using namespace virtual_machine;
using namespace arithmetic_operations;

static std::map<unsigned int, arithmetic_func_t> operator_to_func_map = {
        {VMopcode::add, arithmetic_operations::add}, {VMopcode::sub, arithmetic_operations::sub},
        {VMopcode::mul, arithmetic_operations::mul}, {VMopcode::div, arithmetic_operations::div},
        {VMopcode::mod, arithmetic_operations::mod}
};

void AVM::execute_arithmetic(const VMinstruction &instr) {
    assert(instr.opcode == VMopcode::add || instr.opcode == VMopcode::sub || instr.opcode == VMopcode::mul ||
           instr.opcode == VMopcode::div || instr.opcode == VMopcode::mod);

    Memcell *lv = translate_operand(instr.result);
    Memcell *leftOperand = translate_operand(instr.arg1, &ax);
    Memcell *rightOperand = translate_operand(instr.arg2, &bx);

    assert(lv && (program_stack.is_in_stack(lv) || lv == &retval));
    assert(leftOperand && rightOperand);

    if(leftOperand->type != Memcell::Type::number || rightOperand->type != Memcell::Type::number)
        throw alpha_runtime_error("Not numeric operator in arithmetic operation", instr.source_line);

    arithmetic_func_t operation_func = operator_to_func_map.at(instr.opcode);
    lv->clear();
    lv->type = Memcell::Type::number;
    lv->value.num = (*operation_func)(leftOperand->value.num, rightOperand->value.num);
}