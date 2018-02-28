

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
		
		/* IF */
        keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "if", 2, {DFA::DFA_state("i", 1), DFA::DFA_state(tag_keyword, 2, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[0]->add_move_rule(0, 'i', 1).add_move_rule(1, 'f', 2);
        
		/* ELSE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"else", 4, {DFA::DFA_state("e", 1), DFA::DFA_state("l", 2), DFA::DFA_state("s", 3), DFA::DFA_state(tag_keyword, 4, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[1]->add_move_rule(0, 'e', 1).add_move_rule(1, 'l', 2).add_move_rule(2, 's', 3).add_move_rule(3, 'e', 4);
		
		/* WHILE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"while", 5, {DFA::DFA_state("w", 1), DFA::DFA_state("h", 2), DFA::DFA_state("i", 3), DFA::DFA_state("l", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[2]->add_move_rule(0, 'w', 1).add_move_rule(1, 'h', 2).add_move_rule(2, 'i', 3).add_move_rule(3, 'l', 4).add_move_rule(4, 'e', 5);
		
		/* FOR */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "for", 3, {DFA::DFA_state("f", 1), DFA::DFA_state("o", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[3]->add_move_rule(0, 'f', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'r', 3);
		
		/* FUNCTION */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"function", 8, {DFA::DFA_state("f", 1), DFA::DFA_state("u", 2), DFA::DFA_state("n", 3), DFA::DFA_state("c", 4), DFA::DFA_state("t", 5), DFA::DFA_state("i", 6), DFA::DFA_state("o", 7), DFA::DFA_state(tag_keyword, 8, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[4]->add_move_rule(0, 'f', 1).add_move_rule(1, 'u', 2).add_move_rule(2, 'n', 3).add_move_rule(3, 'c', 4).add_move_rule(4, 't', 5).add_move_rule(5, 'i', 6).add_move_rule(6, 'o', 7).add_move_rule(7, 'n', 8);

		/* RETURN */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"return", 6, {DFA::DFA_state("r", 1), DFA::DFA_state("e", 2), DFA::DFA_state("t", 3), DFA::DFA_state("u", 4), DFA::DFA_state("r", 5), DFA::DFA_state(tag_keyword, 6, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[5]->add_move_rule(0, 'r', 1).add_move_rule(1, 'e', 2).add_move_rule(2, 't', 3).add_move_rule(3, 'u', 4).add_move_rule(4, 'r', 5).add_move_rule(5, 'n', 6);
		
		/* BREAK */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"break", 5, {DFA::DFA_state("b", 1), DFA::DFA_state("r", 2), DFA::DFA_state("e", 3), DFA::DFA_state("a", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[6]->add_move_rule(0, 'b', 1).add_move_rule(1, 'r', 2).add_move_rule(2, 'e', 3).add_move_rule(3, 'a', 4).add_move_rule(4, 'k', 5);
		
		/* CONTINUE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"continue", 8, {DFA::DFA_state("c", 1), DFA::DFA_state("o", 2), DFA::DFA_state("n", 3), DFA::DFA_state("t", 4), DFA::DFA_state("i", 5), DFA::DFA_state("n", 6), DFA::DFA_state("u", 7), DFA::DFA_state(tag_keyword, 8, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[7]->add_move_rule(0, 'c', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'n', 3).add_move_rule(3, 't', 4).add_move_rule(4, 'i', 5).add_move_rule(5, 'n', 6).add_move_rule(6, 'u', 7).add_move_rule(7, 'e', 8);

		/* AND */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "and", 3, {DFA::DFA_state("a", 1), DFA::DFA_state("n", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[8]->add_move_rule(0, 'a', 1).add_move_rule(1, 'n', 2).add_move_rule(2, 'd', 3);
		
		/* NOT */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "not", 3, {DFA::DFA_state("n", 1), DFA::DFA_state("o", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[9]->add_move_rule(0, 'n', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 't', 3);
		
		/* OR */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "or", 2, {DFA::DFA_state("o", 1), DFA::DFA_state(tag_keyword, 2, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[10]->add_move_rule(0, 'o', 1).add_move_rule(1, 'r', 2);

		/* LOCAL */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"local", 5, {DFA::DFA_state("l", 1), DFA::DFA_state("o", 2), DFA::DFA_state("c", 3), DFA::DFA_state("a", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[11]->add_move_rule(0, 'l', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'c', 3).add_move_rule(3, 'a', 4).add_move_rule(4, 'l', 5);
		
		/* TRUE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"true", 4, {DFA::DFA_state("t", 1), DFA::DFA_state("r", 2), DFA::DFA_state("u", 3), DFA::DFA_state(tag_keyword, 4, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[12]->add_move_rule(0, 't', 1).add_move_rule(1, 'r', 2).add_move_rule(2, 'u', 3).add_move_rule(3, 'e', 4);
		
		/* FALSE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"false", 5, {DFA::DFA_state("f", 1), DFA::DFA_state("a", 2), DFA::DFA_state("l", 3), DFA::DFA_state("s", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[13]->add_move_rule(0, 'f', 1).add_move_rule(1, 'a', 2).add_move_rule(2, 'l', 3).add_move_rule(3, 's', 4).add_move_rule(4, 'e', 5);
		
		/* NIL */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "nil", 3, {DFA::DFA_state("n", 1), DFA::DFA_state("i", 2), DFA::DFA_state("l", 3), DFA::DFA_state(tag_keyword, 4, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[14]->add_move_rule(0, 'n', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 't', 3);
		
		/* = */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"=", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[0]->add_move_rule(0, '=', 1);
		
		/* + */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"+", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[1]->add_move_rule(0, '+', 1);
		
		/* - */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"-", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[2]->add_move_rule(0, '-', 1);
		
		/* * */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"*", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[3]->add_move_rule(0, '*', 1);
		
		/* / */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"/", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[4]->add_move_rule(0, '/', 1);
		
		/* % */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"%", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[5]->add_move_rule(0, '%', 1);
		
		/* == */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"==", 2, {DFA::DFA_state("=", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[6]->add_move_rule(0, '=', 1).add_move_rule(1, '=', 2);
		
		/* != */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"!=", 2, {DFA::DFA_state("!", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[7]->add_move_rule(0, '!', 1).add_move_rule(1, '=', 2);
		
		/* ++ */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"++", 2, {DFA::DFA_state("+", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[8]->add_move_rule(0, '+', 1).add_move_rule(1, '+', 2);
		
		/* -- */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"--", 2, {DFA::DFA_state("-", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[9]->add_move_rule(0, '-', 1).add_move_rule(1, '-', 2);
		
		/* > */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				">", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[10]->add_move_rule(0, '>', 1);
		
		/* < */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"<", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[11]->add_move_rule(0, '<', 1);
		
		/* >= */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				">=", 2, {DFA::DFA_state(">", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[11]->add_move_rule(0, '>', 1).add_move_rule(1, '=', 2);
		
		/* <= */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"<=", 2, {DFA::DFA_state("<", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[12]->add_move_rule(0, '<', 1).add_move_rule(1, '=', 2);
		
		/* { */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"{", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[13]->add_move_rule(0, '{', 1);
		
		/* } */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"}", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[14]->add_move_rule(0, '}', 1);
		
		/* [ */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"[", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[15]->add_move_rule(0, '[', 1);
		
		/* ] */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"]", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[16]->add_move_rule(0, ']', 1);
		
		/* ( */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"(", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[17]->add_move_rule(0, '(', 1);
		
		/* ) */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				")", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[18]->add_move_rule(0, ')', 1);
		
		/* ; */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				";", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[19]->add_move_rule(0, ';', 1);
		
		/* , */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				",", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[20]->add_move_rule(0, ',', 1);
		
		/* : */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				":", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[21]->add_move_rule(0, ':', 1);
		
		/* :: */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"::", 2, {DFA::DFA_state(":", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[22]->add_move_rule(0, ':', 1).add_move_rule(1, ':', 2);
		
		/* . */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				".", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[23]->add_move_rule(0, '.', 1);
		
		/* .. */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"..", 2, {DFA::DFA_state(".", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[24]->add_move_rule(0, '.', 1).add_move_rule(1, '.', 2);
		
		/* const int */
		
		dfa_const_int_p(std::make_shared<DFA>(DFA(
				"0123456789", 10, {DFA::DFA_state(tag_const_int, 1, true)}, DFA::DFA_state("", 0) 
		)));
		for(char c='0'; c<='9'; c++){
			dfa_const_int_p->add_move_rule(0, c, 1);
			dfa_const_int_p->add_move_rule(1, c, 1);
		}
		
		/* digit.digit */
		
		dfa_const_real_p(std::make_shared<DFA>(
				"0123456789.", 11, {DFA::DFA_state("0-9" , 1), DFA::DFA_state("0-9." , 2), DFA::DFA_state(tag_const_real , 3, true)}, DFA::DFA_state("", 0)
		)));
		for(char c='0'; c<='9'; c++){
			dfa_const_real_p->add_move_rule(0, c, 1);
			dfa_const_real_p->add_move_rule(1, c, 1);
			dfa_const_real_p->add_move_rule(2, c, 3);
			dfa_const_real_p->add_move_rule(3, c, 3);
		}
		
		dfa_const_real_p->add_move_rule(1, '.', 2);
		dfa_const_real_p->add_move_rule(0, '.', 2);
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