#ifndef PLAYER_H_
#define PLAYER_H_ 
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>

 //#include "chessSystem.h"
 //#include "tournament.h"
 #include "map.h"
 #include <stdbool.h>


typedef struct player_t *Player;
//Player CreatePlayer(int id, int num_wins, int num_losses, int num_draws);
Player CreatePlayer(int id);
void* copyDataPlayer (void* player);
void* copyKeyPlayer(void* player); //player's id
void freeDataPlayer(void* data_player);
void freeKeyPlayer(void* player_id); //it is an integer
int compareIdPlayer(void* first, void* second);

void setPlayerWinsLossesDrawsZero(Player player);


int getPlayerId(Player player);

int getPlayerWins(Player player);

int getPlayerLosses(Player player);

int getPlayerDraws(Player player);

// int getPlayerScore(Player player);

//void returnsTotalResultPerPlayer(Player player, int* wins, int* losses, int* drows);

void addWinsToPlayer(Player player);

void addLossesToPlayer(Player player);


void addDrawsToPlayer(Player player);

int getPlayerNumGamesTotal(Player player);

double SaveScoreForLevel(Player player);
double numGamesPerPlayer(Player player);

void removeWinsToPlayer(Player player, int num);

void removeLosesToPlayer(Player player, int num);

void removeDrawsToPlayer(Player player, int num);
int compareForTotPlayers(void* first, void* second);
void setDrawsPlayer(Player player, int num);
#endif /* PLAYER_H_ */
