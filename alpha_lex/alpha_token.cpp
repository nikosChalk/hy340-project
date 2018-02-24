

#include "alpha_token.h"

using namespace std;

namespace alpha_lex {

    unsigned int alpha_token_t::next_token_num = 0;

    alpha_token_t::alpha_token_t(unsigned int numline, const string &content, const string &category,
                                 const string &sub_category, const string &category_type) {
        this->numline = numline;
        this->token_number = alpha_token_t::next_token_num;
        this->content = content;
        this->category = category;
        this->sub_category = sub_category;
        this->category_type = category_type;

        alpha_token_t::next_token_num++;
    }

    unsigned int alpha_token_t::get_numline() const {
        return this->numline;
    }

    unsigned int alpha_token_t::get_token_num() const {
        return this->token_number;
    }

    string alpha_token_t::get_content() const {
        return this->content;
    }

    string alpha_token_t::get_category() const {
        return this->category;
    }

    string alpha_token_t::get_sub_category() const {
        return this->sub_category;
    }


    string alpha_token_t::get_category_type() const {
        return this->category_type;
    }

    ostream& operator<<(ostream& os, const alpha_token_t& x) {
        string space = "    ";
        os << x.get_numline() << ":" + space + "#" << x.get_token_num()
           << space + x.get_content() << space + x.get_category() << space + x.get_sub_category()
           << space + "<--" + x.get_category_type();
        return os;
    }
}