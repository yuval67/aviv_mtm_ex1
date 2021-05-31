
#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_ 
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>

 #include "chessSystem.h"
 #include "map.h"
 #include <stdbool.h>
 #include "KeyGame.h"
 #include "DataGame.h"
 #include "player.h"
 
 typedef struct tournament_t *Tournament;

typedef enum {
    //TOUR_OUT_OF_MEMORY,
    //CHESS_NULL_ARGUMENT,
    TOUR_GAME_ALREADY_EXISTS,
    TOUR_FAILED,
    TOUR_GAME_NOT_EXIST,
    TOUR_PLAYER_NOT_EXIST,
    TOUR_PLAYER_ALREADY_EXISTS,
    TOUR_SUCCESS
}TournamentResult;


void* copyDataTournament(void* tournament);
void* copyKeyTournament(void* tournament);
void freeDataTournament(void* tournament);
void freeKeyTournament(void* tournament);
int compareIdtournament (void* first_id, void* second_id);
Tournament createTournament(int tournament_id, int max_games, const char* tournament_location);


int getTournamentId(Tournament tournament);

int getTournamentMaxGame(Tournament tournament);

bool getTournamentIsActive(Tournament tournament);

char* getTournamentLocation(Tournament tournament);

void removePlayerFromPlayerMap(Tournament tournament, int player_id);
void removePlayerFromGameMap(Tournament tournament, int player_id);

void addPlayersGameToPlayerMap(Tournament tournament, int player, Player data);

void addPlayersGameToGameMap(Tournament tournament, KeyGame key, DataGame data);
void removePlayersGameToPlayerMap(Tournament tournament, int player);
//int returnsIdPlayer(Player player, int id);

void setResultForGame(Tournament tournament, Winner winner, int first_player, int second_player);
bool tournamentPlayerContain(Tournament tournament, int player_id);
bool tournamentsGameContain(Tournament tournament, KeyGame game);

//Player returnsPlayer(Tournament tournament, int player_id);

bool numGamePlayerLessThenMax(Tournament tournament, int player_id);

int determaineWinnerInTournament(Tournament tournament);

void setTournamentWinner(Tournament tournament, int winner_id);
void setTournamentIsActive(Tournament tournament, bool is_active);

int getTournamentNumPlayers(Tournament tournament);

int getNumGames(Tournament tournament);


int getNumPlayers(Tournament tournament);

void addTournamentNumPlayers(Tournament tournament);

bool tournametIsMapGamesEmpty(Tournament tournament);

int calculatingPlayTimeOfPlayer(Tournament tournament, int player_id);

int calculatingNumOfGamesForPlayer(Tournament tournament, int player_id);

//void tournamentSavePlayersLevels(Tournament tournament, FILE* file, int num_total_games);

double averageGameTimeOfTournament(Tournament tournament);

int calcLongestPlayTimeInTournament(Tournament tournament);

void SaveTournamentStatistics(Tournament tournament, FILE* file);
void sumTournamentNumPlayers(Tournament tournament);

void setScoreIntotalplayers(Map total_players, Tournament tournament);

double getSaveScoreForLevel(Player player);

// void setIfPlayerDeletedInTournament(KeyGame game);

// bool getIfPlayerDeleted(KeyGame games);
void setAllTournament(Tournament tournament);
void setLossesPlayer(Player player, int num);
void setWinsPlayer(Player player, int num);

void returnPlayerScores(Tournament tournament, int* player_id, *int wins, *int losses, *int draws);

#endif /* TOURNAMENT_H_ */
