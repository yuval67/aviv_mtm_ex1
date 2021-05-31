#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "map.h"
#include "KeyGame.h"
#include "chessSystem.h"
#define PLAYER_IS_OUT -1
#define SECOND_PLAYER_WINS 2
#define FIRST_PLAYER_WINS 1


struct KeyGame_t
{   
    int first_player_id;
    int second_player_id;
    //int game_index;
};

//---------------------------------------------------------------------------------------------------------------------------------

int getFirstId(KeyGame key){
    return key->first_player_id;
}

int getSecondId(KeyGame key){
    return key->second_player_id;
}


void setWinnerInKeyGame(KeyGame key, int player_id, DataGame data){
    if (key->first_player_id == player_id){
        //key->first_player_id = PLAYER_IS_OUT;
        setWinnerInGame(data,SECOND_PLAYER);
        //data->winner = SECOND_PLAYER;
    }
    if (key->second_player_id == player_id){
        //key->second_player_id = PLAYER_IS_OUT;
        setWinnerInGame(data,FIRST_PLAYER);
    }
    return;
}

bool containIdKeyGame(KeyGame game, int player_id){
    if ((game->first_player_id == player_id) || game->second_player_id == player_id){
        return true;
    }
    return false;
}

// int getIndexGame(KeyGame key){
//     return key->game_index;
// }

//---------------------------------------------------------------------------------------------------------------------------------


KeyGame CreateKeyGame(int first_player_id, int second_player_id){
    // if(first_player_id < 0 || second_player_id < 0){
    //     return NULL;
    // }
    KeyGame new_Game = (KeyGame)malloc(sizeof(new_Game));
    if (new_Game == NULL) {
        return NULL;
    }
    new_Game->first_player_id = first_player_id;
    new_Game->second_player_id = second_player_id;
    //new_Game->game_index = game_index;
    //new_Game->player_deleted = false; //player_deleted
    return new_Game;
}

// int compareIdInKeyGame(void* first, void* second){
//         if (first == NULL || second == NULL){
//         return 0;
//     }
//     if (((KeyGame)first)->first_player_id == ((KeyGame)second)->first_player_id && ((KeyGame)first)->second_player_id == ((KeyGame)second)->second_player_id){
//         return 0;
//     }
//     if (((KeyGame)first)->first_player_id == ((KeyGame)second)->second_player_id && ((KeyGame)first)->second_player_id == ((KeyGame)second)->first_player_id){
//         return 0;
//     }
//     else return 1;
// }

int compareKeyGame(void* first, void* second){
    // if (first == NULL || second == NULL){
    //     return 0;
    // }
    if (((KeyGame)first)->first_player_id == ((KeyGame)second)->first_player_id 
    && ((KeyGame)first)->second_player_id == ((KeyGame)second)->second_player_id 
    /*&& ((KeyGame)first)->game_index == ((KeyGame)second)->game_index*/){
        return 0;
    }
    if (((KeyGame)first)->first_player_id == ((KeyGame)second)->second_player_id 
    && ((KeyGame)first)->second_player_id == ((KeyGame)second)->first_player_id
    /*&& ((KeyGame)first)->game_index == ((KeyGame)second)->game_index*/){
        return 0;
    }
    else return 1;
}

void* copyKeyGame (void* key_game){
    if (key_game == NULL){
        return NULL;
    }
    // if(((KeyGame)key_game)->first_player_id < 0 || ((KeyGame)key_game)->second_player_id < 0){
    //     return NULL;
    // }
    KeyGame new_KeyGame = (KeyGame)malloc(sizeof(struct KeyGame_t));
    if(new_KeyGame == NULL){
        return NULL;
    }
    new_KeyGame->first_player_id = ((KeyGame) key_game)->first_player_id;
    new_KeyGame->second_player_id = ((KeyGame)key_game)->second_player_id;
    //new_KeyGame->game_index = ((KeyGame)key_game)->game_index;
    //new_KeyGame->player_deleted = ((KeyGame)key_game)->player_deleted; //player_deleted
    return new_KeyGame;
}

 void freeKeyGame (void* key){
    if ((KeyGame)key == NULL){
         return;
    }
    free(key);
}



