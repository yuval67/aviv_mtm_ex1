#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
//#include "chessSystem.h"
//#include "KeyGame.h"
#include "player.h"
#include "tournament.h"
//#include "DataGame.h"
#include "string.h"
#define REMOVE_SECOND_PLAYER -2
#define REMOVE_FIRST_PLAYER -1

#define ID_NOT_VALID -2

struct tournament_t{
    int tournament_id;
    int max_games;
    char* tournament_location;
    Map games;
    Map players;
    bool is_active;
    int winner;
    int num_total_players;
};

//-----------------------------------------------------------------------------------------------------------------------------

void returnPlayerScores(Tournament tournament, int* player_id, int* wins, int* losses, int* draws){
    // MAP_FOREACH(int *, key_player, tournament->players){
     Player player_data = mapGet(tournament->players, player_id);
    //     freeKeyPlayer(key_player);
    *wins = *wins+ getNumWins(player_data);
    *losses = *losses+ getNumLosses(player_data);
    *draws = *draws+ getNumDraws(player_data);
}




void setAllTournament(Tournament tournament){
    MAP_FOREACH(int *, key_player, tournament->players){
        Player player_data = mapGet(tournament->players, key_player);
        freeKeyPlayer(key_player);
        setPlayerWinsLossesDrawsZero(player_data);
    }
    DataGame game_data = NULL;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        game_data = (DataGame)mapGet(tournament->games, key_game);
        Winner winner = getWinner(game_data);
        int first_player = getFirstId(key_game);
        int second_player = getSecondId(key_game);
        Player player_data_first = mapGet(tournament->players, &first_player);
        Player player_data_second = mapGet(tournament->players, &second_player);
        // if (player_data_first == NULL || player_data_second == NULL){
        //     return;
        // }
        if (winner == FIRST_PLAYER){
            addWinsToPlayer(player_data_first);
            addLossesToPlayer(player_data_second);
        }
        if (winner == SECOND_PLAYER){
            addLossesToPlayer(player_data_first);
            addWinsToPlayer(player_data_second);
        }
        if(winner == DRAW){
            addDrawsToPlayer(player_data_first);
            addDrawsToPlayer(player_data_second);
        }
        freeKeyGame(key_game);
    }
}




int getTournamentId(Tournament tournament){
    return tournament->tournament_id;
}

int getTournamentMaxGame(Tournament tournament){
    return tournament->max_games;
}

int getTournamentNumPlayers(Tournament tournament){
    return tournament->num_total_players;
}

bool getTournamentIsActive(Tournament tournament){
    //printf ("tournament id is %d\n" ,(tournament->tournament_id));
    //printf ("map size is %d\n" ,mapGetSize(tournament->games));
    if (tournament->is_active){
        return true;
    }
    return false;
}

char* getTournamentLocation(Tournament tournament){
    return tournament->tournament_location;
}


bool tournamentPlayerContain(Tournament tournament, int player_id){
    int equal_players = 1;
    MAP_FOREACH(int *, key_player, tournament->players){
        Player player_data = mapGet(tournament->players, key_player);
        freeKeyPlayer(key_player);
        int res = getPlayerId(player_data) - player_id;
        if (res == 0){
            equal_players = 0;
        }
        else {
            continue;
        }
    }
    if (equal_players == 1){
        return false;
    }
    return true;
}

void setTournamentIsActive(Tournament tournament, bool is_active){
    tournament->is_active = is_active;
}

// void setIfPlayerDeletedInTournament(KeyGame game){
//     MAP_FOREACH(KeyGame, key_game, tournament->games){
//         int compare_result = compareKeyGame(key_game, game); 
//         freeKeyGame(key_game);
//         if(compare_result == 0){
//             setIfPlayerDeleted(game);
//         }
//     }
// }

