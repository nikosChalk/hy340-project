//
// Created by nikos on 2/26/18.
//

#include "DFA.h"
#include <string>
#include <algorithm>

using namespace std;

namespace alpha_lex {

	DFA::DFA(char const alphabet[], size_t len,
		const std::vector<DFA_state> &states,
		const DFA_state& init) 
		: states(states), init_state(init), curr_state(...) {

		this->states = states;
		/*we check if init state is in the vector*/
		if (std::find(this->states.begin(), this->states.end(), init) != this->states.end()){
			this->init_state = init;
		}
		else{
			throw std::runtime_error("init state is not in vector with states");
		}
		/*we check if alphabet is null*/
		if (alphabet == NULL){
			throw std::runtime_error("init state is not in vector with states");
		}
		else{
			memcpy(this->alphabet, alphabet, len);
		}
		
		for (int i = 0; i < states.size(); i++){
			const DFA_state &state = states[i];
			this->available_moves[state] = std::map<char, DFA_state&>();
		}
	}

	DFA& DFA::add_move_rule(const DFA_state &current, char c, const DFA_state &next){

		std::map<const DFA_state&, std::map<char, DFA_state&>>::iterator it;
		const DFA_state temp = available_moves[current][c];
		it = available_moves.find(temp);
		if (it != available_moves.end()){
			throw std::runtime_error("rule exists in map");
		}
		else{
			available_moves[current][c] = next;
		}
	}

	DFA::DFA_state& DFA::move(char c){
		DFA_state& curr = current_state();
		this->curr_state = this->available_moves[curr][c];
		return this->available_moves[curr][c];
	}

	DFA::DFA_state::DFA_state(string tag, bool is_final){
		this->tag = tag;
		this->is_final_state = is_final;
	}

	bool DFA::DFA_state::is_final() const{
		if (this->is_final_state){
			return true;
		}
		else{
			return false;
		}
	}
}