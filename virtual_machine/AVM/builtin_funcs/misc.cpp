

#include <iostream>
#include <cstring>
#include "../AVM.h"
#include "../Memcell.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

void AVM::libfunc_print() {
    unsigned int total_actuals = program_stack.get_total_actuals();
    for(unsigned int i=0; i<total_actuals; i++)
        cout << program_stack.get_actual_arg(i)->to_string() << endl;
}

void AVM::libfunc_typeof() {
    unsigned int args = program_stack.get_total_actuals();

    if(args != 1)
        throw internal_error("Expected 1 argument in library function 'typeof' but got " + std::to_string(args));

    retval.clear(); //clear register's content
    retval.type = Memcell::Type::string;
    retval.value.str_ptr = strdup(Memcell::type_to_string(program_stack.get_actual_arg(0)->type).c_str());
}