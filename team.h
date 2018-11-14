//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_TEAM_H
#define SIMULATION_TEAM_H

class Team {
public:
	Team();
	~Team();
	
	// Rotation functions here? These are partially determined by coach and partially by team, so it should fit in team?
	// Can contain a bunch of stuff around schemes, maybe?
	
	// subPlayers(); // Here?
	
private:
	Player ** players;
	Coach * coach;
};


#endif //SIMULATION_TEAM_H
