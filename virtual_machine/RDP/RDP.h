

#ifndef HY340_PROJECT_RDP_H
#define HY340_PROJECT_RDP_H

#include <vector>
#include <fstream>
#include "Token.h"
#include "../../common_interface/types.h"
#include "../../common_interface/Constants_pool.h"

namespace virtual_machine {
    class RDP {
    public:
        /**
         * Initializes the RDP
         */
        RDP();

        /**
         * Parses the given file
         * @param file_path The path leading to the file which the RDP should parse
         * @throws ifstream::failure if any error occured during the parsing of the file
         */
        void parse(const std::string &file_path);

        /**
         * Returns the instructions read from the parsed alpha file
         */
        const std::vector<VMinstruction> get_instructions() const;

        /**
         * Returns the pool of constants which were read from the binary fily
         * @return The constants pool
         */
        Constants_pool get_const_pool() const;

        /**
         * Returns the number of total program variables used within the alpha file
         */
        unsigned int get_nr_total_program_vars() const;

    private:
        Constants_pool const_pool;
        std::ifstream ifs;      //input file stream for the file which the RDP should parse

        std::vector<VMinstruction> instructions;
        unsigned int total_program_vars;

        /**
         * Representation of the special Lookahead token
         */
        class Lookahead : public Token {
        public:
            /**
             * Initializes the lookahead token
             * @param outer The RDP in which the lookahead token exists (The outer class instance)
             */
            explicit Lookahead(RDP &outer_rdp);

            /**
             * Checks if the lookahead token has the same Token_type as the given type
             * If the lookahead is empty, then a token is read from RDP::ifs with the given type and is stored in lookahead
             * @param type The type of the lookahead token that it must be
             *
             * @throws ifstream::failure If the given token_type does not match the lookahead's token type
             */
            void match(Type token_type);

            /**
             * Calls match() and then returns the token value of the lookahead token, effectively consuming it
             * Afterwards, lookahead will be empty
             * @param token_type The token type that to be consumed
             * @return The token value of the look ahead
             *
             * @throws ifstream::failure If the given token_type does not match the lookahead's token type
             */
            Value consume(Type token_type);

        private:
            RDP &outer_rdp; //reference to the instance of the outer class

            /**
             * Reads a token of the given type from the outer.ifs
             * Note that the lookahead token must be empty
             * @param token_type The token type to read
             */
            void next_token(Type token_type);
        };

        /**
         * lookahead is the next token that we have available for consumption
         */
        Lookahead lookahead;

        /**
         * Same as Lookahead::match
         */
        inline void match(Token::Type token_type);
        /**
         * Same as Lookahead::consume
         */
        inline Token::Value consume(Token::Type token_type);


        void rule_binaryfile();
        void rule_magicnumber();

        void rule_arrays();         //adds to strings in const_pool
        std::vector<std::string> rule_strings();
        void rule_numbers();        //adds to numbers in const_pool
        void rule_userfunctions();  //adds to userfuncs in const_pool
        void rule_libfunctions();   //adds to libfuncs in const_pool
        void rule_code();           //sets this->instructions vector
        void rule_nr_program_vars();    //sets this->total_program_vars

        unsigned int rule_total();

        unsigned int rule_strsize();    //@return strlen(str), i.e. the number of characters in str EXCLUDING '\0'
        std::string rule_onestring();   //For each string we write its content and then append a '\0'


        Userfunc rule_oneuserfunc();
        unsigned int rule_address();
        unsigned int rule_nr_locals();
        std::string rule_id();

        VMinstruction rule_oneinstruction();
        VMopcode rule_vmopcode();
        VMarg* rule_vmarg();    //The memory where the returned VMarg is stored is allocated through "new" operator
        bool rule_vmarg_used();
        VMarg::Type  rule_vmarg_type();
        unsigned int rule_vmarg_value();
        unsigned int rule_srcline();
    };

};


#endif //HY340_PROJECT_RDP_H
