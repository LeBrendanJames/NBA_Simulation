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
	AdvStats ** stats; // to hold each players expected stats in this lineup

	void calcLineupStats();
	
public:
	Lineup(Player *, Player *, Player *, Player *, Player *);
	~Lineup();

	// Getters
	double getShotFreq(int lineupPosition);
	double getShot3PtFreq(int lineupPosition);
	double getTovFreq(int lineupPosition);
	double getDrawFoulFreq(int lineupPosition);
	double getDrebFreq(int lineupPosition);
	double getOrebFreq(int lineupPosition);
	double getStlFreq(int lineupPosition);
	double getBlkFreq(int lineupPosition);
	int getNumOffPlays(int lineupPosition);
	int getNumDefPlays(int lineupPosition);

	double getShotPct(int lineupPosition);
	double getThreePtPct(int lineupPosition);
		
	//void calcLineupStats(Lineup * oppLineup); // Calc stats, which get placed in each individual players stat categories
	//void subPlayers(Player * playerOn, Player * playerOff); // Changes pointers in lineup variable and re-calcs all of the lineup stats based on new lineup
	
};


#endif //SIMULATION_LINEUP_H
