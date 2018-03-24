

#include <stdexcept>
#include <cassert>
#include "utilities.h"

using namespace std;

namespace utilities {

    string unescape_str(const string &str) {
        string ret_val;

        for(unsigned long i=0; i<str.length(); i++)
            ret_val += unescape_ch(str.at(i));
        return ret_val;
    }

    std::string unescape_ch(char ch) {
        if (ch == '\a')
            return std::string("\\a");
        else if (ch == '\b')
            return std::string("\\b");
        else if (ch == '\f')
            return std::string("\\f");
        else if (ch == '\n')
            return std::string("\\n");
        else if (ch == '\r')
            return std::string("\\r");
        else if (ch == '\t')
            return std::string("\\t");
        else if (ch == '\v')
            return std::string("\\v");
        else
            return std::string(1, ch);
    }

    string escape_str(const string &str) {
        string escaped = str;
        char to_replace;
        enum state{INITIAL, ESC};
        state cur_state = INITIAL;

        for(int i=0; i<escaped.length(); i++) {
            char ch = escaped[i];
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
                    else if(ch=='\'' || ch == '\"' || ch=='\?' || ch=='\\')
                        to_replace = ch;
                    else
                        throw std::runtime_error("Invalid escaped sequence: \"" + string(1, ch) + "\" inside string");

                    escaped.replace(i-1, 2, 1, to_replace);
                    cur_state = INITIAL;
                    i--;
                    break;
                default:
                    assert(false);  /* Unreachable code */
            }
        }
        return escaped;
    }
}