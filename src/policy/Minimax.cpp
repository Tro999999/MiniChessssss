
#include <iostream>
#include <fstream>

#include "../config.hpp"
#include "../state/state.hpp"
#include "./Minimax.hpp"
using namespace std;


int Minimax::minimax(State* node,int depth, int myself){
    int val;
    if(! depth) return node->evaluate(myself);
    if(node->legal_actions.empty()) node->get_legal_actions();
    if(node->player == myself){
        val = -1e9;
        for(auto it : node->legal_actions){
            State* next_move = node->next_state(it);
            val = max(val, minimax(next_move, depth-1, myself));
        }
    }
    else{
        val = 1e9;
        for(auto it : node->legal_actions){
            State* next_move = node->next_state(it);
            val = min(val, minimax(next_move, depth-1, myself));
        }
    }
    return val;
}

Move Minimax::get_move(State *state, int depth){
  Move Best;
  int Max = - 1e9;
  int t;
  if(!state->legal_actions.size())
    state-> get_legal_actions();
  for(auto it:state->legal_actions){
    State* next = state->next_state(it);
    t = minimax(next, depth-1, state->player);
    if(t > Max){
        Max = t;
        Best = it;
    }
  }
  return Best;
}


