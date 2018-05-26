

#include "../AVM.h"
#include "../Table.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

void AVM::libfunc_objectmemberkeys(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'libfunc_objectmemberkeys' but got " + std::to_string(args));

	Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::table)
		throw internal_error("Argument type of 'libfunc_objectmemberkeys' must be table");

	retval.clear(); //clear register's content
	retval.type = Memcell::Type::table;
	retval.value.table_ptr = first_arg->value.table_ptr->get_all_keys();
}

void AVM::libfunc_objecttotalmembers(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'libfunc_objecttotalmembers' but got " + std::to_string(args));

	Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::table)
		throw internal_error("Argument type of 'libfunc_objecttotalmembers' must be table");

	retval.clear(); //clear register's content
	retval.type = Memcell::Type::number;
	retval.value.num = first_arg->value.table_ptr->get_nr_keys();
}

void AVM::libfunc_objectcopy(void) {
	unsigned int args = program_stack.get_total_actuals();

	if (args != 1)
		throw internal_error("Expected 1 argument in library function 'libfunc_objectcopy' but got " + std::to_string(args));

	Memcell const *first_arg = program_stack.get_actual_arg(0);
	if (first_arg->type != Memcell::Type::table)
		throw internal_error("Argument type of 'libfunc_objectcopy' must be table");

	Table* table_copy = new Table(first_arg->value.table_ptr);

	retval.clear(); //clear register's content
	retval.type = Memcell::Type::table;
	retval.value.table_ptr = table_copy;
}