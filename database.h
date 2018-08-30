//
// Created by Brendan on 8/13/2018.
//

// Wrapper functions for database interaction
// that return a bool (true if they succeed, false otherwise)

#ifndef SIMULATION_DATABASE_H
#define SIMULATION_DATABASE_H

#include <libpq-fe.h>
#include <string>

Class DBInterface{

	// Connection open 

	// Base function through which all SELECT requests are sent 
	// This is the only function that directly interacts with the database. All other functions build DBReq objects and pass them to 'getDataFromDB'
	// **This function allocates memory to the passed in DBRes pointer, which needs to be deleted by the calling function**
	bool getDataFromDB(PGconn * conn, DBReq * req, DBRes * res);

	bool getPossessionCount(DBReq * req, DBRes * res); // Will take an array of player ID's so this will return possessions for which all players were on the court (for the same team)
	bool getFGA(DBReq * req, DBRes * res);
	bool getFGM(DBReq * req, DBRes * res);
	bool get3FGM(DBReq * req, DBRes * res);
	bool getDREB(DBReq * req, DBRes * res);
	// etc...


	bool getPlayerTotalPoints(PGconn * conn, char * resultString, int playerID, struct tm * startDate, struct tm * endDate);
	bool getTeamPossessionCount(PGconn * conn, char * resultString, std::string teamName, struct tm * startDate, struct tm * endDate);*/

};

#endif //SIMULATION_DATABASE_H
