#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bitset>
#include <vector>
#include "board_pp.hpp"
#include "engine2.hpp"
#include "move_engine.hpp"
extern "C"
{
    #include "moves.h"
    #include "user_input.h"
}

FlipEngine GameOperations::flip_engine = FlipEngine();

GameOperations::GameOperations() 
{
        board1[0] = 0x0000000810000000;
        board1[1] = 0x0000000810000000;
        board2[0] = 0x0000001008000000;
        board2[1] = 0x0000001008000000;
        player = 1; //1 for black, -1 for white
        count = 1;
        update_move_board();
        game_ended = false;
}

uint8_t GameOperations::count_set_bits(uint64_t p1_board)
{
    uint8_t count = 0;
    while (p1_board) {
        count += p1_board & 1;
        p1_board >>= 1;
    }
    return count;
}

int GameOperations::check_move(int sq) 
{
   if (game_ended) {
        return -2;
    } else if (current_move_board == 0) 
    {
        pass.set(count);
        player *= -1;
        update_move_board();
        return -1;
    } else if (sq == -1) 
    {
        pass.set(count);
        player *= -1;
        update_move_board();
        return -1;
    }
    else if (((1UL << sq) & current_move_board) != 0) 
    {
        flip(sq);
        player *= -1;
        count++;
        pass.reset(count);
        update_move_board();
        return sq;
    } else {
        return -3;
    }
}

void GameOperations::update_move_board() 
{
    if (player == 1) 
    {
        current_move_board = generate_moves(0, board1[count], board2[count]);
        if (current_move_board == 0 && generate_moves(0, board2[count], board1[count]) == 0)
            game_ended = true;
    } else 
    {
        current_move_board = generate_moves(0, board2[count], board1[count]);
        if (current_move_board == 0 && generate_moves(0, board1[count], board2[count]) == 0) 
            game_ended = true;
    }
}

void GameOperations::flip(int move) 
{
    if (player == 1) 
    {
        uint64_t flips = flip_engine.get_updated_board(board1[count], board2[count], move);
        board2[count + 1] = board2[count] & ~flips;
        board1[count + 1] = board1[count] | flips;
    } else 
    {
        uint64_t flips = flip_engine.get_updated_board(board2[count], board1[count], move);
        board1[count + 1] = board1[count] & ~flips;
        board2[count + 1] = board2[count] | flips;
    }
}

int GameOperations::get_p1_score()
{
    int p1_score = count_set_bits(board1[count]);
    int p2_score = count_set_bits(board2[count]);
    int empty = 64 - p2_score - p1_score;

    if(!game_ended)
        return p1_score;
    else if (p1_score > p2_score)
        return p1_score + empty;
    else if (p2_score == p1_score)
        return 32;

    return p1_score;
}

int GameOperations::get_p2_score()
{
    int p1_score = count_set_bits(board1[count]);
    int p2_score = count_set_bits(board2[count]);
    int empty = 64 - p2_score - p1_score;
    
    if(!game_ended)
        return p2_score;
    else if (p2_score > p1_score)
        return p2_score + empty;
    else if (p2_score == p1_score)
        return 32;

    return p2_score;
}

void GameOperations::undo_move() 
{
    if (count <= 1) return;

    player *= -1;

    if(!pass[count]) 
        count--;
    else 
        pass.reset(count);

    game_ended = false;
    update_move_board();
}

int GameOperations::get_player() 
{
    return player;
}

void GameOperations::set_player(int pl) 
{
    player = pl;
}

bool GameOperations::get_end_game() 
{
    return game_ended;
}

vector<int> GameOperations::get_move_list() 
{
    return flip_engine.get_bit_list(current_move_board);
}