

#include <iostream>
#include <cstring>
#include <sstream>
#include "../AVM.h"
#include "../Memcell.h"
#include "../errors/internal_error.h"

using namespace std;
using namespace virtual_machine;

void AVM::libfunc_input(void) {
    string input_str;

    getline(cin, input_str);    //read line from standard input
    retval.clear(); //clear register's content

    if(input_str == "nil") {    //Check if input is "nul"
        retval.type = Memcell::Type::nil;
    } else if(input_str == "true" || input_str == "false") {    //check if input is of type boolean
        retval.type = Memcell::Type::boolean;
        retval.value.boolean = (input_str == "true");
    } else {
        try {   //Try to convert the input to long double
            retval.type = Memcell::Type::number;
            retval.value.num = stold(input_str);
        } catch(exception const &ex) {  //All of the above failed. Return the input as string
            retval.type = Memcell::Type::string;
            retval.value.str_ptr = strdup(input_str.c_str());
        }
    }
}

void AVM::libfunc_print(void) {
    retval.clear();
    retval.type = Memcell::Type::nil;

    unsigned int total_actuals = program_stack.get_total_actuals();
    for(int i=0; i<total_actuals; i++) {
        Memcell const *cur_arg = program_stack.get_actual_arg(i);
        cout << cur_arg->to_string(const_pool);

        if(i != total_actuals-1)
            cout << " ";
    }
    cout << endl;

}

void AVM::libfunc_strtonum(void) {
    unsigned int args = program_stack.get_total_actuals();

    if(args!= 1)
        throw internal_error("Expected 1 argument in library function 'strtonum' but got " + std::to_string(args));

    Memcell const *first_arg = program_stack.get_actual_arg(0);
    if(first_arg->type != Memcell::Type::string)
        throw internal_error("Argument type of 'strtonum' must be of type string");

    retval.clear();
    try {
        retval.type = Memcell::Type::number;
        retval.value.num = std::stold(string(first_arg->value.str_ptr));
    } catch(exception const &ex) {
        //conversion was not possible. Return nil
        retval.type = Memcell::Type::nil;
    }
}

void AVM::libfunc_typeof(void) {
    unsigned int args = program_stack.get_total_actuals();

    if(args != 1)
        throw internal_error("Expected 1 argument in library function 'typeof' but got " + std::to_string(args));

    retval.clear(); //clear register's content
    retval.type = Memcell::Type::string;
    retval.value.str_ptr = strdup(Memcell::type_to_string(program_stack.get_actual_arg(0)->type).c_str());
}