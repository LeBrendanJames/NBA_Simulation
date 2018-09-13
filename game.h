//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"

class Game {
private:
	Lineup * homeLineup;
	Lineup * awayLineup;
	// Coach * homeCoach;
	// Coach * awayCoach;
	
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	int timeRemaining; // Seconds left in quarter 
	
	void determinePossOutcome(); // Function to take in all info and determine the outcome of a possession
								 // This will rely on the underlying lineups and eventually coaches + game state + meta-game considerations (home/away, travel, etc.) 
	
	void determineInterPossessionChanges(); // Determine if anything happens between possessions (mainly substitutions)
	
	void updateTime();
	
public:
	Game();
	Game(Lineup *, Lineup *)
	~Game();
	
	int simGame(); // Manage game state, calling determinePossOutcome() for each possession
				   // Returns homeTeamScore - awayTeamScore at end 
	
};

#endif //SIMULATION_GAME_H
