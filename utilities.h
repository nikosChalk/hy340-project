

#ifndef HY340_PROJECT_UTILITIES_H
#define HY340_PROJECT_UTILITIES_H

#include <string>

namespace utilities {

    /**
    * Checks if the given character is any of the '\ a' '\ b' '\ f' '\ n' '\ r' '\ t' '\ v' and returns its unescaped version (eg "\\n" for '\n')
    * In all other cases, ch is returned
    * @param ch The character to un-escape
    * @return The string which represents the un-escaped version of ch.
    */
    std::string unescape_ch(char ch);

    /**
    * Checks if the given string contains any of the characters '\ a' '\ b' '\ f' '\ n' '\ r' '\ t' '\ v'
    * and un-escapes them (e.g. '\n' becomes "\\n")
    * @param str The string to apply the un-escape operation
    * @return The string which represents the un-escaped version of str.
    */
    std::string unescape_str(const std::string &str);

}

#endif //HY340_PROJECT_UTILITIES_H
