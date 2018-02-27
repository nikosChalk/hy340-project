//
// Created by nikos on 2/26/18.
//

#ifndef HY340_PROJECT_ANALYZER_H
#define HY340_PROJECT_ANALYZER_H

#include <vector>
#include <fstream>
#include <map>
#include <memory>
#include "DFA.h"

namespace alpha_lex {

    class analyzer {

    public:
        /**
         * Possible return values of alpha_yylex, based on what token it identified
         */
        enum identified_token{INPUT_ERROR=50, UNKNOWN=100, IF, ELSE, WHILE, FOR, FUNCTION, RETURN, BREAK, CONTINUE, AND,
            NOT, OR, LOCAL, BOOL_TRUE, BOOL_FALSE, NIL,
            OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD, OP_EQ, OP_NE, OP_PLUS_PLUS, OP_MINUS_MINUS,
            OP_GT, OP_LT, OP_GE, OP_LE,
            CONST_INT, CONST_REAL, CONST_STR, PUNCTUATION, IDENTIFIER, COMMENT
        };

        /**
         * Lexical analyzer for the "alpha" language
         * @param The input file to tokenize. Must have been opened.
         */
        explicit    analyzer(std::ifstream &input);

        /**
         * Reads from the input file and fetches the next token
         * @param ylval pointer to allocated memory where a alpha_token_t object will be stored
         * @return Returns an identified_token which indicates the token that was identified
         */
        int alpha_yylex(void *ylval);

    private:
        /**
         * Tags used by the dfa
         */
        static const std::string &tag_keyword, &tag_op, &tag_const_int, &tag_const_real, &tag_const_str, &tag_punctuation, &tag_id, &tag_line_comment, &tag_block_comment;
        static const std::map<std::string&, identified_token> tag_to_code;
        static const alpha_token_generator &generator;

        unsigned int current_line;
        std::vector<std::shared_ptr<DFA>> keyword_dfas, op_dfas, punctuation_dfas;
        std::shared_ptr<DFA> dfa_const_int_p, dfa_const_real_p, dfa_const_str_p, dfa_id_p, dfa_line_comment_p, dfa_block_comment_p;
        std::ifstream &input;
    };

}


#endif //HY340_PROJECT_ANALYZER_H