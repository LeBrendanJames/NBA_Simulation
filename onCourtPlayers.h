//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_ON_COURT_PLAYERS_H
#define SIMULATION_ON_COURT_PLAYERS_H

#include "lineup.h"
#include "GameState.h"


class OnCourtPlayers {
public:
	OnCourtPlayers(Lineup * homeLineup, Lineup * awayLineup);
	OnCourtPlayers(const OnCourtPlayers &onCourtPlyrs);
	~OnCourtPlayers();
	
	void calcStats();
	// void subPlayers(Player * playerOn, Player * playerOff, bool home); // Changes pointers in lineup array and re-calcs all of the lineup (team & opponent) stats based on new lineup 

    double getTotalFrequencies(GameState * gmState);
    double getTotalShotFreq(GameState * gmState);
    double getFGPct(GameState * gmState);
    double getTotalTOVFreq(GameState * gmState);

private:
	Lineup * homeLineup;
	Lineup * awayLineup;
	
	// To hold current stats for home players (players hold their priors, but actual projected stats, 
	// taking into account everything known at this point, are placed in here when calculated)
	AdvStats ** homeStats; 
	AdvStats ** awayStats;

	void setAdvStats(AdvStats ** stats, Lineup * lineup, int lineupPosition);
	void setOffTeamPtr(AdvStats *** statsPtr, GameState * gmState);
};


#endif //SIMULATION_ON_COURT_PLAYERS_H