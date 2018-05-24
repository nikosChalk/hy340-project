

#ifndef HY340_PROJECT_TOKEN_H
#define HY340_PROJECT_TOKEN_H

#include <string>

namespace virtual_machine {

    /**
     * Representation of Tokens for various types
     */
    struct Token {
        /**
         * Token types that the parser can match (terminal symbol categories)
         */
        enum Type {
            UNSIGNED_INT, DOUBLE_LONG, CHAR, UNSIGNED_SHORT
        };

        static std::string type_to_str(Type type);

        /**
         * Representation of the possible values that the token can take, based on the Token_type
         */
        union Value {
            unsigned int uint;      //used when type is UNSIGNED_INT
            double long doublel;    //used when type iS DOUBLE_LONG
            char ch;                //used when type is CHAR
            unsigned short ushort;  //used when type is UNSIGNED_SHORT
        };

        /**
         * Creates an empty token
         */
        Token();

        Type type;
        Value value;
        bool is_empty;  //true if the token is, i.e. it has invalid value and type. False otherwise
    };
}


#endif //HY340_PROJECT_TOKEN_H
