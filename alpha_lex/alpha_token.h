

#ifndef HY340_PROJECT_ALPHA_TOKEN_H
#define HY340_PROJECT_ALPHA_TOKEN_H

#include <string>

namespace alpha_lex {

    class alpha_token_t {
    public:
        enum type_t{
            KEYWORD,    // leksh kleidi: if, else, while, for, function, return, break, continue, and, not, or, local, true, false, nil
            OPERATOR,   // telesth: = + - * / % == != ++ -- > < >= <=
            CONST_INT,  // akeraia arithmitikh stathera
            CONST_REAL, // pragmatikh arithmitkh stathera
            STRING,     // stathera omada xarakthrwn (string)
            PUNCTUATION, // shmeio stikshs: { } [ ] ( ) ; , : :: . ..
            ID,         // anagnwristiko onoma
            COMMENT     // sxolia
        };

        unsigned int get_numline() const;
        unsigned int get_token_number() const;
        std::string get_content() const;
        type_t get_type() const;

    private:
        static unsigned int next_token_num;

        unsigned int numline;
        unsigned int token_number;
        std::string content;
        type_t type;
    };
}

#endif //HY340_PROJECT_ALPHA_TOKEN_H
