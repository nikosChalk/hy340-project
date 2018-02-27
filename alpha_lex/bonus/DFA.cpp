//
// Created by nikos on 2/26/18.
//

#include "DFA.h"
#include <string>
#include <algorithm>
#include <cstring>
#include <sstream>

using namespace std;

namespace alpha_lex {

	DFA::DFA(char const alphabet[], size_t len, const std::vector<DFA_state> &states, const DFA_state &init) {

        /*we check if alphabet is null*/
        if (alphabet == NULL)
            throw invalid_argument("alphabet must not be NULL");

		/*we check if init state is in the vector*/
		if (find(states.begin(), states.end(), init) != states.end())
			throw invalid_argument("init state is must not be in vector with states");

        this->states = vector(states.begin(), states.end());
        this->states.emplace_back(init);

        init_state = &(this->states[this->states.size()-1]);
        curr_state = init_state;
        alphabet_len = len;
        token = string();
        memcpy(this->alphabet, alphabet, len);

        available_moves = map();
		for(int i = 0; i < states.size(); i++)
			available_moves[this->states[i]] = map();
	}

	DFA& DFA::add_move_rule(const DFA_state &current, char c, const DFA_state &next) {
		if(has_move_rule(current, c))
			throw invalid_argument("rule already exists in map");

        bool exists = false;
        for(int i=0; i<alphabet_len; i++) {
            if(alphabet[i] == c) {
                exists = true;
                break;
            }
        }
        if(!exists)
            throw invalid_argument("DFA does not recognize the given character for adding a new move rule");

        available_moves[const_cast<DFA_state&>(current)][c] = next;
        return *this;
	}

    bool DFA::has_move_rule(const DFA_state &s, char c) const {
        DFA_state &tmp = const_cast<DFA_state&>(s);

        if(available_moves.find(tmp) != available_moves.end())
            if(available_moves[tmp].find(c) != available_moves[tmp].end())
                return true;
        return false;
    }

	DFA::DFA_state& DFA::move(char c) {
        if(!has_move_rule(current_state(), c))
            throw DFA_state_error("No such move exists from current state");

        curr_state = &available_moves[current_state()][c];
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
        is_final_state = false;
    }

    DFA::DFA_state::DFA_state(const std::string &tag, bool is_final /*=false*/){
        this->tag = tag;
        this->is_final_state = is_final;
    }

    DFA::DFA_state::DFA_state(const DFA_state &original) {
        tag = original.get_tag();
        is_final_state = original.is_final_state;
    }

    std::string DFA::DFA_state::get_tag() const {
        return tag;
    }

	bool DFA::DFA_state::is_final() const {
        return is_final_state;
	}





    DFA::DFA_state_error::DFA_state_error(const std::string &msg)
        : runtime_error(msg) { /* empty body */ }
}