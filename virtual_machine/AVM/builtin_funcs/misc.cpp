

#include <iostream>
#include "../AVM.h"

using namespace std;
using namespace virtual_machine;

void AVM::libfunc_print() {
    unsigned int total_actuals = program_stack.get_total_actuals();
    for(unsigned int i=0; i<total_actuals; i++)
        cout << program_stack.get_actual_arg(i)->to_string() << endl;
}