#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *sq_arr_2[64] =
{
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};


static char *sq_arr[64] =
{
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

char * get_line(void) 
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
        {
            break;
        }

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    //linep[(strlen(linep)-1)] = '\0';
    //printf("asdf: [%s]\n", linep);
    return linep;
}

//run bot through this 
void print_bot_choice(int ab_choice, int player)
{

    if (ab_choice == -1 && player == 1)
    {
        printf("B\n");
        fflush(stdout);
        return;
    } else if (ab_choice == -1 && player == -1)
    {
        printf("W\n");
        fflush(stdout);
        return;
    }

    char* user_sq = calloc(2, sizeof(char));
    char user_sq_dump[6] = {'\0'};
    int sq_choice = 0;
    int move_check = 0;

    sq_choice = ab_choice;
    memset(user_sq, 2, '\0');
    user_sq = sq_arr[ab_choice];

    memset(user_sq_dump, 6, '\0');
    if (player == 1)
    {
        user_sq_dump[0] = 'B';
        user_sq_dump[1] = ' ';
        user_sq_dump[2] = user_sq[0];
        user_sq_dump[3] = ' ';
        user_sq_dump[4] = user_sq[1];
        user_sq_dump[strcspn(user_sq_dump, "\0")] = 0;
        printf("%s\n", user_sq_dump);
    }
    else if (player == -1)
    {
        user_sq_dump[0] = 'W';
        user_sq_dump[1] = ' ';
        user_sq_dump[2] = user_sq[0];
        user_sq_dump[3] = ' ';
        user_sq_dump[4] = user_sq[1];
        user_sq_dump[strcspn(user_sq_dump, "\0")] = 0;
        printf("%s\n", user_sq_dump);
    }
/*
    return user_sq;

    move_check = check_move(sq_choice, sq_choice_2);

    if (move_check == -1)
    {
        printf("END OF GAME\n");
        end_game = 1;
    }
    else if (move_check == -3)
        return;

    player *= -1;
*/
    return;
}


//initilize game set preferred player
int start_game(int player)
{
    char* user_sq = malloc(3*sizeof(char));
    char* start_set_B = "I B";
    char* start_set_W = "I W";

    printf("C Enter I B or I W to start game: \n");
    user_sq = get_line();
    user_sq[strcspn(user_sq, "\n")] = 0;

    if (strcmp(start_set_B, user_sq) == 0)
    {
        printf("R B\n");
        fflush(stdout);
        player = -1;
        return player;
    }
    else if(strcmp(start_set_W, user_sq) == 0)    
    {
        printf("R W\n");
        fflush(stdout);
        player = 1;  
        return player; 
    }
    else
    {
        printf("C ***Error [%s] not recognized***\n", user_sq);
        fflush(stdout);
        start_game(player);
    }   
    return 0;
}

char* input_adjust(char* input, int player)                                         
{
    int i,j;
    char *output=malloc(sizeof(input));
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ' && input[i]!= 'B' && input[i]!= 'W' )                           
            output[j]=input[i];                     
        else
            j--;                                     
    }

    output[j]=0;
    return output;
}

int player_1_turn_prompt(int player)
{
    char* user_sq = malloc(2*sizeof(char));
    int sq_choice = 0;
    int move_check = 0;

    if(player == -1)
    {
        printf("C P2 enter move: \n");
        fflush(stdout);
        user_sq = get_line();
        user_sq[strcspn(user_sq, "\n")] = 0;
        user_sq = input_adjust(user_sq, player);
    } else 
    {
        printf("C P1 enter move: \n");
        fflush(stdout);
        user_sq = get_line(); 
        user_sq[strcspn(user_sq, "\n")] = 0;
        if(strlen(user_sq) > 2)
            user_sq = input_adjust(user_sq, player);
        //printf("\n[%s]\n", user_sq);
    }

    if ((user_sq[0] == 'B' || user_sq[0] == 'W') && (strlen(user_sq) < 2))
    {
        if (player == 1)
        {
            printf("B\n");
            fflush(stdout);
        }
        else 
        {
            printf("W\n");
            fflush(stdout);
        }

        return -1;
    }

    for (int i = 0; i <= 63; i++) 
    {
        if (strcmp(sq_arr[i], user_sq) == 0)
        { 
            sq_choice = i;
            break;      
        }
    }

    return sq_choice;
}