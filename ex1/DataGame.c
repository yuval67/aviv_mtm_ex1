 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "chessSystem.h"
 #include "DataGame.h"
 

 struct datagame_t
{
    Winner winner;
    int play_time;
};


//---------------------------------------------------------------------------------------------------------------------

int getPlayTime(DataGame game){
    return game->play_time;
}

void setWinnerInGame(DataGame game, Winner winner){
    game->winner = winner;
}

Winner getWinner(DataGame game){
    return game->winner;
}

//---------------------------------------------------------------------------------------------------------------------

DataGame CreateDataGame(Winner winner, int play_time){
    // if(play_time < 0){
    // return NULL;
    // }
    DataGame new_Game = (DataGame)malloc(1*sizeof(*new_Game));
    if(new_Game == NULL){
        return NULL;
    }
    new_Game->winner = winner;
    new_Game->play_time = play_time;
    return new_Game;
}

void* copyDataGame(void* game){
    // if(game == NULL){
    //     return NULL;
    // }
    DataGame new_game = malloc(sizeof(*new_game));
    if (new_game == NULL){
        return NULL;
    }
    new_game->winner = ((DataGame)game)->winner;
    new_game->play_time = ((DataGame)game)->play_time;
    return new_game;
}

 void freeDataGame (void* game){
    if ((DataGame)game == NULL){ //TODO
         return;
    }
    free(game);
}




