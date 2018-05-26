

#ifndef HY340_PROJECT_BINARY_WRITER_H
#define HY340_PROJECT_BINARY_WRITER_H

#include <vector>
#include <fstream>
#include "../common_interface/vm_types.h"
#include "../common_interface/Constants_pool.h"

namespace target_code {

    class Binary_writer {

    public:
        Binary_writer(std::vector<virtual_machine::VMinstruction> const &vm_instructions,
                      const virtual_machine::Constants_pool &const_pool,
                      unsigned int total_program_vars);


        /**
         * Generates the binary file which is in the same path as the source file. The binary file has the same name as
         * the source path but with the extension ".abc"
         * @param file_path The source file path
         *
         * @throws ofstream::failure If any error occured during the binary file generation
         */
        void generate_binary_file(const std::string &file_path);
        /**
         * Same as generate_binary_file() but with a default "a.abc" binary file name existing in the same directory
         * where the compiler was invoked
         */
        void generate_binary_file();

    private:
        unsigned int total_program_vars; //total_program_vars The number of total program scope space variables used in the alpha binary source code

        std::vector<virtual_machine::VMinstruction> vm_instructions;
        virtual_machine::Constants_pool const_pool;

        std::ofstream binary_ofs;

        /* Helper functions for writing to the binary file */

        /**
         * Internal function that writes the binary file. This function does not handle internal exceptions thus in case
         * of an exception, the binary_ofs must be closed by the caller
         */
        void write_binary_file(const std::string &file_path);

        /**
         * Writes the given string to the binary_ofs, i.e. performs "string" grammar rule
         */
        void write_onestring(const std::string &string);

        /**
         * Writes the given totals as UNSIGNED INT to the binary_ofs, i.e. performs the "totals" grammar rule
         * @param totals The totals to write, which is CONVERTED to unsigned int
         */
        void write_totals(unsigned long totals);

        void write_vmarg(virtual_machine::VMarg const *vmarg);

        /**
         * Writes the given double long value to the binary_ofs
         */
        void write_binary_dl(double long val);

        /**
         * Writes the given unsigned int value to the binary_ofs
         */
        void write_binary_uint(unsigned int val);

        /**
         * Writes the given unsigned short value to the binary_ofs
         */
        void write_binary_ushort(unsigned short val);

        /**
         * Writes the given char value to the binary_ofs
         */
        void write_binary_char(char ch);

        /**
         * Writes the given boolean value to the binary_ofs
         */
        void write_binary_boolean(bool boolean);
    };

}


#endif //HY340_PROJECT_BINARY_WRITER_H
