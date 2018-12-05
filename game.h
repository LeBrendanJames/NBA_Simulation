//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"
#include "GameState.h"
#include <string>


class Game {
public:
	Game();
	Game(std::string homeTeam, std::string awayTeam);
	Game(DBInterface * dbFace, std::string homeTeam, std::string awayTeam, std::string gameDate, std::string gameLoc);
	~Game();
	
	void simGame(); // Manage game state, calling determinePossOutcome() for each possession
	
	int getHomeScore();
	int getAwayScore();
				   
private:
	DBInterface * db;
	Team * homeTeam;
	Team * awayTeam;
	OnCourtPlayers * onCourtPlyrs;
	GameState * gmState; // To hold all the below info about scores/time remaining/etc.
	
	
	void determineStatExpectations(); // **Makes use of both lineups and game state? Should be called for each sub but also at predetermined times in game as well?**
	void determinePossOutcome(); // Function to take in all info and determine the outcome of a possession
								 // This will rely on the underlying lineups and eventually coaches + game state + meta-game considerations (home/away, travel, etc.) 
	void determineInterPossessionChanges(); // Determine if anything happens between possessions (mainly substitutions), then call determineStatExpectations to reset stats
	// void subPlayers(); // **Should this go here or in team?**
	void updateTime();
};


#endif //SIMULATION_GAME_H