// bool getIfPlayerDeleted(KeyGame games){
//     bool deleted = false;
//     MAP_FOREACH(KeyGame, key_game, tournament->games){
//         int compare_result = compareKeyGame(key_game, game); 
//         freeKeyGame(key_game);
//         if(compare_result == 0){
//             if (getIfPlayerDeleted(game)){
//                 deleted = true;
//             }
//         }
//     }
//     return deleted;
// }


void removePlayerFromPlayerMap(Tournament tournament, int player_id){
    Player player_data = mapGet(tournament->players, &player_id);
    if (player_data == NULL){
        return;
    }
    mapRemove(tournament->players, &player_id);
}


// void removePlayerFromGameMap(Tournament tournament, int player_id){
//     DataGame game_data = NULL;
//     MAP_FOREACH(KeyGame, key_game, tournament->games){
//         game_data = (DataGame)mapGet(tournament->games, key_game);
//         removePlayerFromGame(key_game, player_id, game_data);
//         freeKeyGame(key_game);
//     }
// }

// void reduceResultGameBeforRemove(Winner winner, int player_id, int player_first, int player_second){
//     int id = 0;
//     if (winner == FIRST_PLAYER){
//         id = 
//     }

    
// }


void removePlayerFromGameMap(Tournament tournament, int player_id){
    int first_player = 0;
    int second_player = 0;
    //int index = 0;
    DataGame game_data = NULL;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        game_data = (DataGame)mapGet(tournament->games, key_game);
        if (getFirstId(key_game) == player_id){
            first_player = getFirstId(key_game)*(-1);
            second_player = getSecondId(key_game);
        }
        else if (getSecondId(key_game) == player_id){
            first_player = getFirstId(key_game);
            second_player = getSecondId(key_game)*(-1);
        }
        else{ //is there option???
            continue;
        }
        //index = getIndexGame(key_game);
        KeyGame new_key_game = CreateKeyGame(first_player, second_player);
        if (new_key_game == NULL){
            return;
        }
       // Player player= mapGet(tournament->players, &player_id);
        // Winner winner = getWinner(mapGet(tournament->games, key_game));
        // int player_first = getFirstId(mapGet(tournament->games, key_game));
        // int player_second = getSecondId(mapGet(tournament->games, key_game))
        //reduceResultGameBeforRemove(winner, player_id, player_first, player_second);
        setWinnerInKeyGame(key_game, player_id, game_data);
        DataGame new_data_game = copyDataGame(game_data);
        if (new_data_game == NULL){
            freeKeyGame(new_key_game);
            return;
        }
        //int size = mapGetSize(tournament->games);
        //printf ("map size before %d\n", size);
        //int size_1 = mapGetSize(tournament->games);
        //printf ("map size int the middle %d\n", size_1);
        mapRemove(tournament->games, key_game);
        int res = mapPut(tournament->games, new_key_game, new_data_game);
        printf("res is %d\n", (int)res);
        //int size_2 = mapGetSize(tournament->games);
        //printf ("map size after %d\n", size_2);
        freeKeyGame(new_key_game);
        freeDataGame(new_data_game);
        freeKeyGame(key_game);
    }
}


bool tournamentsGameContain(Tournament tournament, KeyGame game){
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        int compare_result = compareKeyGame(key_game, game); 
        freeKeyGame(key_game);
        if(compare_result == 0){
            return true;
        }
        else {
            continue;
        }
    }
    return false;
}


void addPlayersGameToPlayerMap(Tournament tournament, int player, Player data){
    mapPut(tournament->players, &player, data);
}

void removePlayersGameToPlayerMap(Tournament tournament, int player){
    mapRemove(tournament->players, &player);
}

void addPlayersGameToGameMap(Tournament tournament, KeyGame key, DataGame data){
    mapPut(tournament->games, key, data);
    //printf( "map size is %d", mapGetSize(tournament->games));
}


void addTournamentNumPlayers(Tournament tournament){ 
    int temp = getTournamentNumPlayers(tournament);
    tournament->num_total_players = ++temp;
}

