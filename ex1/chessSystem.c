#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chessSystem.h"
#include "string.h"
#include "map.h"
#include "tournament.h"
#include "player.h"
#include "KeyGame.h"
#include "DataGame.h"

struct chess_system_t
{
    Map tournaments;
    Map totalplayers;
    Map sortedTotalPlayers;
};

//--------------------------------------------------------------------------------------------------------------------------

bool isValidLocation(const char* location){
    if (location == NULL){
        return false;
    }
    const char* temp = location;
    if (temp[0] < 'A' || temp[0] > 'Z'){
        return false;
    }
    temp ++;
    while (*temp != '\0'){
        if ((*temp >= 'a' && *temp <= 'z') || *temp == ' '){
            temp ++;
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

void setAllScoreTotalMap(ChessSystem chess){

    MAP_FOREACH(int *, key_player, chess->totalplayers){
        int wins=0;
        int losses=0;
        int draws=0;
        Player player = mapGet(chess->totalplayers, key_player);
        freeKeyPlayer(key_player);
        MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
            Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
            freeKeyTournament(tournament_ptr);
            returnPlayerScores(tournament_ptr,key_player, &wins, &losses, &draws);
        }
        setLossesPlayer(player, losses);
        setDrawsPlayer( player, draws);
        setWinsPlayer( player, wins);
    }
}

// bool chessIsValidLocation(ChessSystem chess, int tournament_id){
//     bool res = false;
//     Tournament tournament = mapGet(chess->tournaments, &tournament_id);
//     res = isValidLocation(getTournamentLocation(tournament));
//     //freeDataTournament(tournament);
//     return res;
// }

bool chessTournamentIsActive(ChessSystem chess, int tournament_id){
    bool temp = false;
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    temp = getTournamentIsActive(tournament);
    //freeDataTournament(tournament);
    return temp;
}


void setNumPlayersInTournament(Tournament tournament){
    addTournamentNumPlayers(tournament);
}


int getToatalNumPlayerInSystem(ChessSystem chess){
    int num_total_players = 0;
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        if (tournament_data == NULL){
            return CHESS_OUT_OF_MEMORY;
        }
        num_total_players+= getTournamentNumPlayers(tournament_data);
        freeDataTournament(tournament_ptr); //YUVAL
        //freeDataTournament(tournament_data);
    }
    //free (tournament_ptr);
    return num_total_players;
}


// int getToatalNumGamesInSystem(ChessSystem chess){
//     int num_total_games = 0;
//     MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
//         Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
//         freeKeyTournament(tournament_ptr);
//         if (tournament_data == NULL){
//             return CHESS_OUT_OF_MEMORY;
//         }
//         num_total_games+= getNumGames(tournament_data);
//         freeDataTournament(tournament_ptr); //YUVAL
//     }
//     //free (tournament_ptr);
//     return num_total_games;
// }



void setScoreForLevel(Map total_players, Winner winner, int first_player, int second_player){
    Player player_data_first = mapGet(total_players, &first_player);
    Player player_data_second = mapGet(total_players, &second_player);
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


// void bubbleSortByLevel (Map map){
//     {
//     bool swapped;
//     Player first;
//     Player second = NULL;
//     if (!map){
//         return;
//     }
//     do{
//         swapped = false;
//         first = map->value;
//         while (mapGetNext(first) != second){
//             if (getSaveScoreForLevel(first) < getSaveScoreForLevel(second))
//             {
//                 swap(, ptr1->next);
//                 swapped = 1;
//             }
//             if (ptr1->level == ptr1->next->level)
//             {
//                 if (ptr1->player_id > ptr1->next->player_id)
//                     swap(ptr1, ptr1->next);
//                 swapped = 1;
//             }
//             ptr1 = ptr1->next;
//         }
//         ptr2 = ptr1;
//     } while (swapped);
// }

void setSortedTotalPlayers(ChessSystem chess){
    if (chess == NULL){
        return;
    }
    MAP_FOREACH(int *, key_player, chess->totalplayers){
        Player player = mapGet(chess->totalplayers, key_player);
        freeKeyPlayer(key_player);
        if (player == NULL){
            return;
        }
        mapPut(chess->sortedTotalPlayers, player, player);
        
    }
}

bool noTournamentEnded(ChessSystem chess){
    if (chess==NULL){
        return false;
    }
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        if (tournament_data == NULL){
            return false;
        }
        if (!getTournamentIsActive(tournament_data)){
            return true;
        }
    }
    return false;
}

void setAllScore(ChessSystem chess){
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        setAllTournament(tournament_data);
    }
}


//--------------------------------------------------------------------------------------------------------------------------

