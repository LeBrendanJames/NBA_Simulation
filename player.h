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
public:
	// Constructor/destructor
	Player(DBInterface * db, int pID);
    ~Player();

	void calcPriors(); // this will contain all the 'business logic' of how to calculate advanced stats that I need.
    // It will make calls to underlying functions
	// TODO: think through scenarios where a player changes teams. Each function should be able to handle that

    // Getters
    double getShotFreq();
	double getShot3PtFreq();
	double getTovFreq();
	double getDrawFoulFreq(); // TODO: consider whether I want FT attempts or just a count of how often player is fouled or some combo
	double getDrebFreq();
	double getOrebFreq();
	double getStlFreq();
	double getBlkFreq();
	int getNumOffPlays();
	int getNumDefPlays();
	
private:
	DBInterface * db; // Pointer to DBInterface, which exposes connection and pre-written DB access functions
    int playerID;
	AdvStats * plyrStats; // Holds stats that will be used directly or used as a prior for stat calculations at higher levels of code (lineup, game, etc.)

	// Private function to get data from DB, for purpose of filling private variables
	// Handles details of building a database request and parsing the response
    int getTotalFromDB(std::string category,
					   std::vector<std::string> * constraintNames,
					   std::vector<std::string> * constraintVals);
};

#endif //SIMULATION_PLAYER_H
