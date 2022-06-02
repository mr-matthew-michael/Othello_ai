#include <stdio.h>
#include <stdint.h>

const int DIR_INCREMENTS[8] = {8, 9, 1, -7, -8, -9, -1, 7};

const uint64_t DIR_MASKS[8] = 
{
    0xFFFFFFFFFFFFFF00, //North
    0xFEFEFEFEFEFEFE00, //NorthWest
    0xFEFEFEFEFEFEFEFE, //West
    0x00FEFEFEFEFEFEFE, //SouthWest
    0x00FFFFFFFFFFFFFF, //South
    0x007F7F7F7F7F7F7F, //SouthEast
    0x7F7F7F7F7F7F7F7F, //East
    0x7F7F7F7F7F7F7F00  //NorthEast
};

int bit_count (uint64_t mask)
{
    int count=0;
    uint64_t hold_mask = mask;
    for (int i=0; i<64; i++){
        if ((hold_mask & 1L) != 0)
        count++;
        hold_mask = hold_mask >> 1;
    }
     return count;
}

uint64_t generate_moves(int player, uint64_t p1_board, uint64_t p2_board)
{
    uint64_t empty_mask = ~p1_board & ~p2_board;
    uint64_t hold_mask, dir_move_mask, move_mask = 0;

    for (int i=0; i<8; i++)
    {
        hold_mask = p1_board;
        if (DIR_INCREMENTS[i] > 0)
            hold_mask = (hold_mask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
        else
            hold_mask = (hold_mask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
        hold_mask = hold_mask & p2_board;

        for (int j=0; ((j<6)&(hold_mask!=0L)); j++)
        {
            if (DIR_INCREMENTS[i] > 0)
                hold_mask = (hold_mask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
            else
                hold_mask = (hold_mask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];

            dir_move_mask = hold_mask & empty_mask;
            move_mask = move_mask | dir_move_mask;
            hold_mask = hold_mask & ~dir_move_mask & p2_board;
        }
    }
    return move_mask;
}
