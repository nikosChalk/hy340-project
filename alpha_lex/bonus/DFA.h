//
// Created by nikos on 2/26/18.
//

#ifndef HY340_PROJECT_DFA_H
#define HY340_PROJECT_DFA_H

#include <cstdlib>
#include <vector>
#include <map>
#include <memory>
#include "../alpha_token_generator.h"

namespace alpha_lex {

    /**
     * Deterministic Finite Automaton
     */
    class DFA {

    public:
        class DFA_state;    /* Forward declaration */
        class DFA_state_error;  /* Forward declaration */

        /**
         * Creates a DFA with the given alphabet and states.
         * @param alphabet The alphabet of the DFA. This is a character sequence, NOT a string. A null-terminating character
         * is considered part of the alphabet.
         * @param len The length of the alphabet. (If the null-terminating character is within alphabet, then it should
         * be included in the calculation of len)
         * @param states The DFA's states, EXCLUDING its initial state
         * @param init The initial state of the DFA. Must NOT be within states.
         * @throws std::invalid_argument if init is within states
         * @throws std::invalid_argument if alphabet is NULL
         * @throws std::invalid_argument if any of the DFA_states withing states vector, have the same id.
         */
        DFA(char const alphabet[], size_t len,
            const std::vector<DFA_state> &states,
            const DFA_state& init);

        DFA(std::string alphabet, const std::vector<DFA_state> &states, const DFA_state& init);

        /**
         * Adds a move rule for the DFA. The rule states that, if from the current state you read character c then go to
         * the next state.
         * @param current The current state from which the transition initiates
         * @param c The character which triggers the transition
         * @param next The resulting state
         * @return *this
         * @throws std::invalid_argument if a rule from the state current with character c to some other state
         * already exists
         * @throws std::invalid_argument if c is not in the DFA's alphabet
         */
        DFA& add_move_rule(const DFA_state &current, char c, const DFA_state &next);

        /**
         * Adds a move rule for the DFA. The rule states that, if from the current state you read character c then go to
         * the next state.
         * @param current The id of the current DFA_state from which the transition initiates
         * @param c The character which triggers the transition
         * @param next The id of the resulting DFA_state
         * @return *this
         * @throws std::invalid_argument if a rule from the DFA_state with current_id, with character c to some other state
         * already exists
         * @throws std::invalid_argument if c is not in the DFA's alphabet
         * @throws std::invalid_argument if current_id or next_id does not belong to any DFA_state
         */
        DFA& add_move_rule(unsigned int current_id, char c, unsigned int next_id);

        /**
         * Checks if this DFA has a move rule from the state s with character c
         * @param s The state to check
         * @param c The character to check
         * @return True if the DFA has a move rule from state s with character c to some other state. False otherwise.
         */
        bool has_move_rule(const DFA_state &s, char c) const;

        /**
         * Moves the DFA from its current state by reading the character c.
         * @param c The character to read
         * @return The resulting state
         * @throws alpha_lex::DFA::DFA_state_error if the DFA does not contain a rule for transitioning from its current state
         * to another state with character c
         */
        DFA_state& move(char c);

        /**
         * Returns the current state of the DFA
         * @return The DFA's current state
         */
        DFA_state& current_state() const;

        /**
         * Resets the DFA to its initial state.
         */
        void reset();

        /**
         * Returns the token that this DFA has identified up to its current state through move actions.
         * Note that calls to reset(), also reset this token to an empty string
         * @return The token that this DFA has identified up until now
         */
        std::string generate_token() const;

        /**
         * Represents a state of a DFA
         */
        class DFA_state {

        public:

            /**
             * Comparator that can be used to compare two DFA_states with their id.
             */
            struct cmpr_by_id {
                /**
                 * Compares two DFA_states through their id
                 * @return True if a.id == b.id. False otherwise
                 */
                bool operator()(const DFA_state &a, const DFA_state &b) const;
            };

            /**
             * Checks if two DFA_states are equal. Comparision is done based on the id of the two states
             * @param other The other state to compare
             * @return True if other.id == this->id. False otherwise
             */
            bool operator==(const DFA_state& other) const;



            /**
             * Creates a new non-final state with a random tag and 0 id
             */
            DFA_state();

            /**
             * Creates a new state.
             * @param tag A name for this state. Note that the name is NOT used for comparison of DFA_state objects
             * @param An id for this state.
             * @param is_final True if the constructed state should be final. False otherwise.
             */
            DFA_state(const std::string &tag, unsigned int id, bool is_final = false);

            /**
             * Copy constructor. Creates a DFA state with the same attributes as original
             * @param original The DFA_state to copy
             */
            DFA_state(const DFA_state& original);

            /**
             * Retunrs the tag of this DFA state
             * @return The tag of this DFA state
             */
            std::string get_tag() const;

            unsigned int get_id() const;

            /**
             * Checks if this state is a final state
             * @return True if this state is final. False otherwise.
             */
            bool is_final() const;

        private:
            std::string tag;
            unsigned int id;
            bool is_final_state;
        };

        /**
         * Represents a state error that might occur within a DFA. (e.g. non-existing move)
         */
        class DFA_state_error: public std::runtime_error {

        public:
            explicit DFA_state_error(const std::string &msg);
        };


    private:
        char alphabet[256];
        size_t alphabet_len;
        
        std::vector<DFA_state> states;
        DFA_state *init_state;
        DFA_state *curr_state;
        std::string token;

        /**
         * Maps each state A to a map of move actions. Each map of move actions maps a character c to a state B.
         * This mapping indicates that from state A we can transition to state B if we read character c.
         * This map is constructed through invocations of add_move_rule()
         * (state --> map of <character, next_state>)
         */
        std::map<DFA_state*, std::map<char, DFA_state*>> available_moves;
    };


}


#endif //HY340_PROJECT_DFA_H
