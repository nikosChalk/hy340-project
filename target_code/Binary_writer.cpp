

#include <map>
#include "Binary_writer.h"
#include "../common_interface/Constants.h"

using namespace std;
using namespace target_code;
using namespace virtual_machine;

Binary_writer::Binary_writer(vector<VMinstruction> const &vm_instructions,
                             const Constants_pool &const_pool,
                             unsigned int total_program_vars)
{
    this->total_program_vars = total_program_vars;
    this->vm_instructions = vector<VMinstruction>(vm_instructions);
    this->const_pool = const_pool;

    binary_ofs = ofstream();
    binary_ofs.exceptions(ofstream::badbit | ofstream::failbit);
}

void Binary_writer::write_binary_file(const std::string &file_path) {
    const string binary_extension = ".abc";
    binary_ofs.open((file_path+binary_extension), ofstream::out | ofstream::binary);

    /*Magic Number*/
    write_binary_uint(Constants::MAGICNUMBER);

    /*write constants arrays (strings)*/
    map<unsigned int, string> strMap = const_pool.get_string_array();
    write_totals(strMap.size());
    for(unsigned int i=0; i<strMap.size(); i++)
        write_onestring(strMap.at(i));

    /*write constants arrays (numbers)*/
    map<unsigned int, double long> numbersMap = const_pool.get_numbers_array();
    write_totals(numbersMap.size());
    for(unsigned int i=0; i<numbersMap.size(); i++)
        write_binary_dl(numbersMap.at(i));

    /*write constants arrays (userfunctions)*/
    std::map<unsigned int, Userfunc> userfuncMap = const_pool.get_userfunc_array();
    write_totals(userfuncMap.size());
    for(unsigned int i=0; i<userfuncMap.size(); i++){
        write_binary_uint(userfuncMap.at(i).address);
        write_binary_uint(userfuncMap.at(i).nr_locals);
        write_onestring(userfuncMap.at(i).name);
    }

    /*write constants arrays (libraryfunctions)*/
    std::map<unsigned int, std::string> libfuncsMap = const_pool.get_libfunc_array();
    write_totals(libfuncsMap.size());
    for(unsigned int i=0; i<libfuncsMap.size(); i++)
        write_onestring(libfuncsMap.at(i));

    /*write code instructions*/
    write_totals(vm_instructions.size());
    for(unsigned int i=0; i<vm_instructions.size(); i++) {
        const VMinstruction &cur_instr = vm_instructions.at(i);

        write_binary_ushort((unsigned short) cur_instr.opcode);

        write_vmarg(cur_instr.result);
        write_vmarg(cur_instr.arg1);
        write_vmarg(cur_instr.arg2);

        /*scrline*/
        write_binary_uint(cur_instr.source_line);
    }

    /* total program scope space variables */
    write_binary_uint(this->total_program_vars);

    binary_ofs.flush();
    binary_ofs.close();
    return;
}

void Binary_writer::generate_binary_file(const std::string &file_path) { //Wrapper of write_binary_file in order to close the file properly
    try {
        write_binary_file(file_path);
    } catch(exception const &ex) {
        binary_ofs.close();
        throw ex;
    }
}
void Binary_writer::generate_binary_file() {
    generate_binary_file(string("a"));
}

void Binary_writer::write_onestring(const std::string &string) {
    write_binary_uint((unsigned int) string.size());
    for(const char &ch : string)
        write_binary_char(ch);
    write_binary_char('\0');
}

void Binary_writer::write_totals(unsigned long totals) {
    write_binary_uint((unsigned int) totals);
}

void Binary_writer::write_vmarg(VMarg const *vmarg) {
    if(vmarg) {
        write_binary_boolean(true); //used
        write_binary_ushort((unsigned short) vmarg->type);  //type
        write_binary_uint(vmarg->value);    //value
    } else {
        write_binary_boolean(false);    //vmarg is not used. Generate dummy info
        write_binary_ushort((unsigned short) VMarg::Type::label);   //the combination of the label and the value will probably raise an exception
        write_binary_uint((unsigned int) vm_instructions.size());    //in case of logical error
    }
}

#define generic_write(lvalue) ( binary_ofs.write(reinterpret_cast<const char*>(&(lvalue)), sizeof(lvalue)) )
void Binary_writer::write_binary_dl(double long val) {
    generic_write(val);
}
void Binary_writer::write_binary_uint(unsigned int val) {
    generic_write(val);
}
void Binary_writer::write_binary_ushort(unsigned short val) {
    generic_write(val);
}
void Binary_writer::write_binary_char(char ch) {
    generic_write(ch);
}
void Binary_writer::write_binary_boolean(bool boolean) {
    generic_write(boolean);
}

