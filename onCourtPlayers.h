//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_ON_COURT_PLAYERS_H
#define SIMULATION_ON_COURT_PLAYERS_H

#include "player.h"


class OnCourtPlayers {
	private:
		Player ** homeLineup;
		Player ** awayLineup;
		
		// To hold current stats for home players (players hold their priors, but actual projected stats, 
		// taking into account everything known at this point, are placed in here when calculated)
		AdvStats ** homeStats; 
		AdvStats ** awayStats;
	
	public:
		OnCountPlayers(Player **, Player **);
		~OnCourtPlayers();
		
		void calcStats();
		// void subPlayers(Player * playerOn, Player * playerOff, bool home); // Changes pointers in lineup array and re-calcs all of the lineup (team & opponent) stats based on new lineup 
	
};


#endif //SIMULATION_ON_COURT_PLAYERS_H