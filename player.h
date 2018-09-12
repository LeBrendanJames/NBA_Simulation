//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_PLAYER_H
#define SIMULATION_PLAYER_H

#include "database.h"
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

	// Percentages of how often a player finishes a team play (based on the 5 types of things that can happen - some offensive & some defensive)
    double shootPct;
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
// TODO: Design a 'DBRequest' object that has all the possible request parameters

	// Private functions to get data from DB, for purpose fo filling private variables
    int getFGAFromDB();


/*bool getPlayerMins(); // Get total time player was on court
bool getTeamMins(); // Get total team time (in date range)
bool getPlayerFGA();
bool getPlayerFTA(); // TODO: consider whether I want FT attempts or just a count of how often player is fouled or some combo
bool getPlayerTOV();
bool getTeamFGA();
bool getTeamFTA();
bool getTeamTOV();*/
	
public:
	// Constructors
	Player(DBInterface * db);
	Player(DBInterface * db, int pID);
	Player(DBInterface * db, int pID, struct tm * startDate, struct tm * endDate);

	// Destructor
    ~Player();

	void calcStats(); // this will contain all the 'business logic' of how to calculate advanced stats that I need.
    // It will make calls to underlying functions

    // Getters
    double getShootPct();
    double getTovPct();
    double getDrawFoulPct();
    double getDrebPct();
    double getOrebPct();

    // Setters
    void setPID(int);
	void setTeamID(int);
	void setStartDate(int, int, int);
	void setEndDate(int, int, int);
		
};

#endif //SIMULATION_PLAYER_H
