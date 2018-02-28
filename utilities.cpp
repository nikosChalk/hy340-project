

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
}