void sumTournamentNumPlayers(Tournament tournament){ 
    int temp = getTournamentNumPlayers(tournament);
    tournament->num_total_players = --temp;
}

void setTournamentWinner(Tournament tournament, int winner_id){
   tournament->winner = winner_id;
}


void setResultForGame(Tournament tournament, Winner winner, int first_player, int second_player){
    Player player_data_first = mapGet(tournament->players, &first_player);
    Player player_data_second = mapGet(tournament->players, &second_player);
    if (player_data_first == NULL || player_data_second == NULL){
        return;
    }
    if (winner == FIRST_PLAYER){
        addWinsToPlayer(player_data_first);
        addLossesToPlayer(player_data_second);
    }
    if (winner == SECOND_PLAYER){
        addLossesToPlayer(player_data_first);
        addWinsToPlayer(player_data_second);
    }
    if(winner == DRAW){
        addDrawsToPlayer(player_data_first);
        addDrawsToPlayer(player_data_second);
    }
}


bool numGamePlayerLessThenMax(Tournament tournament, int player_id){
    Player player_data = mapGet(tournament->players, &player_id);
    if (player_data == NULL){
        return false;
    }
    if (getPlayerNumGamesTotal(player_data) < tournament->max_games){
        return true;
    }
    return false;
}

int determaineWinnerInTournament(Tournament tournament){
    int winner_id = -1;
    int max_score = 0;
    int temp = 0;
    int num_losses = 0;
    int num_wins = 0;
    MAP_FOREACH(int *, key_player, tournament->players){
        Player data_player= mapGet(tournament->players,key_player);
        freeKeyPlayer(key_player);
        if (data_player == NULL){
            return CHESS_OUT_OF_MEMORY;
        }
        temp = 2*(getPlayerWins(data_player)) + (getPlayerDraws(data_player));
        if (temp > max_score){
            max_score = temp;
            num_losses = getPlayerLosses(data_player);
            num_wins = getPlayerWins(data_player);
            winner_id = getPlayerId(data_player);
        }
        if (temp == max_score){
            if (getPlayerLosses(data_player) < num_losses){
                max_score = temp;
                num_losses = getPlayerLosses(data_player);
                winner_id = getPlayerId(data_player);
            }
            if (getPlayerLosses(data_player) == num_losses){
                if (getPlayerWins(data_player) > num_wins){
                    max_score = temp;
                    num_losses = getPlayerLosses(data_player);
                    winner_id = getPlayerId(data_player);
                }
                if (getPlayerWins(data_player) == num_wins){
                    if (getPlayerId(data_player) < winner_id){
                        max_score = temp;
                        num_losses = getPlayerLosses(data_player);
                        winner_id = getPlayerId(data_player);
                    }
                }
            }
        }
    }
    return winner_id;
}


bool tournametIsMapGamesEmpty(Tournament tournament){
    int size = mapGetSize(tournament->games);
    if (size == 0){
        return true;
    }
    return false;
}


int calculatingPlayTimeOfPlayer(Tournament tournament, int player_id){
    int sum = 0;
    DataGame temp = NULL;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        if(containIdKeyGame(key_game, player_id)){
            temp = (mapGet(tournament->games, key_game));
            freeKeyGame(key_game);
            sum += getPlayTime(temp);
        }
    }
    //freeDataGame(temp);
    return sum;
}


int calculatingNumOfGamesForPlayer(Tournament tournament, int player_id){
    int num_games = 0;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        if(containIdKeyGame(key_game, player_id)){
            num_games++;
        }
    }
    return num_games;
}


// void tournamentSavePlayersLevels(Tournament tournament, FILE* file, int num_total_games){
//     if (tournament == NULL || file == NULL){
//         return;
//     }
//     MAP_FOREACH(int *, key_player, tournament->players){
//         Player player = mapGet(tournament->players, key_player);
//         if (player == NULL){
//             return;
//         }
//         fprintf(file, "%d ", getPlayerId(player));
//         double level = SaveScoreForLevel(player)/numGamesPerPlayer(player);
//        //double level= calcLevel(player);
//         fprintf(file, "%.2lf", level);
//         fprintf(file, "\n");
//         //freeDataPlayer(player);
//     }
//     return;
// }


