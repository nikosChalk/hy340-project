/**
 * Token generator for various categories.
 */

#ifndef HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H
#define HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H

#include "alpha_token.h"
#include <string>
#include <map>

namespace alpha_lex {

    class alpha_token_generator{
    public:
        /**
         * Creates a new token generator
         */
        alpha_token_generator();

        alpha_token_t* gen_keyword_token(unsigned int numline, std::string content);
        alpha_token_t* gen_operator_token(unsigned int numline, std::string content);
        alpha_token_t* gen_const_int_token(unsigned int numline, std::string content);
        alpha_token_t* gen_const_real_token(unsigned int numline, std::string content);
        alpha_token_t* gen_const_str_token(unsigned int numline, std::string content);
        alpha_token_t* gen_punctuation_token(unsigned int numline, std::string content);
        alpha_token_t* gen_identifier_token(unsigned int numline, std::string content);
        alpha_token_t* gen_comment_token(unsigned int numline, std::string content);

    private:
        std::map<std::string, std::string> op_to_opname;
        std::map<std::string, std::string> punctuation_to_puncname;
    };
}


#endif //HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H
