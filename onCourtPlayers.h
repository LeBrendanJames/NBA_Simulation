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
		
		AdvStats ** homeStats; // To hold current stats for home players (players hold their priors, but actual stats are places in here when calculated)
		AdvStats ** awayStats;
	
	public:
		OnCountPlayers(Player **, Player **);
		~OnCourtPlayers();
		
		void calcStats();
		//void subPlayers(Player * playerOn, Player * playerOff); // Changes pointers in lineup variable and re-calcs all of the lineup stats based on new lineup 
	
};


#endif //SIMULATION_ON_COURT_PLAYERS_H