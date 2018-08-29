//
// Created by Brendan on 8/13/2018.
//

// Wrapper functions for database interaction
// that return a bool (true if they succeed, false otherwise)

#ifndef SIMULATION_DATABASE_H
#define SIMULATION_DATABASE_H

#include <libpq-fe.h>
#include <ctime>
#include <string>

// Connection open 

// Base function through which all SELECT requests are sent 
// This is the only function that directly interacts with the database. All other functions build DBReq objects and pass them to 'getDataFromDB'
// **This function allocates memory to the passed in DBRes pointer, which needs to be deleted by the calling function**
bool getDataFromDB(PGconn * conn, DBReq * req, DBRes * res); 

/*bool computePlayerAdvStats(); // **Should go in player class???**


bool getPlayerTotalPoints(PGconn * conn, char * resultString, int playerID, struct tm * startDate, struct tm * endDate);
bool getTeamPossessionCount(PGconn * conn, char * resultString, std::string teamName, struct tm * startDate, struct tm * endDate);*/


// Functions to get all data needed to calc player advanced stats (USG (or a breakdown thereof), DRB%, ORB%)
	// Not using usage, as it's really a combination of a few things. Will break down into component parts
	// USG Formula: ((FGA + 0.44*FTA + TOV)*(Tm MP * 5)) / (MP *(Tm FGA + 0.44 * Tm FTA + Tm TOV))
	
	// TODO: think through scenarios where a player changes teams. Each function should be able to handle that 
	// TODO: Design a 'DBRequest' object that has all the possible request parameters
	
	// TODO: Move all of these to 'player' object, which will #include "database.h"
	/*bool getPlayerMins(); // Get total time player was on court
	bool getTeamMins(); // Get total team time (in date range)
	bool getPlayerFGA();
	bool getPlayerFTA(); // TODO: consider whether I want FT attempts or just a count of how often player is fouled or some combo
	bool getPlayerTOV();
	bool getTeamFGA();
	bool getTeamFTA();
	bool getTeamTOV();*/

#endif //SIMULATION_DATABASE_H
