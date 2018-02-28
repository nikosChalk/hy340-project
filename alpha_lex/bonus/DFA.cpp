#include "DFA.h"
#include <string>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iostream>

using namespace std;

namespace alpha_lex {

	DFA::DFA(char const alphabet[], size_t len, const std::vector<DFA_state> &states, const DFA_state &init) {

        /*we check if alphabet is null*/
        if (alphabet == NULL)
            throw invalid_argument("alphabet must not be NULL");

		/*we check if init state is in the vector*/
		if (find(states.begin(), states.end(), init) != states.end())
			throw invalid_argument("init state must not be in vector with states");

        for(int i=0; i<states.size(); i++)
            for(int j=i+1; j<states.size(); j++)
                if(states[i].get_id() == states[j].get_id())
                    throw std::runtime_error("State in index " + to_string(i) + " and index " + to_string(j) +
                                             " have the same id: " + to_string(states[i].get_id()));

        this->states = vector<DFA_state>(states.begin(), states.end());
        this->states.emplace_back(init);

        init_state = &(this->states[this->states.size()-1]);
        curr_state = init_state;
        alphabet_len = len;
        token = string();
        memcpy(this->alphabet, alphabet, len);

        available_moves = map<DFA_state*, std::map<char, DFA_state*>>();
		for(int i = 0; i < states.size(); i++) {
            available_moves[&this->states[i]] = map<char, DFA_state*>();
        }
	}

    DFA::DFA(std::string alphabet, const std::vector<DFA_state> &states, const DFA_state &init)
        : DFA(alphabet.c_str(), alphabet.size(), states, init) { /* Empty Body */ }

	DFA& DFA::add_move_rule(const DFA_state &current, char c, const DFA_state &next) {
		if(has_move_rule(current, c))
			throw invalid_argument("rule already exists in map");
        /*TODO: check if we have these states in our list */
        bool exists = false;
        for(int i=0; i<alphabet_len; i++) {
            if(alphabet[i] == c) {
                exists = true;
                break;
            }
        }
        if(!exists)
            throw invalid_argument("DFA does not recognize the given character for adding a new move rule");

        DFA_state &t1 = const_cast<DFA_state&>(current);
        DFA_state &t2 = const_cast<DFA_state&>(next);
        available_moves[&t1][c] = &t2;
        return *this;
	}

    DFA& DFA::add_move_rule(unsigned int current_id, char c, unsigned int next_id) {
        DFA_state *ptr_cur = nullptr;
        DFA_state *ptr_next = nullptr;

        for(int i=0; i<states.size(); i++) {
            if(states[i].get_id() == current_id)
                ptr_cur = &states[i];
            if(states[i].get_id() == next_id)
                ptr_next = &states[i];
        }

        if(ptr_cur == nullptr || ptr_next == nullptr)
            throw std::invalid_argument("Invalid current_id or next_id");
        return add_move_rule(*ptr_cur, c, *ptr_next);
    }

    bool DFA::has_move_rule(const DFA_state &s, char c) const {
        DFA_state &tmp = const_cast<DFA_state&>(s);

        if(available_moves.find(&tmp) != available_moves.end())
            if(available_moves.at(&tmp).find(c) != available_moves.at(&tmp).end())
                return true;
        return false;
    }

	DFA::DFA_state& DFA::move(char c) {
        if(!has_move_rule(current_state(), c))
            throw DFA_state_error("No such move exists from current state");

        DFA_state &tmp = current_state();
        curr_state = available_moves[&tmp][c];
        token = token + c;
		return current_state();
	}

    DFA::DFA_state& DFA::current_state() const {
        DFA_state &ref = *curr_state;
        return *curr_state;
    }

    void DFA::reset() {
        curr_state = init_state;
        token = string();
    }

    std::string DFA::generate_token() const {
        return token;
    }





    DFA::DFA_state::DFA_state() {
        const void *addr = static_cast<const void*>(this);
        stringstream ss;

        ss << addr;
        tag = ss.str();
        id = 0;
        is_final_state = false;
    }

    DFA::DFA_state::DFA_state(const std::string &tag, unsigned int id, bool is_final /*=false*/){
        this->tag = tag;
        this->id = id;
        this->is_final_state = is_final;
    }

    DFA::DFA_state::DFA_state(const DFA_state &original) {
        tag = original.get_tag();
        id = original.get_id();
        is_final_state = original.is_final_state;
    }

    bool DFA::DFA_state::cmpr_by_id::operator()(const DFA_state &a, const DFA_state &b) const {
        return ((a.get_id() - b.get_id()) == 0);
    }
    bool DFA::DFA_state::operator==(const DFA_state &other) const {
        return this->get_id() == other.get_id();
    }

    std::string DFA::DFA_state::get_tag() const {
        return tag;
    }

    unsigned int DFA::DFA_state::get_id() const {
        return id;
    }

	bool DFA::DFA_state::is_final() const {
        return is_final_state;
	}





    DFA::DFA_state_error::DFA_state_error(const std::string &msg)
        : runtime_error(msg) { /* empty body */ }
}