int getNumGames(Tournament tournament){
    return mapGetSize(tournament->games);
}


int getNumPlayers(Tournament tournament){
    return mapGetSize(tournament->players);
}


double averageGameTimeOfTournament(Tournament tournament){
    double total_time = 0;
    double total_games=0;
    DataGame temp = NULL;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        temp = (mapGet(tournament->games, key_game));
        freeKeyGame(key_game);
        total_time += getPlayTime(temp);
        total_games++;
    }
    //freeDataGame(temp);
    if (total_games==0){
        return 0;
    }
    return (total_time/total_games);
}


int calcLongestPlayTimeInTournament(Tournament tournament){
    int longest_time = 0;
    DataGame temp = NULL;
    MAP_FOREACH(KeyGame, key_game, tournament->games){
        temp = (mapGet(tournament->games, key_game));
        freeKeyGame(key_game);
        longest_time = (longest_time < getPlayTime(temp)) ? getPlayTime(temp) : longest_time;
    }
    //freeDataGame(temp);
    return longest_time;
}


void SaveTournamentStatistics(Tournament tournament, FILE* file){
    if (tournament == NULL || file == NULL){
        return;
    }
    fprintf(file, "%d\n", getTournamentId(tournament));
    fprintf(file,"%d\n", calcLongestPlayTimeInTournament(tournament));
    fprintf(file,"%.2lf\n", averageGameTimeOfTournament(tournament));
    fprintf(file, "%s\n", getTournamentLocation(tournament));
    fprintf(file,"%d\n", getNumGames(tournament));
    fprintf(file,"%d\n", getNumPlayers(tournament));
    
}


void setScoreIntotalplayers(Map total_players, Tournament tournament){
    if (tournament == NULL || total_players == NULL){
        return;
    }
    MAP_FOREACH(int *, key_player, tournament->players){
        int winsToRemove = 0;
        int lossesToRemove = 0;
        int drawsToRemove = 0;
        Player player_data = mapGet(tournament->players, key_player);
        freeKeyPlayer(key_player);
        if (player_data == NULL){
            return;
        }
        winsToRemove = getPlayerWins(player_data);
        lossesToRemove = getPlayerLosses(player_data);
        drawsToRemove = getPlayerDraws(player_data);
        Player tot = mapGet(total_players, key_player);
        removeWinsToPlayer(tot, winsToRemove);
        removeLosesToPlayer(tot, lossesToRemove);
        removeDrawsToPlayer(tot, drawsToRemove);
    }
}

double getSaveScoreForLevel(Player player){
    return SaveScoreForLevel(player)/numGamesPerPlayer(player);
}


//-----------------------------------------------------------------------------------------------------------------------------


