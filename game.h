//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"


class Game {
private:
	Team * homeTeam;
	Team * awayTeam;
	OnCourtPlayers * onCourtPlyrs;
	// Lineup * homeLineup;
	// Lineup * awayLineup;
	// Coach * homeCoach; // holds substitution patterns?
	// Coach * awayCoach;
	
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	int timeRemaining; // Seconds left in quarter 
	
	// gameDate?
	// gameLocation?
	
	void determinePossOutcome(); // Function to take in all info and determine the outcome of a possession
								 // This will rely on the underlying lineups and eventually coaches + game state + meta-game considerations (home/away, travel, etc.) 
	
	void determineInterPossessionChanges(); // Determine if anything happens between possessions (mainly substitutions)
	// void subPlayers(); // **Should this go here or in team?**
	
	void updateTime();
	
public:
	Game();
	Game(std::string homeTeam, std::string awayTeam);
	~Game();
	
	int simGame(); // Manage game state, calling determinePossOutcome() for each possession
				   // Returns homeTeamScore - awayTeamScore at end 
	
};


#endif //SIMULATION_GAME_H