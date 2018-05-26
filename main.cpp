

#include <iostream>
#include <cstdlib>
#include <sstream>
#include "alpha_lex/alpha_flex.h"
#include "syntax_analyzer/alpha_bison.h"
#include "intermediate_code/icode_generator.h"
#include "target_code/VMcode_generator.h"
#include "target_code/Binary_writer.h"

using namespace std;
using namespace target_code;
using namespace virtual_machine;

extern intermediate_code::icode_generator icode_gen;

int main (int argc, char *argv[]) {
    char const *src_file;
    int ret_val;
    syntax_analyzer::symbol_table sym_table = syntax_analyzer::symbol_table();

    /* Check command line arguments and open I/O files */
    if(argc < 2) {
        cerr << "Too few arguments. Expected input file." << endl;
        exit(EXIT_FAILURE);
    }
    src_file = argv[1];

    yyout = stdout;
    if(argc >= 2) {
        if(!(yyin=fopen(src_file, "r"))) {
            perror("Error while opening input file. Execution aborted");
            exit(EXIT_FAILURE);
        }
        cout << "Using input file: " << src_file << endl;
    }
    if(argc>=3) {
        if(!(yyout=fopen(argv[2], "w"))) {
            perror("Error while opening output file. Using stdout instead.");
            yyout = stdout;
        } else {
            cout << "Using output file: " << argv[2] << endl;
        }
    }

    try {
        ret_val = yyparse(sym_table);
        fprintf(yyout, "%s\n", sym_table.to_string().c_str());
        fprintf(yyout, "%s\n", icode_gen.to_string().c_str());

        if(ret_val == 0)
            fprintf(yyout, "EOF reached. Success!\n");
        else
            fprintf(yyout, "Error while parsing. yyparse returned %d\n", ret_val);
    } catch(runtime_error &err) {
        cerr << "Current Symbol Table Is:" << endl;
        cerr << sym_table << endl;

        cerr << icode_gen << endl;

        cerr << err.what() << endl;
        cerr << "Parsing Failed" << endl;
        exit(EXIT_FAILURE);
    }
    cout << endl;

    //Close files since we do not need them anymore
    if(yyin != stdin) {
        if(fclose(yyin) == EOF)
            perror("Error while closing input file.");
    }
    if(yyout != stdout) {
        if(fclose(yyout) == EOF)
            perror("Error while closing output file.");
    }

    VMcode_generator vmcode_generator = VMcode_generator(icode_gen.get_quad_vector());
    vmcode_generator.generate_target_code();
    cout << VMinstruction::to_string(vmcode_generator.get_vm_instr_vector());

    Binary_writer writer = Binary_writer(
            vmcode_generator.get_vm_instr_vector(), vmcode_generator.get_const_pool(), syntax_analyzer::get_total_program_vars()
    );

    try {
        writer.generate_binary_file(string(src_file));
    } catch(ofstream::failure const &err) {
        cerr << "Binary file generation failed: " << err.what() << endl;
    }

    return 0;
}