

#include <cassert>
#include <algorithm>
#include "analyzer.h"

using namespace std;

namespace alpha_lex {

    const string analyzer::tag_keyword = "KEYWORD";
    const string analyzer::tag_op = "OPERATOR";
    const string analyzer::tag_const_int = "CONST_INT";
    const string analyzer::tag_const_real = "CONST_REAL";
    const string analyzer::tag_const_str = "CONST_STR";
    const string analyzer::tag_punctuation = "PUNCTUATION";
    const string analyzer::tag_id = "ID";
    const string analyzer::tag_line_comment = "LINE_COMMENT";
    const string analyzer::tag_block_comment = "BLOCK_COMMENT";

    const std::map<string, analyzer::identified_token> analyzer::tag_to_code = {
            {"if", IF}, {"else", ELSE}
    };

    const alpha_token_generator analyzer::generator = alpha_token_generator();

    analyzer::analyzer(ifstream &input, ofstream &output)
        : input(input), output(output)
    {
        if(!input.is_open())
            throw invalid_argument("Input stream is not open!");
        if(!output.is_open())
            throw invalid_argument("Output stream is not open!");

        current_line = 0;
        keyword_dfas = vector<std::shared_ptr<DFA>>();
        op_dfas = vector<std::shared_ptr<DFA>>();
        punctuation_dfas = vector<std::shared_ptr<DFA>>();


        /*
        dfa_const_int_p = std::make_shared<DFA>(DFA(
                "0123456789", 10, {DFA::DFA_state(), DFA::DFA_state()}, DFA::DFA_state()
        ));
        */
        /* Add move rules */
        /*TODO: to be filled for all */

        /* TEST */

        keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "if", 2, {DFA::DFA_state("i", 1), DFA::DFA_state("f", 2, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[0]->add_move_rule(0, 'i', 1).add_move_rule(1, 'f', 2);
        
    }

    int analyzer::alpha_yylex(void *ylval) {
        char ch;

        vector<shared_ptr<DFA>> active;
        active.insert(active.end(), keyword_dfas.begin(), keyword_dfas.end() );
        /*
        active.insert(active.end(), op_dfas.begin(), op_dfas.end() );
        active.insert(active.end(), punctuation_dfas.begin(), punctuation_dfas.end() );
        active.push_back(dfa_const_int_p);
        active.push_back(dfa_const_real_p);
        active.push_back(dfa_const_str_p);
        active.push_back(dfa_id_p);
        active.push_back(dfa_line_comment_p);
        active.push_back(dfa_block_comment_p);
        */

        /* Reset the DFAs */
        for(int i=0; i<active.size(); i++)
            active[i]->reset();

        /* Scan input until a matching token is found or EOF or something bad happens */
        while(input.get(ch)) {
            /* Check if any of the parallel DFAs can move */
            bool exists_move = false;
            for(int i=0; i<active.size(); i++) {
                if(active[i]->has_move_rule(active[i]->current_state(),ch)) {
                    exists_move = true;
                    break;
                }
            }

            for(int i=0; i<active.size(); i++) {

                if(exists_move) { /* Move all the DFAs that can move */
                    try {
                        active[i]->move(ch);
                    } catch(DFA::DFA_state_error &error) {  /* Move failed because no rule exists */
                        active.erase(active.begin() + i);
                        i--;
                    }
                } else { /* No DFA can move. */
                    if(active[i]->current_state().is_final()) {
                        string cur_tag = active[i]->current_state().get_tag();

                        if(cur_tag == tag_keyword) {
                            generator.gen_keyword_token(current_line, active[i]->generate_token(), ylval);
                        } else if(cur_tag == tag_op) {

                        } else if(cur_tag == analyzer::tag_const_int) {

                        } else if(cur_tag == analyzer::tag_const_real) {

                        } else if(cur_tag == analyzer::tag_const_str) {

                        } else if(cur_tag == tag_punctuation) {

                        } else if(cur_tag == analyzer::tag_id) {

                        } else if(cur_tag == analyzer::tag_line_comment) {

                        } else if(cur_tag == analyzer::tag_block_comment) {

                        } else {
                            assert(false); /* unreachable statement */
                        }
                        input.unget();
                        return tag_to_code.at(cur_tag);
                    }
                }
            }

            if(!exists_move) {    /* No DFA can move and none of them is in final state */
                /* Found unknown token */
                input.unget();
                return UNKNOWN;
            }

            if(!input.good()) {
                if(input.eof()) {
                    /* EOF reached */
                    return EOF;
                } else {
                    /* Something bad happend */
                    return INPUT_ERROR;
                }
            }

        }
    }
}