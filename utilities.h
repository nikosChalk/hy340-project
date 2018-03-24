

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

    /**
     * Applies the escaping operation on the given str. If the string contains a backslash (\)
     * followed by either: 'a' 'b' 'f' 'n' 'r' 't' 'v'
     * then these two characters are re-placed by their corresponding escape character
     * @param str The string to apply the escape operation
     * @throws std::runtime_error if an escaped character was found which does not belong to the above mentioned.
     * @return The string which represents the escaped version of str.
     */
    std::string escape_str(const std::string &str);

}

#endif //HY340_PROJECT_UTILITIES_H
