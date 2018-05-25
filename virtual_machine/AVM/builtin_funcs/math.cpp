

#include <cmath>
#include "../AVM.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

/**
 * Converts the given number from degrees to rads
 */
#define to_rads(x) ( ((x) * M_PI) / 180.0)

void AVM::libfunc_sqrt(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'sqrt' but got " + std::to_string(args));

	Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::number)
		throw internal_error("Argument type of 'sqrt' must be a number");

	//Sqrt can be calculated
    retval.clear(); //clear register's content
    if(first_arg->value.num < 0) {
        retval.type = Memcell::Type::nil;
    } else {
        retval.type = Memcell::Type::number;
        retval.value.num = pow(first_arg->value.num, 0.5);
    }
}

void AVM::libfunc_cos(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'cos' but got " + std::to_string(args));

    Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::number)
		throw internal_error("Argument type of 'cos' must be a number");


    retval.clear(); //clear register's content
    retval.type = Memcell::Type::number;
    retval.value.num = cos(to_rads(first_arg->value.num));
}

void AVM::libfunc_sin(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'sin' but got " + std::to_string(args));

    Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::number)
		throw internal_error("Argument type of 'sin' must be a number");


    retval.clear(); //clear register's content
    retval.type = Memcell::Type::number;
    retval.value.num = sin(to_rads(first_arg->value.num));
}