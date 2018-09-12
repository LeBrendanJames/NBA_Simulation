//
// Created by Brendan on 6/26/2018.
//

#include "player.h"



void Player::setDate(struct tm * date, int day, int month, int year){
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
	
	// **Maybe check that the passed in start and end date are valid??**
}

Player::~Player(){

}



int Player::getFGAFromDB() {
    // Create DBReq
    DBReq * req = new DBReq;
    req->addPID(this->playerID);
	// Additional constraints

    // Create DBRes
    DBRes * res = new DBRes;

    // Call getFGA from DBInterface
    bool dbResult = this->db->getFGA(req, res);
	if (!dbResult){
		// Throw error & return -1?
	}

    // get Player Res from res object
    PlayerRes * plyrRes = new PlayerRes;
    res->getPlayerRes(plyrRes, 0);
	int retVal = static_cast<int>(plyrRes->getResVal(0))
	
	// Delete plyrRes
	delete plyrRes;
	
	// Delete req & res
	delete req;
	delete res;

    // Return value from DBRes
	return retVal;
    //return static_cast<int>(plyrRes->getResVal(0));
}


// Wrapper function that fills all stat data members of player object
void Player::calcStats(){

    int FGA = getFGAFromDB();
	if (FGA == -1){
		// Error handle
	}
    //int FGM = getFGMFromDB();

    // **FOR TESTING. Obviously FGA != shootPct**
    this->shootPct = static_cast<double>(FGA);
}



// GETTERS
double Player::getShootPct(){
	return shootPct;
}

double Player::getTovPct(){
	return tovPct;
}

double Player::getDrawFoulPct(){
	return drawFoulPct;
}

double Player::getDrebPct(){
	return drebPct;
}

double Player::getOrebPct(){
	return orebPct;
}


// SETTERS
void Player::setPID(int newPID){
    playerID = newPID;
}

void Player::setTeamID(int teamID){
	this->teamID = teamID;
}

void Player::setStartDate(int day, int month, int year){
	setDate(this->startDate, day, month, year);
}

void Player::setEndDate(int day, int month, int year){
	setDate(this->endDate, day, month, year);
}
