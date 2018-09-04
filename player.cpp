//
// Created by Brendan on 6/26/2018.
//

#include "player.h"



void setDate(struct tm * date, int day, int month, int year){
	date->tm_mday = day; // Day
	date->tm_mon = month - 1; // Months since January
	date->tm_year = year - 1900; // Years since 1900

	return;
}


// Default constructor
Player::Player(DBInterface * db){
	this->db = db;
	
	playerID = 0;
	
	startDate = new struct tm;
    setDate(startDate, 1, 1, 1970);
	
	endDate = new struct tm;
	setDate(endDate, 1, 1, 3000);
	
	
}

Player::Player(DBInterface * db, int pID){
	this->db = db;
	playerID = pID;
	
	startDate = new struct tm;
    setDate(startDate, 1, 1, 1970);
	
	endDate = new struct tm;
	setDate(endDate, 1, 1, 3000);
}

Player::Player(DBInterface * db, int pID, struct tm * startDate, struct tm * endDate){
	this->db = db;
	playerID = pID;
	this->startDate = startDate;
	this->endDate = endDate;
	
	// **Maybe chaeck that the passed in start and end date are valid??**
}





// Wrapper function that fills all stat data members of player object
void calcStats(){
	
	shootPct = calcStat('shootPct');
	tovPct = calcStat('tovPct');
	drawFoulPct = calcStat('drawFoulPct');
	drebPct = calcStat('drebPct');
	orebPct = calcStat('orebPct');
	
	return;
}

float calcStat(std::string * statName){
	float pctStat = 0.0;
	
	// Build DBReq (just the start date/ end date/playerID)
	// The rest will be built at the database level, since that level will know how to build a request to get the data needed by this function
	DBReq * req = new DBReq(pID, startDate, endDate);
	DBRes * res = new DBRes();
	
	// Send req, res, and statName to dbInterface object
	db->getStatFromDB(req, res, statName);
	
	// Add stat to correct variable
	pctStat = res->singleStat;
	
	// Clear allocated memory
	delete req;
	delete res;
	
	return pctStat;
}



// GETTERS
float getShootPct(){
	return shootPct;
}

float getTovPct(){
	return tovPct;
}

float getDrawFoulPct(){
	return drawFoulPct;
}

float getDrebPct(){
	return drebPct;
}

float getOrebPct(){
	return orebPct;
}
