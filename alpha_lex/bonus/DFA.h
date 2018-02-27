//
// Created by nikos on 2/26/18.
//

#ifndef HY340_PROJECT_DFA_H
#define HY340_PROJECT_DFA_H

#include <cstdlib>
#include <vector>
#include <map>

namespace alpha_lex {

    /**
     * Deterministic Finite Automaton including a "hang" state
     */
    class DFA {

    public:
        class DFA_state;    /* Forward declaration */

        /**
         * Creates a DFA with the given alphabet and states. In addition to the given states,
         * the DFA has a non-final "hang" state, in which no matter-what the input it stays there.
         * The DFA goes in the "hang" state if it meets a character for which it has no rule defined from its current state.
         * @param alphabet The alphabet of the DFA. This is a character sequence, NOT a string. A null-terminating character
         * is considered part of the alphabet.
         * @param len The length of the alphabet. (If the null-terminating character is within alphabet, then it should
         * be included in the calculation of len)
         * @param states The DFA's states
         * @param init The initial state of the DFA. Must be within states.
         * @throws std::invalid_argument if init is not within states
         * @throws std::invalid_argument if alphabet is NULL
         */
        DFA(char const alphabet[], size_t len,
            const std::vector<DFA_state> &states,
            const DFA_state& init);

        /**
         * Adds a move rule for the DFA. The rule states that, if from the current state you read character c then go to
         * the next state.
         * @param current The current state from which the transition initiates
         * @param c The character which triggers the transition
         * @param next The resulting state
         * @return *this
         * @throws std::invalid_argument if a rule from the state current with character c to some other state
         * already exists
         */
        DFA& add_move_rule(const DFA_state &current, char c, const DFA_state &next);

        /**
         * Moves the DFA from its current state by reading the character c.
         * @param c The character to read
         * @return The resulting state
         */
        DFA_state& move(char c);

        /**
         * Returns the current state of the DFA
         * @return The DFA's current state
         */
        DFA_state& current_state() const;

        /**
         * Checks if the DFA is hanging
         * @return True if the DFA is in the "hang" state, as described in the constructor. False otherwise.
         */
        bool has_hanged() const;

        /**
         * Resets the DFA to its initial state.
         */
        void reset();


        /**
         * Represents a state of a DFA
         */
        class DFA_state {

        public:
            /**
             * Creates a new state.
             * @param tag A name for this state. Note that the name is NOT used for comparison of DFA_state objects
             * @param is_final True if the constructed state should be final. False otherwise.
             */
            DFA_state(std::string tag, bool is_final);

            /**
             * Checks if this state is a final state
             * @return True if this state is final. False otherwise.
             */
            bool is_final() const;

        private:
            std::string tag;
            bool is_final_state;
        };


    private:
        char alphabet[];
        size_t alphabet_len;
        
        std::vector<DFA_state> states;
        DFA_state& init_state;

        /**
         * Maps each state A to a map of move actions. Each map of move actions maps a character c to a state B.
         * This mapping indicates that from state A we can transition to state B if we read character c.
         * This map is constructed through invocations of add_move_rule()
         * (state --> map of <character, next_state>)
         */
        std::map<DFA_state&, std::map<char, DFA_state&>> available_moves;

    };


}


#endif //HY340_PROJECT_DFA_H
