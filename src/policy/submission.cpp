#include <iostream>
#include <fstream>

#include "../config.hpp"
#include "../state/state.hpp"
#include "./submission.hpp"

using namespace std;


int submission::Submission(State* node,int depth, int Alpha, int Beta, int myself){
    int val;
    if(! depth) return node->evaluate(myself);
    if(node->legal_actions.empty()) node->get_legal_actions();
    if(node->player == myself){
        val = -(1e9);
        for(auto it : node->legal_actions){
            State* next_move = node->next_state(it);
            val = max(val, Submission(next_move,depth-1, Alpha, Beta, myself));
            Alpha = max(Alpha, val);
            if(Alpha >= Beta) break;
        }
        return val;
    }
    else{
        val = 1e9;
        for(auto it:node->legal_actions){
            State* next_move = node->next_state(it);
            val = min(val, Submission(next_move, depth-1, Alpha, Beta, myself));
            Beta = min(Beta, val);
            if(Beta <= Alpha) break;
        }
        return val;
    }
}

Move submission::get_move(State *state, int depth){
  Move Best;
  int Max = -1e9;
  int tmp;
  if(!state->legal_actions.size())
    state-> get_legal_actions();
  for(auto it:state->legal_actions){
    State* next = state->next_state(it);
    tmp = Submission(next, depth-1, Max, 1e9, state->player);
    if(tmp > Max){
        Max = tmp;
        Best = it;
    }
  }
  return Best;
}