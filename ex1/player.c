 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include "chessSystem.h"
 #include "KeyGame.h"
#include "DataGame.h"
 #include "player.h"
 //#include "map.h"
 //#include "player.h"
 #include "tournament.h"
 #define ID_NOT_VALID -2
#define PLAYER_IS_OUT -1
 
struct player_t
{
    int player_id;
    int num_wins;
    int num_losses;
    int num_draws;
};

//--------------------------------------------------------------------------------------------------------------------

int getPlayerId(Player player){
    if(player == NULL){
        return -1;
    }
    return player->player_id;
}

int getPlayerWins(Player player){
    if(player == NULL){
        return -1;
    }
    return player->num_wins;
}

int getPlayerLosses(Player player){
    if(player == NULL){
        return -1;
    }
    return player->num_losses;
}

int getPlayerDraws(Player player){
    if(player == NULL){
        return -1;
    }
    return player->num_draws;
}

void setPlayerWinsLossesDrawsZero(Player player){
    if(player == NULL){
        return;
    }
    player->num_wins = 0;
    player->num_draws = 0;
    player->num_losses = 0;
    return;
}


int getPlayerNumGamesTotal(Player player){
    if(player == NULL){
        return -1;
    }
    int sum = 0;
    sum = (player->num_draws) + (player->num_wins) + (player->num_losses);
    return sum;
}


void removeWinsToPlayer(Player player, int num){
    if(player == NULL){
        return;
    }
    int temp = getPlayerWins(player);
    temp -= num;
    player->num_wins = temp;
}

void removeLosesToPlayer(Player player, int num){
    if(player == NULL){
        return;
    }
    int temp = getPlayerLosses(player);
    temp -= num;
    player->num_wins = temp;
}

void removeDrawsToPlayer(Player player, int num){
    if(player == NULL){
        return;
    }
    int temp = getPlayerDraws(player);
    temp -= num;
    player->num_wins = temp;
}



void addWinsToPlayer(Player player){
    if(player == NULL){
        return;
    }
    int temp = getPlayerWins(player);
    player->num_wins = ++temp;
}


void addLossesToPlayer(Player player){
    if(player == NULL){
        return;
    }
    int temp = getPlayerLosses(player);
    player->num_losses = ++temp;
}

void setLossesPlayer(Player player, int num){
    if (player==NULL)
    {
        return;
    }
    player->num_losses=num;
    return;
}

void setWinsPlayer(Player player, int num){
    if (player==NULL)
    {
        return;
    }
    player->num_wins=num;
    return;
}


void setDrawsPlayer(Player player, int num){
    if (player==NULL)
    {
        return;
    }
    player->num_draws=num;
    return;
}


void addDrawsToPlayer(Player player){
    if(player == NULL){
        return;
    }
    int temp = getPlayerDraws(player);
    player->num_draws = ++temp;
}


double SaveScoreForLevel(Player player){
    if(player == NULL){
        return -1;
    }
    double score = 0;
    score = 6*getPlayerWins(player) - 10*getPlayerLosses(player) + 2*getPlayerDraws(player);
    return score;
}

double numGamesPerPlayer(Player player){
    if(player == NULL){
        return -1;
    }
    int num = 0;
    num = getPlayerWins(player) + getPlayerLosses(player) + getPlayerDraws(player);
    return num;
}


//--------------------------------------------------------------------------------------------------------------------


Player CreatePlayer(int player_id){
    Player new_player = (Player)malloc(sizeof(*new_player));
    if(new_player == NULL){
        return NULL;
    }
    new_player-> player_id = player_id;
    new_player->num_losses = 0;
    new_player->num_wins = 0;
    new_player->num_draws = 0;
    return new_player;
}


void* copyDataPlayer(void* player){
    if (!player){
        return NULL;
    }
    Player new_player = (Player)malloc(sizeof(struct player_t));
    if (new_player == NULL){
        return NULL;
    }
    new_player->num_draws = ((Player)player)->num_draws;
    new_player->player_id = ((Player)player)->player_id;
    new_player->num_losses = ((Player)player)->num_losses;
    new_player->num_wins = ((Player)player)->num_wins;
    return new_player;
}


void* copyKeyPlayer(void* player_id){ //player's id
    if (!player_id){
        return NULL;
    }
    int* new_key = (int*)malloc(sizeof(int));
    if(new_key == NULL){
        return NULL;
    }
    *new_key = *((int*)player_id);
    return new_key;  
}


 void freeDataPlayer(void* data_player){
    if ((Player)data_player == NULL){
        return;
    }
    free(data_player);
 }


 void freeKeyPlayer(void* player_id){ //it is an integer
     if(!player_id){
       return;
     }
     free(player_id);
 }


int compareIdPlayer(void* first, void* second){
    if (first == NULL){
        return -1;
    }
    if (second == NULL){
        return 1;
    }
    int player_first = ((Player)first)->player_id;
    int player_second = ((Player)second)->player_id;
    if (player_first - player_second > 0){
        return 1;
    }
    if (player_first - player_second < 0){
        return -1;
    }
    else {
        return 0;
    }
}


int compareForTotPlayers(void* first, void* second){
    if (first == NULL){
        return -1;
    }
    if (second == NULL){
        return 1;
    }
    if (getPlayerId((Player)first) == getPlayerId((Player)second)){
        return 0;
    }
    double scoreFirstPlayer = getSaveScoreForLevel((Player)first);
    double scoreSecondPlayer = getSaveScoreForLevel((Player)second);
    if (scoreFirstPlayer > scoreSecondPlayer){
        return -1;
    }
    if (scoreSecondPlayer > scoreFirstPlayer){
        return 1;
    }
    else { //(scoreFirstPlayer == scoreSecondPlayer)
        if(getPlayerId((Player)first) > getPlayerId((Player)second)){
            return 1;
        }
        else{
            return -1;
        }
    }
}





