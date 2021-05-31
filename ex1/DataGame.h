#ifndef DATAGAME_H_
#define DATAGAME_H_

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
//#include "KeyGame.h"
#include "chessSystem.h"

typedef struct datagame_t *DataGame;
DataGame CreateDataGame(Winner winner, int play_time);
void* copyDataGame(void* game);
void freeDataGame (void* game);

int getPlayTime(DataGame game);

void setWinnerInGame(DataGame game, Winner winner);
Winner getWinner(DataGame game);
 #endif /* DATAGAME_H_ */