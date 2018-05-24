

#include <string>
#include <iostream>
#include "RDP/RDP.h"
#include "AVM/AVM.h"
#include "AVM/errors/alpha_runtime_error.h"

using namespace std;
using namespace virtual_machine;

int main (int argc, char *argv[]) {

    /* Check command line arguments and open I/O files */
    if(argc < 2) {
        cerr << "Too few arguments. Expected input alpha binary file file." << endl;
        exit(EXIT_FAILURE);
    }

    //Initialize RDP and parse binary file
    RDP rdp = RDP();
    try {
        rdp.parse(string(argv[1]));
    } catch(ifstream::failure const &err) {
        cerr << err.what() << endl;
        cerr << "Initialization of virtual machine failed." << endl;
        exit(EXIT_FAILURE);
    }

    //Initialize AVM
    AVM avm = AVM(
            rdp.get_instructions(), rdp.get_numbers(), rdp.get_strings(),
            rdp.get_libfuncs(), rdp.get_userfuncs(), rdp.get_nr_total_program_vars()
    );

    //Execute binary file
    try {
        avm.run();
    } catch(alpha_runtime_error const &err) {
        cerr << err.what() << endl;
        cerr << "Execution stopped" << endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}