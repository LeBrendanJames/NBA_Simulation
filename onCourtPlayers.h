//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_ON_COURT_PLAYERS_H
#define SIMULATION_ON_COURT_PLAYERS_H

#include "lineup.h"


class OnCourtPlayers {
public:
	OnCourtPlayers(Lineup * homeLineup, Lineup * awayLineup);
	~OnCourtPlayers();

	void changeOffensiveTeam();
	
	void calcStats();
	// void subPlayers(Player * playerOn, Player * playerOff, bool home); // Changes pointers in lineup array and re-calcs all of the lineup (team & opponent) stats based on new lineup 

private:
	Lineup * homeLineup;
	Lineup * awayLineup;
	
	// To hold current stats for home players (players hold their priors, but actual projected stats, 
	// taking into account everything known at this point, are placed in here when calculated)
	AdvStats ** homeStats; 
	AdvStats ** awayStats;

	int offLineup; // Which team is on offense (1 = home, 0 = away)
};


#endif //SIMULATION_ON_COURT_PLAYERS_H