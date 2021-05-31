#ifndef KeyGame_H_
#define KeyGame_H_

//#include "chessSystem.h"
#include "DataGame.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "tournament.h"

typedef struct KeyGame_t *KeyGame;

KeyGame CreateKeyGame (int first_player_id, int second_player_id);
int compareKeyGame (void* first, void* second);
void* copyKeyGame (void* key_game);
void freeKeyGame (void* key);
int isPlayerInGame (void* game ,int player_id);

int getFirstId (KeyGame key);
int getSecondId (KeyGame key);
void setWinnerInKeyGame(KeyGame key, int player_id, DataGame data);
int returnWinner(KeyGame key, int player_id);

bool containIdKeyGame(KeyGame game, int player_id);
//void setPlayerId(KeyGame key, int player_index);
// bool getIfPlayerDeleted(KeyGame key);
// void setIfPlayerDeleted(KeyGame key);
//int compareIdInKeyGame(void* first, void* second);
//int getIndexGame(KeyGame key);

#endif /* KeyGame_H_ */
