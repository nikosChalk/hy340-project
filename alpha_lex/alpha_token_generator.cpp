

#include "alpha_token_generator.h"
#include <algorithm>
#include <cassert>

namespace alpha_lex {

    alpha_token_generator::alpha_token_generator() {
        op_to_opname["="] = "ASSIGNMENT";
        op_to_opname["+"] = "PLUS";
        op_to_opname["-"] = "MINUS";
        op_to_opname["*"] = "MUL";
        op_to_opname["/"] = "DIV";
        op_to_opname["%"] = "MOD";
        op_to_opname["=="] = "EQUALS";
        op_to_opname["!="] = "NOT_EQUAL";
        op_to_opname["++"] = "PLUS_PLUS";
        op_to_opname["--"] = "MINUS_MINUS";
        op_to_opname[">"] = "GT";
        op_to_opname["<"] = "LT";
        op_to_opname[">="] = "GE";
        op_to_opname["<="] = "LE";

        punctuation_to_puncname["{"] = "LEFT_BRACE";
        punctuation_to_puncname["}"] = "RIGHT_BRACE";
        punctuation_to_puncname["["] = "LEFT_BRACKET";
        punctuation_to_puncname["]"] = "RIGHT_BRACKET";
        punctuation_to_puncname["("] = "LEFT_PARENTHESIS";
        punctuation_to_puncname[")"] = "RIGHT_PARENTHESIS";
        punctuation_to_puncname[";"] = "SEMICOLON";
        punctuation_to_puncname[","] = "COMMA";
        punctuation_to_puncname[":"] = "COLON";
        punctuation_to_puncname["::"] = "DOUBLE_COLON";
        punctuation_to_puncname["."] = "DOT";
        punctuation_to_puncname[".."] = "ELLIPSIS";
    }

    alpha_token_t* alpha_token_generator::gen_token(unsigned int numline, const std::string &content, const std::string &category,
                                                    const std::string &sub_category, const std::string &category_type, void *placement) const {
        if(placement == nullptr)
            return new alpha_token_t(numline, content, category, sub_category, category_type);
        else
            return new(placement) alpha_token_t(numline, content, category, sub_category, category_type);
    }

    alpha_token_t* alpha_token_generator::gen_keyword_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        std::string sub_category = std::string(content);
        std::transform(sub_category.begin(), sub_category.end(), sub_category.begin(), ::toupper);

        return (this->gen_token(numline, ("\"" + content + "\""), "KEYWORD", sub_category, "enumerated", placement));
    }

    alpha_token_t* alpha_token_generator::gen_operator_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        return (this->gen_token(numline, ("\"" + content + "\""), "OPERATOR", this->op_to_opname.at(content), "enumerated", placement));
    }

    alpha_token_t* alpha_token_generator::gen_const_int_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        return (this->gen_token(numline, ("\"" + content + "\""), "CONST_INT", content, "integer", placement));
    }

    alpha_token_t* alpha_token_generator::gen_const_real_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        return (this->gen_token(numline, ("\"" + content + "\""), "CONST_REAL", content, "real", placement));
    }

    alpha_token_t* alpha_token_generator::gen_const_str_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        std::string token_content = std::string("\"" + content + "\"");

        /* Replaced unescaped characters (e.g. char *str = {'\\' 'n'}; ) within sub_category with the corresponding escaped character */
        std::string sub_category = std::string(content);
        char to_replace;
        enum state{INITIAL, ESC};
        state cur_state = INITIAL;

        for(int i=0; i<sub_category.length(); i++) {
            char ch = sub_category[i];
            switch(cur_state){
                case INITIAL:
                    if(ch == '\\')
                        cur_state = ESC;
                    break;
                case ESC:
                    if(ch=='a')
                        to_replace = '\a';
                    else if(ch=='b')
                        to_replace = '\b';
                    else if(ch=='f')
                        to_replace = '\f';
                    else if(ch=='n')
                        to_replace = '\n';
                    else if(ch=='r')
                        to_replace = '\r';
                    else if(ch=='t')
                        to_replace = '\t';
                    else if(ch=='v')
                        to_replace = '\v';
                    else if(ch=='0')
                        to_replace = '\0';
                    else if(ch=='\'' || ch == '\"' || ch=='\?' || ch=='\\')
                        to_replace = ch;
                    else
                        throw std::runtime_error("Malformed escaped character in string");
                    sub_category.replace(i-1, 2, 1, to_replace);
                    cur_state = INITIAL;
                    i--;
                    break;
                default:
                    assert(false);  /* Unreachable code */

            }
        }
        sub_category = ("\"" + sub_category + "\"");
        return (this->gen_token(numline, token_content, "STRING", sub_category, "char*", placement));
    }

    alpha_token_t* alpha_token_generator::gen_punctuation_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        return (this->gen_token(numline, ("\"" + content + "\""), "PUNCTUATION", this->punctuation_to_puncname.at(content), "enumerated", placement));
    }

    alpha_token_t* alpha_token_generator::gen_identifier_token(unsigned int numline, const std::string &content, void *placement /*=nullptr*/) const {
        std::string token_content = std::string("\"" + content + "\"");
        return (this->gen_token(numline, content, "ID", content, "char*", placement));
    }

    alpha_token_t* alpha_token_generator::gen_comment_token(unsigned int numline, const std::string &content,
                                                            const std::string &sub_category, void *placement /*=nullptr*/) const {
        return (this->gen_token(numline, ("\"" + content + "\""), "COMMENT", sub_category, "enumerated", placement));
    }
}
