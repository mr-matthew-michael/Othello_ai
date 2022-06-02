#ifndef _BOARDpp_H
#define _BOARDpp_H
#include <unordered_map>
#include <vector>
#include <string>

uint64_t flip_vertical(uint64_t bitboard);
uint64_t flip_horizontal(uint64_t bitboard);
uint64_t flip_diag(uint64_t bitboard);
uint64_t flip_anti_diag(uint64_t bitboard);

uint8_t reverse_8_bit(uint8_t x);
uint64_t set_bit(uint64_t bitboard, int square);

uint64_t rotate_clock_90(uint64_t bitboard);
uint64_t rotate_counter_90(uint64_t bitboard);
uint64_t rotate_clock_45(uint64_t bitboard);
uint64_t rotate_counter_45(uint64_t bitboard);

uint8_t get_diagonal(uint64_t bitboard, uint8_t x);
uint8_t get_anti_diagonal(uint64_t bitboard, uint8_t x);
uint8_t get_row(uint64_t bitboard, uint8_t x);
uint8_t get_column(uint64_t bitboard, uint8_t x);

uint64_t set_diagonal(uint64_t b, uint64_t d, uint8_t x);
uint64_t set_anti_diagonal(uint64_t b, uint64_t d, uint8_t x);
uint64_t set_row(uint64_t b, uint64_t d, uint8_t x);
uint64_t set_column(uint64_t b, uint64_t d, uint8_t x);

const uint64_t vertical_masks[6] = 
{
    0x00ff000000000000, 0x0000ff0000000000, 0x000000ff00000000, 
    0x00000000ff000000, 0x0000000000ff0000, 0x000000000000ff00
};

const uint8_t vertical_shift[8] = {56, 40, 24, 8, 8, 24, 40, 56};

const uint64_t horizontal_masks[8] = 
{
    0x8080808080808080, 0x4040404040404040, 0x2020202020202020, 
    0x1010101010101010, 0x0808080808080808,0x0404040404040404, 
    0x0202020202020202, 0x0101010101010101
};
const uint8_t horizontal_shift[8] = {7, 5, 3, 1, 1, 3, 5, 7};

const uint64_t diag_A8H1_masks[15] = 
{
    0x8040201008040201, 0x4020100804020100, 0x2010080402010000, 
    0x1008040201000000, 0x0804020100000000, 0x0402010000000000,
    0x0201000000000000, 0x0100000000000000, 0x0080402010080402, 
    0x0000804020100804, 0x0000008040201008, 0x0000000080402010,
    0x0000000000804020, 0x0000000000008040, 0x0000000000000080
};
const uint8_t diag_A8H1_shift[15] = {0, 7, 14, 21, 28, 35, 42, 49, 7, 14, 21, 28, 35, 42, 49};

const uint64_t diag_A1H8_masks[15] = 
{
    0x0102040810204080, 0x0204081020408000, 0x0408102040800000, 
    0x0810204080000000, 0x1020408000000000, 0x2040800000000000,
    0x4080000000000000, 0x8000000000000000, 0x0001020408102040, 
    0x0000010204081020, 0x0000000102040810, 0x0000000001020408,
    0x0000000000010204, 0x0000000000000102, 0x0000000000000001
};
const uint8_t diag_A1H8_shift[15] = {0, 9, 18, 27, 36, 45, 54, 63, 9, 18, 27, 36, 45, 54, 63};

const uint64_t clockwise_45_masks[15] = 
{
    0x8080808080808080, 0x4040404040404000, 0x2020202020200000, 
    0x1010101010000000, 0x0808080800000000, 0x0404040000000000,
    0x0202000000000000, 0x0100000000000000, 0x0000000000000040, 
    0x0000000000002020, 0x0000000000101010, 0x0000000008080808,
    0x0000000404040404, 0x0000020202020202, 0x0001010101010101
};
const uint8_t clockwise_45_shift[15] = {0, 8, 16, 24, 32, 40, 48, 56, 56, 48, 40, 32, 24, 16, 8};
const uint8_t shift_diag[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
const uint8_t mask_diag [11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};

#endif