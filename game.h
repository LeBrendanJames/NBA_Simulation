//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"

class Game {
private:
	Lineup * homeTeam;
	Lineup * awayTeam;
	// Coach * homeCoach;
	// Coach * awayCoach;
	
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	struct tm * timeRemaining;
	
	void determinePossOutcome(); // Function to take in all info and determine the outcome of a possession
								 // This will rely on the underlying lineups and eventually coaches + game state + meta-game considerations (home/away, travel, etc.) 
	
	void determineInterPossessionChanges(); // Determine if anything happens between possessions (mainly substitutions)
	
	void updateTime();
	
public:
	Game();
	Game(Lineup *, Lineup *)
	~Game();
	
	void simGame(); // Manage game state, calling determinePossOutcome() for each possession
					// This will mainly be responsible for keeping time?
	
};

#endif //SIMULATION_GAME_H
