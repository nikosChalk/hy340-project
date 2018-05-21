#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

#include "../common_interface/vm_types.h"

namespace virtual_machine {

    class rdp {
    public:
        /*rdp parser constructor*/
        rdp();

        /*constants arrays and instruction array*/
        std::vector<double> num_Consts;
        unsigned int total_Num_Consts;
        std::vector<std::string> string_Consts;
        unsigned int total_String_Consts;
        std::vector<std::string> named_Lib_Funcs;
        unsigned int total_Named_Lib_Funcs;
        std::vector<Userfunc> user_Funcs;
        unsigned int total_User_Funcs;
        std::vector<VMinstruction> instr_code_vector;
        unsigned int total_Instractions;

        /*helper function*/
        VMopcode opcode_type(unsigned int);

        VMarg::Type vmarg_type(unsigned int);

        /*basic parsing function*/
        void RDP_Parser();
    };
}


