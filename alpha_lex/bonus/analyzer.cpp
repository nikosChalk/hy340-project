

#include <cassert>
#include <algorithm>
#include <cstring>
#include <iostream>
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
    const string analyzer::tag_nested_comment = "NESTED_COMMENT";
    const string analyzer::tag_ws = "WHITESPACE";

    const std::map<string, analyzer::identified_token> analyzer::tag_to_code = {
            {"if", IF}, {"else", ELSE}, {"while", WHILE}, {"for", FOR}, {"function", FUNCTION}, {"return", RETURN},
            {"break", BREAK}, {"continue", CONTINUE}, {"and", AND}, {"not", NOT}, {"or", OR}, {"local", LOCAL},
            {"true", BOOL_TRUE}, {"false", BOOL_FALSE}, {"nil", NIL},

            {"=", OP_ASSIGN}, {"+", OP_PLUS}, {"-", OP_MINUS}, {"*", OP_MUL}, {"/", OP_DIV}, {"%", OP_MOD},
            {"==", OP_EQ}, {"!=", OP_NE}, {"++", OP_PLUS_PLUS}, {"--", OP_MINUS_MINUS}, {">", OP_GT},
            {"<", OP_LT}, {">=", OP_GE}, {"<=", OP_LE},

            {"{", PUNCT_LEFT_BRACE}, {"}", PUNCT_RIGHT_BRACE}, {"[", PUNCT_LEFT_BRACKET}, {"]", PUNCT_RIGHT_BRACKET},
            {"(", PUNCT_LEFT_PARENTHESIS}, {")", PUNCT_RIGHT_PARENTHESIS}, {";", PUNCT_SEMICOLON},
            {",", PUNCT_COMMA}, {":", PUNCT_COLON,}, {"::", PUNCT_DOUBLE_COLON}, {".", PUNCT_DOT}, {"..", PUNCT_ELIPSIS},

            {tag_const_int, CONST_INT}, {tag_const_real, CONST_REAL}, {tag_const_str, CONST_STR},
            {tag_id, IDENTIFIER}, {tag_line_comment, LINE_COMMENT}, {tag_block_comment, BLOCK_COMMENT},
            {tag_nested_comment, NESTED_COMMENT},

            {tag_ws, WHITE_SPACE}
    };

    const alpha_token_generator analyzer::generator = alpha_token_generator();

    analyzer::analyzer(ifstream &input)
        : input(input)
    {
        current_line = 1;
        keyword_dfas = vector<std::shared_ptr<DFA>>();
        op_dfas = vector<std::shared_ptr<DFA>>();
        punctuation_dfas = vector<std::shared_ptr<DFA>>();

        string all_characters;
        for(int j=0; j<=255; j++)
            all_characters += (char)j;

        string all_letters, all_digits;
        for(char c='a'; c<'z'; c++)
            all_letters += c;
        for(char c='A'; c<'Z'; c++)
            all_letters += c;
        for(char c='0'; c<'9'; c++)
            all_digits += c;


		/* IF */
        keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "if", 2, {DFA::DFA_state("i", 1), DFA::DFA_state(tag_keyword, 2, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'i', 1).add_move_rule(1, 'f', 2);
        
		/* ELSE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"else", 4, {DFA::DFA_state("e", 1), DFA::DFA_state("l", 2), DFA::DFA_state("s", 3), DFA::DFA_state(tag_keyword, 4, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'e', 1).add_move_rule(1, 'l', 2).add_move_rule(2, 's', 3).add_move_rule(3, 'e', 4);
		
		/* WHILE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"while", 5, {DFA::DFA_state("w", 1), DFA::DFA_state("h", 2), DFA::DFA_state("i", 3), DFA::DFA_state("l", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'w', 1).add_move_rule(1, 'h', 2).add_move_rule(2, 'i', 3).add_move_rule(3, 'l', 4).add_move_rule(4, 'e', 5);
		
		/* FOR */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "for", 3, {DFA::DFA_state("f", 1), DFA::DFA_state("o", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'f', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'r', 3);
		
		/* FUNCTION */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"function", 8, {DFA::DFA_state("f", 1), DFA::DFA_state("u", 2), DFA::DFA_state("n", 3), DFA::DFA_state("c", 4), DFA::DFA_state("t", 5), DFA::DFA_state("i", 6), DFA::DFA_state("o", 7), DFA::DFA_state(tag_keyword, 8, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'f', 1).add_move_rule(1, 'u', 2).add_move_rule(2, 'n', 3).add_move_rule(3, 'c', 4).add_move_rule(4, 't', 5).add_move_rule(5, 'i', 6).add_move_rule(6, 'o', 7).add_move_rule(7, 'n', 8);

		/* RETURN */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"return", 6, {DFA::DFA_state("r", 1), DFA::DFA_state("e", 2), DFA::DFA_state("t", 3), DFA::DFA_state("u", 4), DFA::DFA_state("r", 5), DFA::DFA_state(tag_keyword, 6, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'r', 1).add_move_rule(1, 'e', 2).add_move_rule(2, 't', 3).add_move_rule(3, 'u', 4).add_move_rule(4, 'r', 5).add_move_rule(5, 'n', 6);
		
		/* BREAK */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"break", 5, {DFA::DFA_state("b", 1), DFA::DFA_state("r", 2), DFA::DFA_state("e", 3), DFA::DFA_state("a", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'b', 1).add_move_rule(1, 'r', 2).add_move_rule(2, 'e', 3).add_move_rule(3, 'a', 4).add_move_rule(4, 'k', 5);
		
		/* CONTINUE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"continue", 8, {DFA::DFA_state("c", 1), DFA::DFA_state("o", 2), DFA::DFA_state("n", 3), DFA::DFA_state("t", 4), DFA::DFA_state("i", 5), DFA::DFA_state("n", 6), DFA::DFA_state("u", 7), DFA::DFA_state(tag_keyword, 8, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'c', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'n', 3).add_move_rule(3, 't', 4).add_move_rule(4, 'i', 5).add_move_rule(5, 'n', 6).add_move_rule(6, 'u', 7).add_move_rule(7, 'e', 8);

		/* AND */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "and", 3, {DFA::DFA_state("a", 1), DFA::DFA_state("n", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'a', 1).add_move_rule(1, 'n', 2).add_move_rule(2, 'd', 3);
		
		/* NOT */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "not", 3, {DFA::DFA_state("n", 1), DFA::DFA_state("o", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'n', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 't', 3);
		
		/* OR */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "or", 2, {DFA::DFA_state("o", 1), DFA::DFA_state(tag_keyword, 2, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'o', 1).add_move_rule(1, 'r', 2);

		/* LOCAL */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"local", 5, {DFA::DFA_state("l", 1), DFA::DFA_state("o", 2), DFA::DFA_state("c", 3), DFA::DFA_state("a", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'l', 1).add_move_rule(1, 'o', 2).add_move_rule(2, 'c', 3).add_move_rule(3, 'a', 4).add_move_rule(4, 'l', 5);
		
		/* TRUE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"true", 4, {DFA::DFA_state("t", 1), DFA::DFA_state("r", 2), DFA::DFA_state("u", 3), DFA::DFA_state(tag_keyword, 4, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 't', 1).add_move_rule(1, 'r', 2).add_move_rule(2, 'u', 3).add_move_rule(3, 'e', 4);
		
		/* FALSE */
		keyword_dfas.push_back(std::make_shared<DFA>(DFA(
				"false", 5, {DFA::DFA_state("f", 1), DFA::DFA_state("a", 2), DFA::DFA_state("l", 3), DFA::DFA_state("s", 4), DFA::DFA_state(tag_keyword, 5, true)}, DFA::DFA_state("", 0)
		)));
		keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'f', 1).add_move_rule(1, 'a', 2).add_move_rule(2, 'l', 3).add_move_rule(3, 's', 4).add_move_rule(4, 'e', 5);
		
		/* NIL */
		 keyword_dfas.push_back(std::make_shared<DFA>(DFA(
                "nil", 3, {DFA::DFA_state("n", 1), DFA::DFA_state("i", 2), DFA::DFA_state(tag_keyword, 3, true)}, DFA::DFA_state("", 0)
        )));
        keyword_dfas[keyword_dfas.size()-1]->add_move_rule(0, 'n', 1).add_move_rule(1, 'i', 2).add_move_rule(2, 'l', 3);
		
		/* = */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"=", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '=', 1);
		
		/* + */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"+", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '+', 1);
		
		/* - */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"-", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '-', 1);
		
		/* * */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"*", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '*', 1);
		
		/* / */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"/", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '/', 1);
		
		/* % */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"%", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '%', 1);
		
		/* == */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"=", 1, {DFA::DFA_state("=", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '=', 1).add_move_rule(1, '=', 2);
		
		/* != */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"!=", 2, {DFA::DFA_state("!", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '!', 1).add_move_rule(1, '=', 2);
		
		/* ++ */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"+", 1, {DFA::DFA_state("+", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '+', 1).add_move_rule(1, '+', 2);
		
		/* -- */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"-", 1, {DFA::DFA_state("-", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '-', 1).add_move_rule(1, '-', 2);
		
		/* > */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				">", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '>', 1);
		
		/* < */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"<", 1, {DFA::DFA_state(tag_op, 1, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '<', 1);
		
		/* >= */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				">=", 2, {DFA::DFA_state(">", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '>', 1).add_move_rule(1, '=', 2);
		
		/* <= */
		op_dfas.push_back(std::make_shared<DFA>(DFA(
				"<=", 2, {DFA::DFA_state("<", 1), DFA::DFA_state(tag_op, 2, true)}, DFA::DFA_state("", 0)
		)));
		op_dfas[op_dfas.size()-1]->add_move_rule(0, '<', 1).add_move_rule(1, '=', 2);


        /* PUNCTUATION */

		/* { */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				"{", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '{', 1);
		
		/* } */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				"}", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '}', 1);
		
		/* [ */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				"[", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '[', 1);
		
		/* ] */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				"]", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ']', 1);
		
		/* ( */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				"(", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '(', 1);
		
		/* ) */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				")", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ')', 1);
		
		/* ; */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				";", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ';', 1);
		
		/* , */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				",", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ',', 1);
		
		/* : */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				":", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ':', 1);
		
		/* :: */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				":", 1, {DFA::DFA_state(":", 1), DFA::DFA_state(tag_punctuation, 2, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, ':', 1).add_move_rule(1, ':', 2);
		
		/* . */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				".", 1, {DFA::DFA_state(tag_punctuation, 1, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '.', 1);
		
		/* .. */
        punctuation_dfas.push_back(std::make_shared<DFA>(DFA(
				".", 1, {DFA::DFA_state(".", 1), DFA::DFA_state(tag_punctuation, 2, true)}, DFA::DFA_state("", 0)
		)));
        punctuation_dfas[punctuation_dfas.size()-1]->add_move_rule(0, '.', 1).add_move_rule(1, '.', 2);
		
		/* const int */

		dfa_const_int_p = (std::make_shared<DFA>(DFA(
				"0123456789", 10, {DFA::DFA_state(tag_const_int, 1, true)}, DFA::DFA_state("", 0) 
		)));
		for(char c='0'; c<='9'; c++) {
			dfa_const_int_p->add_move_rule(0, c, 1);
			dfa_const_int_p->add_move_rule(1, c, 1);
		}
		
		/* digit.digit */
        dfa_const_real_p = (std::make_shared<DFA>(DFA(
				"0123456789.", 11, {DFA::DFA_state("0-9" , 1), DFA::DFA_state("0-9." , 2), DFA::DFA_state(tag_const_real , 3, true)}, DFA::DFA_state("", 0)
		)));
		for(char c='0'; c<='9'; c++) {
			dfa_const_real_p->add_move_rule(0, c, 1);
			dfa_const_real_p->add_move_rule(1, c, 1);
			dfa_const_real_p->add_move_rule(2, c, 3);
			dfa_const_real_p->add_move_rule(3, c, 3);
		}
		dfa_const_real_p->add_move_rule(1, '.', 2);
		dfa_const_real_p->add_move_rule(0, '.', 2);

        dfa_id_p = (std::make_shared<DFA>(DFA(
                string(all_letters+all_digits+"_"), {DFA::DFA_state(tag_id, 1, true)}, DFA::DFA_state("", 0)
        )));
        for(unsigned long i=0; i<all_letters.size(); i++)
            dfa_id_p->add_move_rule(0, all_letters.at(i), 1).add_move_rule(1, all_letters.at(i), 1);
        for(unsigned long i=0; i<all_digits.size(); i++)
            dfa_id_p->add_move_rule(1, all_digits.at(i), 1);
        dfa_id_p->add_move_rule(1, '_', 1);

        dfa_const_str_p = (std::make_shared<DFA>(DFA(
                all_characters, {DFA::DFA_state("\"", 1), DFA::DFA_state("\\", 2), DFA::DFA_state(tag_const_str, 3, true)}, DFA::DFA_state("", 0)
        )));
        dfa_const_str_p->add_move_rule(0, '\"', 1);
        for(unsigned long i=0; i<all_characters.size(); i++) {
            char ch = all_characters.at(i);

            if(ch != '\\' && ch != '\"')
                dfa_const_str_p->add_move_rule(1, ch, 1);
            dfa_const_str_p->add_move_rule(2, ch, 1);
        }
        dfa_const_str_p->add_move_rule(1, '\\', 2);
        dfa_const_str_p->add_move_rule(1, '\"', 3);

        /* white space */
        string ws_str = " \t\n\v\f\r";
        dfa_ws_p = (std::make_shared<DFA>(DFA(
                ws_str, {DFA::DFA_state(tag_ws, 1, true)}, DFA::DFA_state("", 0)
        )));
        for(int i=0; i<ws_str.size(); i++)
            dfa_ws_p->add_move_rule(0, ws_str.at(i), 1).add_move_rule(1, ws_str.at(i), 1);
		
		/* line comment */
		dfa_line_comment_p = (std::make_shared<DFA>(DFA(
                all_characters, {DFA::DFA_state("/", 1), DFA::DFA_state(tag_line_comment, 2, true)}, DFA::DFA_state("", 0)
		)));
        for(int i=0; i<=255; i++)
            if(i != '\n')
                dfa_line_comment_p->add_move_rule(2, i, 2);
		dfa_line_comment_p->add_move_rule(0, '/', 1);
		dfa_line_comment_p->add_move_rule(1, '/', 2);
		
		/* block comment */
		dfa_block_comment_p = (std::make_shared<DFA>(DFA(
                all_characters, {DFA::DFA_state("/", 1), DFA::DFA_state("*", 2), DFA::DFA_state("*", 3), DFA::DFA_state(tag_block_comment, 4, true)}, DFA::DFA_state("", 0)
		)));
		dfa_block_comment_p->add_move_rule(0, '/', 1);
		dfa_block_comment_p->add_move_rule(1, '*', 2);
		for(int i=0; i<=255; i++){
			if(i != '*')
				dfa_block_comment_p->add_move_rule(2, i, 2);
			if( i != '/' && i != '*')
				dfa_block_comment_p->add_move_rule(3, i, 2);
		}
		dfa_block_comment_p->add_move_rule(2, '*', 3);
		dfa_block_comment_p->add_move_rule(3, '*', 3);
		dfa_block_comment_p->add_move_rule(3, '/', 4);
	}

    int analyzer::alpha_yylex(void *ylval) {
        int token_type;
        do {
            token_type = __alpha_yylex(ylval);
        } while(token_type == WHITE_SPACE); /* discard white spaces */
        return token_type;
    }

    int analyzer::__alpha_yylex(void *ylval) {
        char ch;

        vector<shared_ptr<DFA>> active;
        active.insert(active.end(), keyword_dfas.begin(), keyword_dfas.end() );
        active.insert(active.end(), op_dfas.begin(), op_dfas.end() );
        active.insert(active.end(), punctuation_dfas.begin(), punctuation_dfas.end() );
        active.push_back(dfa_const_int_p);
        active.push_back(dfa_const_real_p);
        active.push_back(dfa_const_str_p);
        active.push_back(dfa_id_p);
        active.push_back(dfa_line_comment_p);
        active.push_back(dfa_block_comment_p);
         // TODO: nested comments?
        active.push_back(dfa_ws_p);

        /* Reset the DFAs */
        for(int i=0; i<active.size(); i++)
            active[i]->reset();

        /* Scan input until a matching token is found or EOF or something bad happens */
        int init_line = current_line;
        bool is_open_block_comment = false;
        bool is_open_str = false;
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

                        if(active[i] == dfa_block_comment_p && active[i]->generate_token() == "/*")
                            is_open_block_comment = true;
                        else if(active[i] == dfa_const_str_p && active[i]->generate_token() == "\"")
                            is_open_str = true;
                    } catch(DFA::DFA_state_error &error) {  /* Move failed because no rule exists */
                        active.erase(active.begin() + i);
                        i--;
                    }
                } else { /* No DFA can move. */
                    if(active[i]->current_state().is_final()) {
                        string cur_tag = active[i]->current_state().get_tag();
                        string token_content = active[i]->generate_token();
                        int ret_value;

                        if(cur_tag == tag_keyword) {
                            generator.gen_keyword_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(token_content);

                        } else if(cur_tag == tag_op) {
                            generator.gen_operator_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(token_content);

                        } else if(cur_tag == tag_const_int) {
                            generator.gen_const_int_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(tag_const_int);

                        } else if(cur_tag == tag_const_real) {
                            generator.gen_const_real_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(tag_const_real);

                        } else if(cur_tag == tag_const_str) {
                            generator.gen_const_str_token(init_line, token_content, ylval);
                            ret_value = tag_to_code.at(tag_const_str);

                        } else if(cur_tag == tag_punctuation) {
                            generator.gen_punctuation_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(token_content);

                        } else if(cur_tag == tag_id) {
                            generator.gen_identifier_token(current_line, token_content, ylval);
                            ret_value = tag_to_code.at(tag_id);

                        } else if(cur_tag == tag_line_comment) {
                            generator.gen_comment_token(current_line-1, "", "LINE_COMMENT", ylval);
                            ret_value = tag_to_code.at(tag_line_comment);

                        } else if(cur_tag == tag_block_comment) {
                            /* Calculate how many lines the comment spans */
                            /*
                            unsigned int start_line = current_line;
                            for(unsigned long i=0; i<token_content.size(); i++)
                                if(token_content.at(i) == '\n')
                                    start_line--;
                            */

                            generator.gen_comment_token(init_line, (to_string(init_line) + " - " + to_string(current_line)), "BLOCK_COMMENT", ylval);
                            ret_value = tag_to_code.at(tag_block_comment);

                        } else if(cur_tag == tag_ws) {
							/* noop. will be discarded. Just count how many lines we read */
                            /* unget() before the return statment will never put back a '\n' */
                            for(unsigned long i=0; i<token_content.size(); i++)
                                if(token_content.at(i) == '\n')
                                    current_line++;
                            ret_value = tag_to_code.at(tag_ws);
						} else {
                            assert(false); /* unreachable statement */
                        }
                        input.unget();
                        return ret_value;
                    }
                }
            }

            if(!exists_move) {    /* No DFA can move and none of them is in final state */
                /* Found unknown token */
                input.unget();
                throw runtime_error("unknown token: " + string(1, ch) + " (ASCII code: " + to_string(ch) + ")");
            }
        }

        if(!input.good()) {
            if(input.eof()) {
                if(is_open_block_comment)
                    throw runtime_error("EOF reached while block comment is open. Opened in line: " + to_string(init_line));
                if(is_open_str)
                    throw runtime_error("EOF reached while string is open. Opened in line: " + to_string(init_line));

                return EOF;
            } else {
                throw runtime_error("Something went wrong with the input file");
            }
        }
    }
}