#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
//#include <bit>
#include <iostream>
#include "board_pp.hpp"
#include "move_engine.hpp"

using namespace std;

FlipEngine::FlipEngine() {generate_flip_table();}

uint64_t b_floor(uint64_t x)
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

void FlipEngine::print_board(uint64_t player_1, uint64_t player_2)
{
   // printf("\n");
    printf("C      a b c d e f g h\n");

    for (int rank = 0; rank < 8; rank++)
    {
        printf("C ");
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            
            if (!file)
                printf("  %d ", rank+1);
            if (get_bit(player_1, square)) 
                printf(" B");    
            else if (get_bit(player_2, square)) 
                printf(" W");  
            else if (!get_bit(player_1, square) && !get_bit(player_2, square))
                printf(" -");
        }
        printf("\n");
    }
    printf("C player_1: %lu \nC player_2: %lu\n", player_1, player_2);
}

uint64_t FlipEngine::get_bit(uint64_t bitboard, int square)
{
    uint64_t b_square = 1ULL << square;

    return bitboard &= b_square;
} 

int get_x_position(int square)
{
    int x_position = (63-square) % 8;
    return x_position;
}

int get_x_position2(int square)
{
    int x_position = (square) % 8;
    return x_position;
}

int get_y_position(int square)
{
    int y_position = (63-square) / 8;
    return y_position;
}

int get_diag1_position(int square)
{
    int diag1 = 5 + get_y_position(square) - get_x_position2(square);
    return diag1;
}

int get_diag2_position(int square)
{
    int diag2 = -2 + get_y_position(square) + get_x_position2(square);
    return diag2;
}

uint64_t FlipEngine::flip_table_location(int p1, int p2, int sq) 
{
    return (p1&0xff) << 16 | (p2&0xff) << 8 | (sq&0xff);
}

void FlipEngine::compute_flip(uint8_t player_1, uint8_t player_2) 
{
    bitset<8> p1 = player_1;
    bitset<8> p2 = player_2;
    bitset<8> flipped_stones = 0;

    for (int i = 0; i < 8; i++) 
    {
        if (p1[i] == 1 and p1[i+1] == 0 and p2[i+1] == 1) 
        {
            flipped_stones.reset();
            for (int j = i + 1; j < 8; j++) 
            {
                flipped_stones[j] = 1;
                if (p2[j] == 0) 
                {
                    flipped_stones[j] = 0;
                    flip_map[flip_table_location(p1.to_ulong(), p2.to_ulong(), (0b1 << j))] |= flipped_stones.to_ulong();
                    break;
                }
            }         
        }
    }
    for (int i = 7; i > 0; i--) 
    {
        if (p1[i] == 1 and p1[i-1] == 0 and p2[i-1] == 1) 
        {
            flipped_stones.reset();
            for (int j = i - 1; j >= 0; j--) 
            {
                flipped_stones[j] = 1;
                if (p2[j] == 0) 
                {
                    flipped_stones[j] = 0;
                    flip_map[flip_table_location(p1.to_ulong(), p2.to_ulong(), (0b1 << j))] |= flipped_stones.to_ulong();
                    break;
                }
            }           
        }
    }
}

void FlipEngine::generate_flip_table() 
{
    for (int i = 0; i < 256; i++) 
    {
        for (int j = 0; j < 256; j++) 
        {
            if ((i & j) == 0) 
               compute_flip(i,j);
        }
    }
}

uint64_t FlipEngine::get_updated_board(uint64_t player, uint64_t opponent, int move)
{
    int xy[2];
    xy[0] = get_x_position(move);
    xy[1] = get_y_position(move);

    int diag[2];
    diag[1] = get_diag1_position(move);
    diag[0] = get_diag2_position(move);

    uint64_t move_board = (uint64_t)0b1 << move;
    uint64_t flip_board  = 
                ( set_row(0, flip_map[flip_table_location(get_row(player, xy[1]), get_row(opponent, xy[1]), 
                    get_row(move_board, xy[1]))], xy[1])

                | set_column(0, flip_map[flip_table_location(get_column(player, xy[0]), 
                    get_column(opponent, xy[0]), get_column(move_board,xy[0]))], xy[0])

                | set_diagonal(0, flip_map[flip_table_location(get_diagonal(player, diag[0]), 
                    get_diagonal(opponent, diag[0]), get_diagonal(move_board,diag[0]))], diag[0])

                | set_anti_diagonal(0, flip_map[flip_table_location(get_anti_diagonal(player, diag[1]), 
                    get_anti_diagonal(opponent, diag[1]), get_anti_diagonal(move_board,diag[1]))], diag[1])
                );
    if (flip_board == 0) 
        return flip_board;
    else 
        return flip_board | move_board;
}

vector<int> FlipEngine::get_bit_list(uint64_t move_board) 
{
    vector<int> move_indices;
    while (move_board != 0)
    {
        uint64_t current_move = b_floor(move_board);
        move_board = move_board - current_move;
        move_indices.push_back(log2(current_move));
    } 
    return move_indices;
}