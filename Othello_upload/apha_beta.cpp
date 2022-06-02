#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <limits>
#include <vector>
#include <iostream>
#include "board_pp.hpp"
#include "engine2.hpp"

extern "C" {
#include "moves.h"
#include "user_input.h"
}

int position = 0;

uint64_t highestPowerof2(uint64_t x)
{
	// check for the set bits
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
    x |= x >> 32;
    x |= x >> 63;

	return x ^ (x >> 1);
}

int getNoBits(uint64_t board) 
{
    int bits = 0;
    while (board != 0){
        board = board - highestPowerof2(board);
        bits++;
    }
    return bits;
}

int corners(uint64_t board) 
{
    uint64_t corners = board & 0x8100000000000081;
    return getNoBits(corners);
}

std::vector<int> getBitList(uint64_t move_board) 
{
    std::vector<int> move_indices;
     while (move_board != 0){
        uint64_t current_move = highestPowerof2(move_board);
        move_board = move_board - current_move;
        move_indices.push_back(log2(current_move));
    } 
    return move_indices;
}

std::vector<int> getBlackList(uint64_t p1)
{
    return getBitList(p1);
}

std::vector<int>getWhiteList(uint64_t p1)
{
    return getBitList(p1);
}

int stone_eval (uint64_t p1, uint64_t p2) 
{
    return getBlackList(p1).size() - getWhiteList(p2).size();
}

std::vector<int> get_move_list(uint64_t player, uint64_t opponent) 
{
    return getBitList(generate_moves(0, player, opponent));
}

int move_eval(uint64_t p1, uint64_t p2)
{
    int black = getNoBits(generate_moves(0,p1, p2));
    //printf("\nblack_moves: [%d] \n", black);
    int white = getNoBits(generate_moves(0,p2,p1));
    //printf("\nwhite_moves: [%d] \n", white);
    return black - white;
}

int points_eval(uint64_t p1, uint64_t p2)
{
    int score = 0;
    score += stone_eval(p1, p2);
    score += 10*move_eval(p1, p2);
    score += 1000*(corners(corners(p1) - corners(p2)));

    return score;
}

int alpha_beta(GameOperations* engine, int alpha, int beta, int evaluation, int depth)
{
    if (depth == 0)
    {
        position++;
        return points_eval(engine->get_p1_board(), engine->get_p2_board());
    } else if (engine->get_end_game()) {
        if (engine->get_p1_score() == 32) {
            return 0;
        } else {}
            return engine->get_p1_score() > 32 ? 10000 : -10000;
    }

    std::vector<int> moves = engine->get_move_list();

    if (moves.size() == 0)
    {
        engine->check_move(-1);
        int eval = alpha_beta(engine, evaluation, alpha, beta, depth - 1);
        engine->undo_move();
        return eval;
    } else if (engine->get_player() == 1) { //maximising player
        int eval = numeric_limits<int>::min();
        for (int i : moves) {
            engine->check_move(i);
            eval = max(eval, alpha_beta(engine, evaluation, alpha, beta, depth - 1));
            engine->undo_move();
            alpha = max(alpha, eval);
            if (beta<=alpha) break;
        }
        return eval;
    } else 
    { //minimising player
        int eval = numeric_limits<int>::max();
        for (int i : moves) {
            engine->check_move(i);
            eval = min(eval, alpha_beta(engine, evaluation, alpha, beta, depth - 1));
            engine->undo_move();
            beta = min(beta, eval); 
            if (beta<=alpha) break;
        }
        return eval;
    }
}

int alpha_beta_search(GameOperations* engine, int evaluation, int depth)
{
    position = 0;
    std::vector<int> moves = engine->get_move_list();

    if (moves.size() == 0) 
        return -1;

    if (engine->get_player() == 1) 
    {
        int max_eval = numeric_limits<int>::min();
        int best_move = -1;
        for (int i : moves) {
            engine->check_move(i);
            int eval = alpha_beta(engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
            if (max_eval < eval) {
                max_eval = eval;
                best_move = i;
            }
            engine->undo_move();
        }
        return best_move;
    } else
    {
        int min_eval = numeric_limits<int>::max();
        int best_move = -1;
        for (int i : moves) {
            engine->check_move(i);
            int eval = alpha_beta(engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
            if (min_eval > eval) {
                min_eval = eval;
                best_move = i;
            }
            engine->undo_move();
        }
        return best_move;
    }
}

void game_loop_justBot(GameOperations* engine)
{
    while(!engine->get_end_game())
    {
        std::vector<int> moves2 =  engine->get_move_list(); 
        int player = engine->get_player();
        printf("C *** B: [%d] || W: [%d] ***\n", engine->get_p1_score(), engine->get_p2_score());
        engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());
        int other_player_move = alpha_beta_search(engine, points_eval(engine->get_p1_board(), engine->get_p2_board()), 7);
        engine->check_move(other_player_move);
        print_bot_choice(other_player_move, player);
        if (player == 1)
        {
            int other_player_move = alpha_beta_search(engine, points_eval(engine->get_p1_board(), engine->get_p2_board()), 7);
            engine->check_move(other_player_move);
            print_bot_choice(other_player_move, player);
        }else 
        {
            if (moves2.size() == 0)
            {
                engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());
                engine->check_move(-1);
            }
            else
                engine->check_move(moves2[0]);
        }
    }
    
    printf("\n*** B: [%d] || W: [%d] ***", engine->get_p1_score(), engine->get_p2_score());
    engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());
}

void player_vs_bot(GameOperations* engine)
{
    int pl = engine->get_player();
    int pl_choice = start_game(pl);
    //engine->set_player(pl_choice);

    while(!engine->get_end_game())
    {
        int player = engine->get_player();
        printf("C*** B: [%d] || W: [%d] ***\n", engine->get_p1_score(), engine->get_p2_score());
        fflush(stdout);

        engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());

        if (engine->get_player() != pl_choice)
        {
            int other_player_move = alpha_beta_search(engine, points_eval(engine->get_p1_board(), engine->get_p2_board()), 7);
            print_bot_choice(other_player_move, player);
            engine->check_move(other_player_move);
        }
        else 
        {
            int square = player_1_turn_prompt(player); 
            engine->check_move(square);
        }
        fflush(stdout);
    }
    
    printf("\nC *** B: [%d] || W: [%d] ***", engine->get_p1_score(), engine->get_p2_score());
    fflush(stdout);
    engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());
}

void bot_vs_drMecBot(GameOperations* engine)
{
    int pl = engine->get_player();
    int pl_choice = start_game(pl);
    //engine->set_player(pl_choice);

    while(!engine->get_end_game())
    {
        int player = engine->get_player();
        printf("C*** B: [%d] || W: [%d] ***\n", engine->get_p1_score(), engine->get_p2_score());

        engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());

        if (engine->get_player() == pl_choice)
        {
            int other_player_move = alpha_beta_search(engine, points_eval(engine->get_p1_board(), engine->get_p2_board()), 7);
            print_bot_choice(other_player_move, player);
            engine->check_move(other_player_move);
        }
        else 
        {
            int square = player_1_turn_prompt(player); 
            engine->check_move(square);
        }
    }
    
    printf("\nC *** B: [%d] || W: [%d] ***", engine->get_p1_score(), engine->get_p2_score());
    engine->flip_engine.print_board(engine->get_p1_board(), engine->get_p2_board());
}

int main(int argc, char *argv[])
{ 
    GameOperations engine = GameOperations();
    player_vs_bot(&engine);
}
