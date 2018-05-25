#include <math.h>
#include "../AVM.h"
#include "../errors/internal_error.h"

#define PI 3.14159265

using namespace std;
using namespace virtual_machine;

void AVM::libfunc_sqrt(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'sqrt' but got " + std::to_string(args));

	if (program_stack.get_actual_arg(0)->type != Memcell::Type::number)
		throw internal_error("Argument type of 'sqrt' must be a number");
	else{
		if (program_stack.get_actual_arg(0)->value.num < 0){
			retval.clear(); //clear register's content
			retval.type = Memcell::Type::nil;
		}
		else{
			retval.clear(); //clear register's content
			retval.type = Memcell::Type::number;
			retval.value.num = pow(program_stack.get_actual_arg(0)->value.num, 1 / 2);
		}
	}
}

void AVM::libfunc_cos(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'cos' but got " + std::to_string(args));

	if (program_stack.get_actual_arg(0)->type != Memcell::Type::number)
		throw internal_error("Argument type of 'cos' must be a number");
	else{
		retval.clear(); //clear register's content
		retval.type = Memcell::Type::number;
		retval.value.num = cos(program_stack.get_actual_arg(0)->value.num * PI / 180.0);
	}
}

void AVM::libfunc_sin(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'sin' but got " + std::to_string(args));

	if (program_stack.get_actual_arg(0)->type != Memcell::Type::number)
		throw internal_error("Argument type of 'sin' must be a number");
	else{
		retval.clear(); //clear register's content
		retval.type = Memcell::Type::number;
		retval.value.num = sin(program_stack.get_actual_arg(0)->value.num * PI / 180.0);
	}
}