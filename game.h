//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"
#include "GameState.h"
#include "team.h"
#include "onCourtPlayers.h"
#include <string>


class Game {
public:
	Game();
	Game(DBInterface * dbFace, std::string homeTeam, std::string awayTeam, std::string gameDate, std::string gameLoc);
	Game(const Game &game);
	~Game();
	
	void simGame(); // Manage game state, calling determinePossOutcome() for each possession
	
	int getHomeScore();
	int getAwayScore();
				   
private:
	DBInterface * dbFace;
	Team * homeTeam;
	Team * awayTeam;
	OnCourtPlayers * onCourtPlyrs;
	GameState * gmState;
	
	
	void determinePossOutcome(); // Function to take in all info and determine the outcome of a possession
								 // This will rely on the underlying lineups and eventually coaches + game state + meta-game considerations (home/away, travel, etc.) 
	void determineInterPossChgs(); // Determine if anything happens between possessions (mainly substitutions), then call determineStatExpectations to reset stats
	// void subPlayers(); // **Should this go here or in team?**
	void updateTime();
};


#endif //SIMULATION_GAME_H