ChessSystem chessCreate(){
    ChessSystem new_chess_system = (ChessSystem)malloc(sizeof(*new_chess_system));
    if (new_chess_system == NULL){
        return NULL;
    }
    new_chess_system->tournaments = mapCreate(copyDataTournament,
                                              copyKeyTournament,
                                              freeDataTournament,
                                              freeKeyTournament,
                                              compareIdtournament);
    if (new_chess_system->tournaments == NULL){
        free(new_chess_system);
        return NULL;
    }
    new_chess_system->totalplayers = mapCreate(copyDataPlayer,
                                              copyKeyPlayer,
                                              freeDataPlayer,
                                              freeKeyPlayer,
                                              compareIdPlayer);
    if (new_chess_system->totalplayers == NULL){
        mapDestroy(new_chess_system->tournaments);
        free(new_chess_system);
        return NULL;
    }
    new_chess_system->sortedTotalPlayers = mapCreate(copyDataPlayer,
                                              copyDataPlayer,
                                              freeDataPlayer,
                                              freeDataPlayer,
                                              compareForTotPlayers);
    if (new_chess_system->sortedTotalPlayers == NULL){
        mapDestroy(new_chess_system->tournaments);
        mapDestroy(new_chess_system->totalplayers);
        free(new_chess_system);
        return NULL;
    }
    return new_chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if (chess!=NULL){
        mapDestroy(chess->tournaments);
        mapDestroy(chess->totalplayers);
        mapDestroy(chess->sortedTotalPlayers);
        free(chess);
    }
}


ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
{
    if (chess == NULL || tournament_location == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0){
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->tournaments, &tournament_id)){
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!isValidLocation(tournament_location)){
        return CHESS_INVALID_LOCATION;
    }
    if (max_games_per_player <= 0){
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament tournament = createTournament(tournament_id, max_games_per_player, tournament_location);
    if(tournament == NULL){
        return CHESS_OUT_OF_MEMORY;
    }
    if (mapPut(chess->tournaments, &tournament_id ,tournament) == MAP_SUCCESS){
        freeDataTournament(tournament);
        return CHESS_SUCCESS;
    }
    freeDataTournament(tournament);
    return CHESS_OUT_OF_MEMORY;
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    if (chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (first_player < 0 || second_player < 0 || first_player == second_player || tournament_id <= 0){
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->tournaments, &tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (!chessTournamentIsActive(chess, tournament_id)){
        return CHESS_TOURNAMENT_ENDED;
    }
    KeyGame new_key_game = CreateKeyGame(first_player, second_player);
    if (new_key_game == NULL){
        return CHESS_OUT_OF_MEMORY;
    }
    DataGame new_data_game = CreateDataGame(winner, play_time);
    if (new_data_game == NULL){
        freeKeyGame(new_key_game);
        return CHESS_OUT_OF_MEMORY;
    }
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (getTournamentId(tournament) - tournament_id == 0){
        if(tournamentsGameContain(tournament, new_key_game)){
            freeKeyGame(new_key_game);
            freeDataGame(new_data_game);
            return CHESS_GAME_ALREADY_EXISTS;
        }
    }
    if (play_time < 0){
        freeKeyGame(new_key_game);
        freeDataGame(new_data_game);
        return CHESS_INVALID_PLAY_TIME;
    }
    int first_contain = tournamentPlayerContain(tournament, first_player);
    if (!first_contain){
        Player new_data_player1 = CreatePlayer(first_player);
        if (new_data_player1 == NULL){
            freeKeyGame(new_key_game);
            freeDataGame(new_data_game);
            return CHESS_OUT_OF_MEMORY;
        }
            addPlayersGameToPlayerMap(tournament, first_player, new_data_player1);
            if (!mapContains(chess->totalplayers, &first_player)){
                mapPut(chess->totalplayers, &first_player, new_data_player1);
            }
            setNumPlayersInTournament(tournament);
            freeDataPlayer(new_data_player1);
        }    
    if (!tournamentPlayerContain(tournament, second_player)){
        Player new_data_player2 = CreatePlayer(second_player);
        if (new_data_player2 == NULL){
            if (tournamentPlayerContain(tournament, first_player)==0){
                removePlayersGameToPlayerMap(tournament, first_player);
                mapRemove(chess->totalplayers, &first_player);
                sumTournamentNumPlayers(tournament);
            }
            
            freeKeyGame(new_key_game);
            freeDataGame(new_data_game);
            return CHESS_OUT_OF_MEMORY;
        }
        addPlayersGameToPlayerMap(tournament, second_player, new_data_player2);
        if (!mapContains(chess->totalplayers, &second_player)){
            mapPut(chess->totalplayers, &second_player, new_data_player2);
        }
        setNumPlayersInTournament (tournament);
        freeDataPlayer(new_data_player2);
    }
    if(numGamePlayerLessThenMax(tournament, first_player) && numGamePlayerLessThenMax(tournament, second_player)/* && !contain*/){
        setResultForGame(tournament, winner, first_player, second_player);
        setScoreForLevel(chess->totalplayers, winner, first_player,second_player);
        addPlayersGameToGameMap(tournament, new_key_game, new_data_game);
        freeDataGame(new_data_game);
        freeKeyGame(new_key_game);
    }
    else {
        freeDataGame(new_data_game);
        freeKeyGame(new_key_game);
        return CHESS_EXCEEDED_GAMES;
    }
    mapPut(chess->tournaments, &tournament_id, tournament);
    setAllScore(chess);
    return CHESS_SUCCESS;
 }


ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id){
    if (chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0){
        return CHESS_INVALID_ID;
    }
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    setScoreIntotalplayers(chess->totalplayers, tournament);
    MapResult res = mapRemove(chess->tournaments, &tournament_id);
    if (res == MAP_NULL_ARGUMENT){
        return CHESS_NULL_ARGUMENT;
    }
    if (res == MAP_ITEM_DOES_NOT_EXIST){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}



 ChessResult chessRemovePlayer(ChessSystem chess, int player_id){
     if (chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0){
        return CHESS_INVALID_ID;
    }
    bool contain = false;
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        if (tournamentPlayerContain(tournament_data, player_id)){
            contain = true;
            if (getTournamentIsActive(tournament_data)){
                removePlayerFromPlayerMap(tournament_data, player_id);
                removePlayerFromGameMap(tournament_data, player_id);
            }
        }
    }
    MapResult res = mapRemove(chess->totalplayers, &player_id);
    if (!contain && res == MAP_SUCCESS){
        setAllScore(chess);
        return CHESS_SUCCESS;
        }
    if (!contain){
        return CHESS_PLAYER_NOT_EXIST;
    }
    else{
    setAllScore(chess);
    return CHESS_SUCCESS;
    }
 }


ChessResult chessEndTournament(ChessSystem chess, int tournament_id){
    //printf ("got into fun\n");
    if (chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament_id <= 0){
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->tournaments,&tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament tournament_data = mapGet(chess->tournaments, &tournament_id);
    if (tournament_data == NULL){
        return CHESS_OUT_OF_MEMORY;
    }
    //int id = getTournamentId(tournament_data);
    //int num = getNumGames(tournament_data);
    //printf ("%d\n" , id);
    //printf ("%d\n" , num);
    if(!getTournamentIsActive(tournament_data)){
        return CHESS_TOURNAMENT_ENDED;
    }
    if (tournametIsMapGamesEmpty(tournament_data)){
        return CHESS_NO_GAMES;
    }
    int winner = determaineWinnerInTournament(tournament_data);
    setTournamentWinner(tournament_data, winner);
    setTournamentIsActive(tournament_data, false);
    return CHESS_SUCCESS;
}



double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result){
    if (chess == NULL){
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    if (player_id <= 0){
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    double total_time = 0;
    double total_games = 0;
   // bool contain = false;
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        if (tournament_data == NULL){
            return CHESS_OUT_OF_MEMORY;
        }
        if (tournamentPlayerContain(tournament_data, player_id)){
           // contain = true;
            total_time+= (double)calculatingPlayTimeOfPlayer(tournament_data, player_id);
            total_games+= (double)calculatingNumOfGamesForPlayer(tournament_data, player_id);
        }
    }
    if (!mapContains(chess->totalplayers, &player_id)){
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    *chess_result = CHESS_SUCCESS;
    if (total_games == 0){
        return 0;
    }
    return (total_time/total_games);
}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file){
    if (chess == NULL || file == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (!file){
        return CHESS_SAVE_FAILURE;
    }
    setSortedTotalPlayers(chess);
        MAP_FOREACH(Player, player_ptr, chess->sortedTotalPlayers){
        Player player_data = mapGet(chess->sortedTotalPlayers, player_ptr);
        freeDataPlayer(player_ptr);
        if (player_data == NULL){
            return CHESS_OUT_OF_MEMORY;
        }
        fprintf(file, "%d ", getPlayerId(player_data));
        double level = getSaveScoreForLevel(player_data);
        fprintf(file, "%.2lf", level);
        fprintf(file, "\n");
    }
    return CHESS_SUCCESS;
}


ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file){
    if (chess == NULL || path_file == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    if (!noTournamentEnded(chess)){
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    FILE* file = fopen(path_file,"w+");
    if (file == NULL){
        return CHESS_SAVE_FAILURE;
    }
    MAP_FOREACH(int *, tournament_ptr, chess->tournaments){
        Tournament tournament_data = mapGet(chess->tournaments, tournament_ptr);
        freeKeyTournament(tournament_ptr);
        if (tournament_data == NULL){
            fclose(file);
            return CHESS_OUT_OF_MEMORY;
        }
        // if (getNumGames(tournament_data)==0){
        //     continue;
        // }
        if (!getTournamentIsActive(tournament_data)){
            SaveTournamentStatistics(tournament_data, file);
        }
    }
    fclose(file);
    return CHESS_SUCCESS;
}

