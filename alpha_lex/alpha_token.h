

#ifndef HY340_PROJECT_ALPHA_TOKEN_H
#define HY340_PROJECT_ALPHA_TOKEN_H

#include <string>
#include <ostream>
#include <map>

namespace alpha_lex {

    /**
     * Represents a runtime_error that occurs when an undefined token is encountered
     */
    class undefined_token: public std::runtime_error {

    public:
        explicit undefined_token(const std::string &msg);
    };

    class alpha_token_t {
    public:

        /**
         * Creates a new token with the given attributes
         * @param numline The line number where the token was found, as shown in the example's 2nd column (e.g. 1)
         * @param content The token's content as shown in the example's 3rd column (e.g. "function")
         * @param category The token's category as shown in the example's 4th column. (e.g. PUNCTUATION)
         * @param sub_category The token's sub-category as shown in the example's 5th column. (e.g. SEMICOLON)
         * @param category_type The token's category type as shown in the example's last column. (e.g. char*)
         */
        alpha_token_t(unsigned int numline, const std::string &content, const std::string &category,
                                     const std::string &sub_category, const std::string &category_type);

        /**
         * Returns a string representation of this object in printable format (with no newline at the end)
         * @return The string representation
         */
        std::string to_str() const;

        /**
         * Returns the line number of this token
         * @return The line number of this token
         */
        unsigned int get_numline() const;

        /**
         * Returns a unique number (id) for each token
         * @return The token's id
         */
        unsigned int get_token_num() const;

        /**
         * Returns the token's content
         * @return The token's content
         */
        std::string get_content() const;

        /**
         * Returns the token's category as shown in the example's 4th column. (e.g. PUNCTUATION)
         * @return The token's category
         */
        std::string get_category() const;

        /**
         * Returns the token's sub-category as shown in the example's 5th column. (e.g. SEMICOLON)
         * @return The token's sub-category
         */
        std::string get_sub_category() const;

        /**
         * Returns the token's category type as shown in the example's last column. (e.g. char*)
         * @return The token's category type
         */
        std::string get_category_type() const;


        friend std::ostream& operator<<(std::ostream& os, const alpha_token_t& x);

    private:
        static unsigned int next_token_num;


        unsigned int numline, token_number;
        std::string content, category, sub_category, category_type;
    };
}

#endif //HY340_PROJECT_ALPHA_TOKEN_H
