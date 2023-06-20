#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"


/**
 * @brief evaluate the state
 * 
 * @return int 
 */
int State::evaluate(int myself){
  // [TODO] design your own evaluation function
  const Board& cur = this->board;


  // Position weights
  static const int position_player[8][9][10]={
  {//兵
    {550,550,550,550,550},
    {400,450,450,450,400},
    {250,300,300,300,250},
    {200,200,200,200,-300},
    {150,150,150,150,150},
    {0,0,0,0,0},
  },
  { // 馬
    {250,250,250,250,250},
    {300,300,300,300,300},
    {350,300,350,300,350},
    {250,250,250,200,200},
    {200,200,200,200,200},
    {200,300,200,200,200},
  },
  { // bishop
    {0,350,0,350,0},
    {350,0,350,0,350},
    {0,300,0,300,0},
    {300,0,350,0,300},
    {0,300,0,300,0},
    {200,0,300,0,200},
  },
   { //rook
    {350,350,300,300,300},
    {350,350,300,300,300},
    {300,350,300,300,300},
    {350,350,300,300,300},
    {300,350,300,300,300},
    {500,400,300,200,200},
  },
  { //queen
    {500,300,300,300,500},
    {300,300,300,300,300},
    {300,300,300,300,300},
    {100,100,800,100,100},
    {100,200,400,200,100},
    {100,300,300,300,100},
  },
  { // king
  /*
    {200,200,200,200,200},
    {300,300,300,300,300},
    {400,400,400,400,400},
    {500,500,500,500,500},
    {700,600,500,600,700},
    {800,700,600,800,1000},
    */
   {100,100,100,100,100},
   {150,150,150,150,150},
    {200,200,200,200,200},
    {250,250,250,250,250},
    {350,300,250,300,350},
    {400,350,300,400,500},
  }
  };

  static const int position_enemy[8][9][10]={
  {
    {0,0,0,0,0},
    {150,150,150,150,150},
    {-300,200,200,200,200},
    {250,300,300,300,250},
    {400,450,450,450,400},
    {550,550,550,550,550},
  },
  {
    {200,200,200,300,200},
    {200,200,200,200,200},
    {200,200,250,250,250},
    {350,300,350,300,350},
    {300,300,300,300,300},
    {250,250,250,250,250},
  },
  {
    {200,0,300,0,200},
    {0,300,0,300,0},
    {300,0,350,0,300},
    {0,300,0,300,0},
    {350,0,350,0,350},
    {0,350,0,350,0},
  },
  {
    {200,200,300,400,500},
    {300,300,300,350,300},
    {300,300,300,350,350},
    {300,300,300,350,300},
    {300,300,300,350,350},
    {300,300,300,350,350},
  },
  {
    {100,300,300,300,100},
    {100,200,400,200,100},
    {100,100,800,100,100},
    {300,300,300,300,300},
    {300,300,300,300,300},
    {500,300,300,300,500},
  },
  {
    /*
    {1000,800,600,700,800},
    {700,600,500,600,700},
    {500,500,500,500,500},
    {400,400,400,400,400},
    {300,300,300,300,300},
    {200,200,200,200,200},
    */
   {500,400,300,350,400},
    {350,300,250,300,350},
    {250,250,250,250,250},
    {200,200,200,200,200},
    {150,150,150,150,150},
    {100,100,100,100,100},
  }
  };
  static const int value[10] = {0, 100, 300, 320, 500, 900, 5000};
  int myself_scores = 0;
  int enemy_scores = 0;

  for (int i = 0; i < BOARD_H; i++) {
    for (int j = 0; j < BOARD_W; j++) {
      int player_piece = cur.board[myself][i][j];
      int enemy_piece = cur.board[myself ^ 1][i][j];
  
      //enemy_scores += position_values_player[player_piece][i][j];
      //enemy_scores +=  position_values_enemy[enemy_piece][i][j];
      //myself_scores += value[player_piece];
      //enemy_scores +=  value[enemy_piece];
      myself_scores += value[player_piece] + position_player[player_piece][i][j];
      enemy_scores +=  value[enemy_piece] + position_enemy[enemy_piece][i][j];
    }
  }
  
  return myself_scores - enemy_scores;
 
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};


/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


const char piece_table[2][7][5] = {
  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
};
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}