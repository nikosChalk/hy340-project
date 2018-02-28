

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "../alpha_token.h"
#include "../alpha_token_generator.h"

using namespace std;
using namespace alpha_lex;

int main (int argc, char *argv[]) {
    int identified;
    vector<alpha_token_t*> token_vector;
    allocator<alpha_token_t> allocator;
    alpha_token_t *token_ptr;
    ifstream *input;
    ifstream *output;


    /* Check command line arguments and open I/O files */
    if(argc < 2) {
        std::cerr << "Too few arguments. Expected input file." << std::endl;
        exit(EXIT_FAILURE);
    }
    if(argc >= 2) {
        ifstream in = ifstream(argv[1], ios_base::in);

        if(!(=fopen(argv[1], "r"))) {
            perror("Error while opening input file. Execution aborted");
            exit(EXIT_FAILURE);
        }
        std::cout << "Using input file: " << argv[1] << std::endl;
    }
    if(argc>=3) {
        if(!(=fopen(argv[2], "w"))) {
            perror("Error while opening output file. Using stdout instead.");
            yyouta = stdout;
        } else {
            std::cout << "Using output file: " << argv[2] << std::endl;
        }
    }

    token_ptr = 0;  /* ignore any compiler warnings */
    while(true) {
        token_ptr = allocator.allocate(1, token_ptr);
        identified = alpha_yylex(token_ptr);
        if(identified == EOF || identified == UNKNOWN) {
            if(identified == UNKNOWN)
                fprintf(yyout, "UNIDENTIFIED TOKEN: %s\n", yytext);
            else
                fprintf(yyout, "EOF reached\n");
            allocator.deallocate(token_ptr, 1);
            break;
        }

        /* Valid token here */
        fprintf(yyout, "%s\n", (token_ptr->to_str().c_str()));
        token_vector.push_back(token_ptr);
    }

    for(int i=0; i<token_vector.size(); i++) {
        token_vector[i]->~alpha_token_t();      /* Destructor call since placement new operator was used */
        allocator.deallocate(token_vector[i], 1);
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