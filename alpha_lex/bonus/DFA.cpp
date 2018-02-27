//
// Created by nikos on 2/26/18.
//

#include "DFA.h"
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

namespace alpha_lex {

	DFA::DFA(char const alphabet[], size_t len,
		const std::vector<DFA_state> &states,
		const DFA_state& init) 
		: states(states.begin(), states.end()), init_state(init), curr_state(make_shared(init)) {

        /*we check if alphabet is null*/
        if (alphabet == NULL)
            throw invalid_argument("alphabet must not be NULL");

		/*we check if init state is in the vector*/
		if (find(this->states.begin(), this->states.end(), init) == this->states.end())
			throw invalid_argument("init state is not in vector with states");

        alphabet_len = len;
        token = string();
        memcpy(this->alphabet, alphabet, len);
		
		for(int i = 0; i < states.size(); i++)
			this->available_moves[this->states[i]] = map<char, const DFA_state&>();
	}

    bool DFA::has_move_rule(const DFA_state &s, char c) const {
        /*TODO: implement */

        /*
		map<const DFA_state&, std::map<char, const DFA_state&>>::iterator it;

		it = available_moves.find(available_moves[current][c]);
		if(it != available_moves.end())
         */
        throw runtime_error("Not Implemented!");
    }

	DFA& DFA::add_move_rule(const DFA_state &current, char c, const DFA_state &next) {
		if(has_move_rule(current, c))
			throw invalid_argument("rule already exists in map");

        available_moves[current][c] = next;
        return *this;
	}

	DFA::DFA_state& DFA::move(char c) {

        if(!has_move_rule(current_state(), c))
            throw DFA_state_error("No such move exists from current state");

        /*TODO: to be fixed bellow line */
        //curr_state = available_moves[current_state()][c];
        token = token + c;
        /*TODO: fix bellow line to work */
		//return curr_state;
	}

	DFA::DFA_state::DFA_state(const std::string &tag, bool is_final /*=false*/){
		this->tag = tag;
		this->is_final_state = is_final;
	}

	bool DFA::DFA_state::is_final() const {
        return is_final_state;
	}
}