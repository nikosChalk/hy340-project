/**
 * Token generator for various categories.
 */

#ifndef HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H
#define HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H

#include "alpha_token.h"
#include <string>
#include <map>

namespace alpha_lex {

    class alpha_token_generator {
    public:
        /**
         * Creates a new token generator
         */
        alpha_token_generator();

        /**
         * Generates a new token for the keyword category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_keyword_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the operator category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         * @throw std::out_of_range If the given content is an operator that is not mapped to a name. Mapped operators are:
         * = + - * / % == != > < >= <= ++ --
         */
        alpha_token_t* gen_operator_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the const_int category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_const_int_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the const_real category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_const_real_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the const_str category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         * @throw std::runtime_error If content contains a malformed escaped character. Valid escaped characters are:
         *  '\ a' '\ b' '\ f' '\ n' '\ r' '\ t' '\ v' and '\ 0'
         */
        alpha_token_t* gen_const_str_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the punctuation category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         * @throw std::out_of_range If the given content is punctuation that is not mapped to a name. Mapped punctuation symbols are:
         * { } [ ] ( ) ; , : :: . and ..
         */
        alpha_token_t* gen_punctuation_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the identifier category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_identifier_token(unsigned int numline, const std::string &content, void *placement = nullptr) const;

        /**
         * Generates a new token for the comment category in the given numline and with content read from lex.
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The line number where the token was encountered
         * @param content The token's content as read from lex
         * @param sub_category Whether this is a line, comment, block comment or nested comment
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_comment_token(unsigned int numline, const std::string &content, const std::string &sub_category, void *placement = nullptr) const;

    private:
        std::map<std::string, std::string> op_to_opname;
        std::map<std::string, std::string> punctuation_to_puncname;

        /**
         * Generates a new token with the given attributes
         * If placement is given, the new token is initialized within the allocated placement memory with the new placement operator.
         * @param numline The token's line number
         * @param content The token's content attribute
         * @param category The token's category attribute
         * @param sub_category The token's sub-category attribute
         * @param category_type The token's category type attribute
         * @param placement The memory to place the token
         * @return A pointer to the constructed token. Placement is returned if not null.
         */
        alpha_token_t* gen_token(unsigned int numline, const std::string &content, const std::string &category,
                                 const std::string &sub_category, const std::string &category_type, void *placement) const;
    };
}


#endif //HY340_PROJECT_ALPHA_TOKEN_GENERATOR_H
