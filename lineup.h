//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_LINEUP_H
#define SIMULATION_LINEUP_H

#include "player.h"

// this one should contain a bunch of the methods to figure out what happens based on who is on the floor?

class Lineup {
	private:
		Player ** lineup;
	
	public:
		Lineup(Player *, Player *, Player *, Player *, Player *);
		~Lineup();
		
		//void calcLineupStats(); // Calc stats, which get placed in each individual players stat categories
		//void subPlayers(Player * playerOn, Player * playerOff); // Changes pointers in lineup variable and re-calcs all of the lineup stats based on new lineup 
	
};


#endif //SIMULATION_LINEUP_H
