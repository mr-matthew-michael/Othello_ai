#ifndef _MOVE_ENGINE_H
#define _MOVE_ENGINE_H
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class FlipEngine 
{
    public:
        FlipEngine();
        uint64_t set_bit(uint64_t bitboard, int square);
        uint64_t get_bit(uint64_t bitboard, int square);
        uint64_t get_updated_board(uint64_t p1, uint64_t p2, int square);
        void print_board(uint64_t player_1, uint64_t player_2);
        void print_single_bitboard(uint64_t bitboard);
        vector<int> get_bit_list(uint64_t move_board);
        void playMove(uint64_t * player, uint64_t * opponent, int move);
    private:
        uint64_t flip_table_location(int p1, int p2, int sq);
        void compute_flip(uint8_t player_1, uint8_t player_2);
        void generate_flip_table();
        vector<int> getMoveList(bool t = false);
        uint64_t current_move;
        unordered_map<int, char> flip_map;
};

#endif