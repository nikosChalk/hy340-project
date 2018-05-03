

#include <iostream>
#include "alpha_lex/alpha_flex.h"
#include "syntax_analyzer/symbol_table.h"
#include "syntax_analyzer/alpha_bison.h"
#include "intermediate_code/icode_generator.h"

using namespace std;

int main (int argc, char *argv[]) {
    int ret_val;
    syntax_analyzer::symbol_table sym_table = syntax_analyzer::symbol_table();
	intermediate_code::icode_generator icode_gen = intermediate_code::icode_generator();

    /* Check command line arguments and open I/O files */
    if(argc < 2) {
        cerr << "Too few arguments. Expected input file." << endl;
        exit(EXIT_FAILURE);
    }

    yyout = stdout;
    if(argc >= 2) {
        if(!(yyin=fopen(argv[1], "r"))) {
            perror("Error while opening input file. Execution aborted");
            exit(EXIT_FAILURE);
        }
        cout << "Using input file: " << argv[1] << endl;
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
        ret_val = yyparse(sym_table,icode_gen);
        fprintf(yyout, "%s", sym_table.to_string().c_str());
        if(ret_val == 0)
            fprintf(yyout, "EOF reached. Success!\n");
        else
            fprintf(yyout, "Error while parsing. yyparse returned %d\n", ret_val);
    } catch(runtime_error &err) {
        cerr << "Current Symbol Table Is: " << endl;
        cerr << sym_table;
        cerr << err.what() << endl;
        cerr << "Parsing Failed" << endl;
    }

    if(yyin != stdin) {
        if(fclose(yyin) == EOF)
            perror("Error while closing input file.");
    }
    if(yyout != stdout) {
        if(fclose(yyout) == EOF)
            perror("Error while closing output file.");
    }

    return 0;
}