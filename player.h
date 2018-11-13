//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_PLAYER_H
#define SIMULATION_PLAYER_H

#include "Database/DBInterface.h"
#include "AdvStats.h"
#include <ctime>
#include <string>
#include <iostream>

class Player {
private:
    int playerID;
	int teamID; // If you want to restrict data pulled to a player's games for a certain team (for players that have played on multiple teams)

    struct tm * startDate;
    struct tm * endDate;

    DBInterface * db; // Pointer to DBInterface, which exposes connection and pre-written DB access functions

	AdvStats * stats;

	// Percentages of how often a player finishes a team play (based on the 5 types of things that can happen - some offensive & some defensive)
	// **Replace with AdvStats object**
    double shootPct;
	double shoot3PtPct;
    double tovPct;
    double drawFoulPct;
    double drebPct;
    double orebPct;

    // Used to set start date and end date
    void setDate(struct tm *, int, int, int);

// Functions to get all data needed to calc player advanced stats (USG (or a breakdown thereof), DRB%, ORB%)
// Not using usage, as it's really a combination of a few things. Will break down into component parts
// USG Formula: ((FGA + 0.44*FTA + TOV)*(Tm MP * 5)) / (MP *(Tm FGA + 0.44 * Tm FTA + Tm TOV))

// TODO: think through scenarios where a player changes teams. Each function should be able to handle that

	// Private functions to get data from DB, for purpose fo filling private variables
    int getTotalFromDB(std::string category,
					   std::vector<std::string> * constraintNames,
					   std::vector<std::string> * constraintVals);
	
public:
	// Constructors
	Player(DBInterface * db);
	Player(DBInterface * db, int pID);
	Player(DBInterface * db, int pID, struct tm * startDate, struct tm * endDate);

	// Destructor
    ~Player();

	void calcPriors(); // this will contain all the 'business logic' of how to calculate advanced stats that I need.
    // It will make calls to underlying functions

    // Getters
    double getShootPct();
    double getTovPct();
    double getDrawFoulPct(); // TODO: consider whether I want FT attempts or just a count of how often player is fouled or some combo
    double getDrebPct();
    double getOrebPct();

    // Setters
    void setPID(int);
	void setTeamID(int);
	void setStartDate(int, int, int);
	void setEndDate(int, int, int);
		
};

#endif //SIMULATION_PLAYER_H
