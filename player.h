//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_PLAYER_H
#define SIMULATION_PLAYER_H

#include "database.h"
#include "dbReq.h"
#include <ctime>
#include <string>

Class Player {
	private:
		// Constructors
		Player(dbInterface * db);
		Player(dbInterface * db, int pID);
		Player(dbInterface * db, int pID, struct tm * startDate, struct tm * endDate);
	
		int playerID;
		
		struct tm * startDate;
		struct tm * endDate;
		
		dbInterface * db;
	
		float shootPct;
		float tovPct;
		float drawFoulPct;
		float drebPct;
		float orebPct;
		
		// Used to set start date and end date 
		void setTime(struct tm *, int, int, int);
		
// Functions to get all data needed to calc player advanced stats (USG (or a breakdown thereof), DRB%, ORB%)
	// Not using usage, as it's really a combination of a few things. Will break down into component parts
	// USG Formula: ((FGA + 0.44*FTA + TOV)*(Tm MP * 5)) / (MP *(Tm FGA + 0.44 * Tm FTA + Tm TOV))
	
	// TODO: think through scenarios where a player changes teams. Each function should be able to handle that 
	// TODO: Design a 'DBRequest' object that has all the possible request parameters
	
	void calcStats(); // this will contain all the 'business logic' of how to calculate advanced stats that I need. 
					  // It will make calls to underlying functions
	
	
	/*bool getPlayerMins(); // Get total time player was on court
	bool getTeamMins(); // Get total team time (in date range)
	bool getPlayerFGA();
	bool getPlayerFTA(); // TODO: consider whether I want FT attempts or just a count of how often player is fouled or some combo
	bool getPlayerTOV();
	bool getTeamFGA();
	bool getTeamFTA();
	bool getTeamTOV();*/
	
	
	
	public:
		float getShootPct();
		float getTovPct();
		float getDrawFoulPct();
		float getDrebPct();
		float getOrebPct();
		
};

#endif //SIMULATION_PLAYER_H
