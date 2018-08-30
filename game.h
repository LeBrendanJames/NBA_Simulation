//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_GAME_H
#define SIMULATION_GAME_H

#include "lineup.h"
#include "coach.h"

Class Game {
	Lineup * homeTeam;
	Lineup * awayTeam;
	Coach * homeCoach;
	Coach * awayCoach;
	
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	// time left in quarter variable
	
};

#endif //SIMULATION_GAME_H
