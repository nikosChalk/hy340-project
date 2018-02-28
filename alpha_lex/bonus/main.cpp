#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "../alpha_token.h"
#include "../alpha_token_generator.h"
#include "analyzer.h"

using namespace std;
using namespace alpha_lex;

int main (int argc, char *argv[]) {
    int identified;
    vector<alpha_token_t*> token_vector;
    allocator<alpha_token_t> allocator;
    alpha_token_t *token_ptr;
	std::shared_ptr<std::ifstream> in;
    std::shared_ptr<std::ofstream> out;
    ostream *ptr_out = &cout;

    /* Check command line arguments and open I/O files */
    if(argc < 2) {
        std::cerr << "Too few arguments. Expected input file." << std::endl;
        exit(EXIT_FAILURE);
    }
    if(argc >= 2) {
        in = std::make_shared<std::ifstream>(argv[1], ios_base::in);
        if(!in->good()) {
            cerr << "Error while opening input file: " << argv[1] << ". Execution aborted." << endl;
            exit(EXIT_FAILURE);
        }
        cout << "Using input file: " << argv[1] << endl;
    }
    if(argc>=3) {
        out = std::make_shared<std::ofstream>(argv[3], ios_base::out);
        if(!out->good()) {
            cerr << "Error while opening output file: " << argv[3] << ". Using stdout instead." << endl;
            ptr_out = &cout;
        } else {
			ptr_out = &(*out);
            cout << "Using output file: " << argv[2] << endl;
        }
    }

    analyzer my_analyzer(*in);
    token_ptr = 0;  /* ignore any compiler warnings */
    while(true) {
        token_ptr = allocator.allocate(1, token_ptr);

        try {
            identified = my_analyzer.alpha_yylex(token_ptr);
        } catch(runtime_error &error) {	/* Catch any possible errors such as unknown tokens, malformed escape sequences in strings, EOF reach while block comment is open, etc. */
            *ptr_out << error.what() << endl;
            allocator.deallocate(token_ptr, 1);
            break;
        }

        if(identified == EOF) {
            *ptr_out << "EOF reached. Success!" << endl;
            allocator.deallocate(token_ptr, 1);
            break;
        }

        /* Valid token here */
        *ptr_out << *token_ptr << endl;
        token_vector.push_back(token_ptr);
    }

    for(int i=0; i<token_vector.size(); i++) {
        token_vector[i]->~alpha_token_t();      /* Destructor call since placement new operator was used */
        allocator.deallocate(token_vector[i], 1);
    }

    return 0;
}