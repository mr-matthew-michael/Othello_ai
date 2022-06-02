#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <stdio.h>
#include <stdint.h>
#include "board_pp.hpp"

using namespace std;

uint64_t set_bit(uint64_t bitboard, int square) 
{
    uint64_t b_square = 1ULL << square;
    return bitboard |= b_square; 
}

uint64_t flip_vertical(uint64_t bitboard) 
{
    uint64_t new_bb = 0ULL;
    new_bb = (bitboard << vertical_shift[0]);
    
    for (int i = 0; i < 3; i++)
        new_bb |= ((bitboard << vertical_shift[i+1]) & vertical_masks[i]);

    for (int i = 3; i < 7; i++)
        new_bb |= ((bitboard >> vertical_shift[i+1]) & vertical_masks[i]);

    new_bb |= (bitboard >> vertical_shift[7]);

    return new_bb;
}

uint64_t flip_horizontal(uint64_t bitboard) 
{
    uint64_t new_bb = 0ULL;
    for (int i = 0; i < 4; i++)
        new_bb |= ((bitboard << horizontal_shift[i]) & horizontal_masks[i]);

    for (int i = 4; i < 8; i++)
        new_bb |= ((bitboard >> horizontal_shift[i]) & horizontal_masks[i]);

    return new_bb;
}

uint64_t flip_diag(uint64_t bitboard) 
{
    uint64_t new_bb = 0ULL;
    for (int i = 0; i < 8; i++)
        new_bb |= ((bitboard & diag_A8H1_masks[i]) >> diag_A8H1_shift[i]);

    for (int i = 8; i < 15; i++)
        new_bb |= ((bitboard & diag_A8H1_masks[i]) << diag_A8H1_shift[i]);

    return new_bb;
}

uint8_t reverse_8_bit(uint8_t x) 
{
   x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
   x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
   x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
   return x;
}

uint64_t flip_anti_diag(uint64_t bitboard) 
{
    uint64_t new_bb = 0ULL;
    for (int i = 0; i < 8; i++)
        new_bb |= ((bitboard & diag_A1H8_masks[i]) >> diag_A1H8_shift[i]);

    for (int i = 8; i < 15; i++)
        new_bb |= ((bitboard & diag_A1H8_masks[i]) << diag_A1H8_shift[i]);

    return new_bb;
}

uint64_t rotate_clock_90(uint64_t b) {return flip_vertical(flip_anti_diag(b));}

uint64_t rotate_counter_90(uint64_t b) {return flip_anti_diag(flip_vertical(b));}

uint64_t rotate_clock_45(uint64_t bitboard) 
{
    uint64_t new_bb = 0ULL;
    for (int i = 0; i < 8; i++)
        new_bb |= ((bitboard & clockwise_45_masks[i]) >> clockwise_45_shift[i]);

    for (int i = 8; i < 15; i++)
        new_bb |= ((bitboard & clockwise_45_masks[i]) << clockwise_45_shift[i]);

    return new_bb;
}

uint64_t rotate_counter_45(uint64_t bitboard) 
{
    return flip_vertical(rotate_clock_45(flip_vertical(bitboard)));
}

uint8_t get_diagonal(uint64_t bitboard, uint8_t i)
{
    if (i > 10 || i < 0) 
        return 0;

    return (rotate_counter_45(bitboard) >> shift_diag[i]) & mask_diag[i];
}

uint8_t get_anti_diagonal(uint64_t bitboard, uint8_t x) 
{
    return reverse_8_bit(get_diagonal(flip_horizontal(bitboard),x));
}

uint64_t set_diagonal(uint64_t bitboard, uint64_t temp, uint8_t x)
{
    if (x > 10 or x < 0) 
        return 0;

    return rotate_clock_45(rotate_counter_45(bitboard) | ((temp & mask_diag[x]) << shift_diag[x]));
}

uint64_t set_anti_diagonal(uint64_t bitboard, uint64_t temp, uint8_t x)
{ 
    return flip_horizontal(set_diagonal(flip_horizontal(bitboard),reverse_8_bit(temp),x));
}

uint8_t get_row(uint64_t bitboard, uint8_t x)
{
    return (bitboard >> (7-x)*8) & 0xff;
}

uint8_t get_column(uint64_t bitboard, uint8_t x)
{
    return get_row(rotate_clock_90(bitboard), x);
}

uint64_t set_row(uint64_t bitboard, uint64_t temp, uint8_t x) 
{
    return bitboard | ((temp & 0xff) << (7-x)*8);
}

uint64_t set_column(uint64_t bitboard, uint64_t temp, uint8_t x) 
{
    return rotate_counter_90(set_row(rotate_clock_90(bitboard), temp, x));
}