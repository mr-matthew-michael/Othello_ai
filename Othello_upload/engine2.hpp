#ifndef _ENGINE2_H
#define _ENGINE2_H
#include <stdlib.h>
#include <vector>
#include <bitset>
#include "move_engine.hpp"

class GameOperations
{
    public: 
        GameOperations();
        int get_p2_score();
        int get_p1_score();
        int check_move (int sq);
        uint8_t count_set_bits(uint64_t p1_board);
        bool get_end_game();
        int get_player();
        void set_player(int pl);
        int get_count(){return count;};
        vector<int> get_move_list();
        uint64_t current_move_board; 
        uint64_t get_p1_board(){return board1[count];};
        uint64_t get_p2_board(){return board2[count];};
        void undo_move();
        static FlipEngine flip_engine;
    private:
        void update_move_board();
        vector<int> __get_move_List(bool t);
        bool game_ended;
        int player;
        int count; 
        uint64_t board1[62];
        uint64_t board2[62];
        bitset<62> pass = 0;
        void flip(int move);
};

#endif