//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_TEAM_H
#define SIMULATION_TEAM_H

#include "database/DBInterface.h"

class Team {
public:
	Team();
	Team(DBInterface * dbFace, std::string teamCode, std::string gameDate);
	~Team();
	
	// Rotation functions here? These are partially determined by coach and partially by team, so it should fit in team?
	// Can contain a bunch of stuff around schemes, maybe?
	
	// subPlayers(); // Here?
	
private:
	DBInterface * dbFace;
	Player ** players;
	Coach * coach;
};


#endif //SIMULATION_TEAM_H
