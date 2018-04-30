

#include <stdexcept>
#include <cassert>
#include "loop_handler.h"

using namespace std;
using namespace syntax_analyzer;

loop_handler::loop_handler() {
    loop_depth_stack = stack<unsigned int>();
    break_list_stack = stack<vector<unsigned int>>();
    continue_list_stack = stack<vector<unsigned int>>();

    loop_depth_stack.push(0);   //Initially we are at the global space (no function) and at depth 0 ==> not inside a loop.
}

unsigned int loop_handler::enter_loop() {
    unsigned int new_scope = loop_depth_stack.top() + 1;
    loop_depth_stack.pop();
    loop_depth_stack.push(new_scope);

    //Since loop depth increased, we need new break and continue lists
    break_list_stack.push(vector<unsigned int>()); //empty list
    continue_list_stack.push(vector<unsigned int>()); //empty list
    return new_scope;
}

unsigned int loop_handler::exit_loop() {
    if(!is_in_loop())
        throw logic_error("Cannot decrease loop scope since we are not in a loop");

    unsigned int new_scope = loop_depth_stack.top() - 1;
    loop_depth_stack.pop();
    loop_depth_stack.push(new_scope);

    //Since we exited that loop, it is safe to throw away the break/continue lists for that loop
    break_list_stack.pop();
    continue_list_stack.pop();
    return new_scope;
}

unsigned int loop_handler::get_current_depth() {
    assert(!loop_depth_stack.empty());  //should never be empty

    return loop_depth_stack.top();
}

void loop_handler::enter_function_ss() {
    loop_depth_stack.push(0);
}

void loop_handler::exit_function_ss() {
    if(loop_depth_stack.size() == 1)    // ==> we are in program space
        throw std::logic_error("Exit function scope space in loop handler called, when we are not in a function!");
    loop_depth_stack.pop();
}

vector<unsigned int> loop_handler::get_break_list() const {
    if(!is_in_loop())
        throw logic_error("get_break_list() called when not in loop");

    vector<unsigned int> copy = vector<unsigned int>(break_list_stack.top());
    return copy;
}

vector<unsigned int> loop_handler::get_continue_list() const {
    if(!is_in_loop())
        throw logic_error("get_continue_list() called when not in loop");

    vector<unsigned int> copy = vector<unsigned int>(continue_list_stack.top());
    return copy;
}

void loop_handler::append_to_break_list(unsigned int quadno) {
    if(!is_in_loop())
        throw logic_error("No break list to add the given quadno. Not in a loop");

    break_list_stack.top().push_back(quadno);
}

void loop_handler::append_to_continue_list(unsigned int quadno) {
    if(!is_in_loop())
        throw logic_error("No continue list to add the given quadno. Not in a loop");

    continue_list_stack.top().push_back(quadno);
}

bool loop_handler::is_in_loop() const {
    return (loop_depth_stack.top() > 0);
}