void* copyDataTournament(void* tournament){
    if (tournament == NULL){
        return NULL;
    }
    Tournament new_tournament = (Tournament)malloc(sizeof(*new_tournament));
    if (new_tournament == NULL){
        return NULL;
    }
    //printf ("%s", ((Tournament)tournament)->tournament_location);
    new_tournament->tournament_id = ((Tournament)tournament)->tournament_id;
    new_tournament->max_games = ((Tournament)tournament)->max_games;
    new_tournament->is_active = ((Tournament)tournament)->is_active;
    new_tournament->winner = ((Tournament)tournament)->winner;
    new_tournament->num_total_players = ((Tournament)tournament)->num_total_players;
    //new_tournament->tournament_location = (char*)malloc(strlen(((Tournament)tournament)->tournament_location)+1);
    
    size_t new_location_size = strlen(((Tournament)tournament)->tournament_location) + 1;
    char *new_location = (char*)malloc(sizeof(*new_location) * new_location_size);
    if (new_location == NULL){
        //free((void*)new_location_size);
        freeDataTournament(new_tournament);
        return NULL;
    }
    //Tournament t=((Tournament)tournament);
    strcpy(new_location, ((Tournament)tournament)->tournament_location);
    Map new_games = mapCopy(((Tournament)tournament)->games);
    if (new_games == NULL){
        //free((void*)new_location_size);
        free(new_location);
        freeDataTournament(new_tournament);
        return NULL;
    }
    //Map new_players = mapCreate(copyDataPlayer, copyKeyPlayer, freeDataPlayer, freeKeyPlayer, compareIdPlayer);
    Map new_players= mapCopy(((Tournament)tournament)->players);
    if (new_players == NULL){
        free(new_location);
        //free((void*)new_location_size);
        mapDestroy(new_games);
        freeDataTournament(new_tournament);
        return NULL;
    }
    new_tournament->tournament_location = new_location;
    new_tournament->games = new_games;
    new_tournament->players = new_players;
    return (Tournament)new_tournament;
}


void* copyKeyTournament(void* tournament_id){
    if (tournament_id == NULL){
        return NULL;
    }
    int* new_key = (int*)malloc(sizeof(int));
    if(new_key == NULL){
        return NULL;
    }
    *new_key = *((int*)tournament_id);
    return new_key;
}


void freeDataTournament(void* tournament){
    if (tournament == NULL){
        return;
    }
    mapDestroy(((Tournament)tournament)->games); //TODO
    mapDestroy(((Tournament)tournament)->players); //TODO
    free(((Tournament)tournament)->tournament_location);
    free(tournament);
}


void freeKeyTournament(void* tournament){
    free(tournament);
    return;
}


// int compareIdtournament (void* first_id, void* second_id){
//     int id_one = *(int*)first_id;
//     int id_two = *(int*)second_id;
//     printf ("%p\n", first_id);
//     printf ("%p" ,second_id);
//     if ( *(*int)first_id) < 0 || *(*int)second_id) < 0){
//         return ID_NOT_VALID;
//     }
//     if ( *((int*)first_id) - *((int*)second_id) > 0){
//         return 1;
//     }
//     if ( *((int*)first_id) - *((int*)second_id) < 0){
//         return -1;
//     }
//     return 0;
// }

int compareIdtournament (void* first_id, void* second_id){
    if (first_id == NULL || second_id == NULL){
        return 0;
    }
    int id_first = *(int*)first_id;
    int id_second = *(int*)second_id;
    // printf ("first value: %d\n", id_one);
    // printf ("second value: %d" ,id_two);
    if (id_first < 0 || id_second < 0){
        return ID_NOT_VALID;
    }
    if ((id_first - id_second) > 0){
        return 1;
    }
    if ((id_first - id_second) < 0){
        return -1;
    }
    return 0;
}

Tournament createTournament(int tournament_id, int max_games, const char* tournament_location){
    if (tournament_location == NULL){
        return NULL;
    }
    Tournament new_tournament = (Tournament)malloc(sizeof(struct tournament_t));
    if(new_tournament == NULL){
        return NULL;
    }
    new_tournament->tournament_id = tournament_id;
    new_tournament->max_games = max_games;
    new_tournament->tournament_location = (char*)malloc(strlen(tournament_location)+1);
    if (new_tournament->tournament_location == NULL){
        free(new_tournament);
        return NULL;
    }
    strcpy(new_tournament->tournament_location, tournament_location); 
    new_tournament->games = mapCreate(copyDataGame, copyKeyGame, freeDataGame, freeKeyGame, compareKeyGame);
    new_tournament->players = mapCreate(copyDataPlayer, copyKeyPlayer, freeDataPlayer, freeKeyPlayer, compareIdPlayer);
    new_tournament->is_active = true;
    new_tournament->winner = -1;
    new_tournament->num_total_players = 0;
    return new_tournament;
}