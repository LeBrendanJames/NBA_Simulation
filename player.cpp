//
// Created by Brendan on 6/26/2018.
//

#include "player.h"

void Player::setDate(struct tm * date, int day, int month, int year){
	date->tm_mday = day; // Day
	date->tm_mon = month - 1; // Months since January
	date->tm_year = year - 1900; // Years since 1900
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

	stats = new AdvStats;
}

Player::Player(DBInterface * db, int pID, struct tm * startDate, struct tm * endDate){
	this->db = db;
	playerID = pID;
	this->startDate = startDate;
	this->endDate = endDate;

	stats = new AdvStats;
	
	// **Maybe check that the passed in start and end date are valid??**
}

Player::~Player(){
	delete stats;
}



int Player::getTotalFromDB(std::string category,
						   std::vector<std::string> * constraintNames,
						   std::vector<std::string> * constraintVals) {
    // Create DBReq
	// std::vector<std::string> * constraintNames = new std::vector<std::string>;
	// std::vector<std::string> * constraintVals = new std::vector<std::string>;
    DBReq * req = new DBReq(this->playerID, category, constraintNames, constraintVals);
    // req->addPID(this->playerID);
    // req->addCategory(category);
	// Additional constraints

    // Create DBRes
    DBRes * res = new DBRes;

    std::cout << "About to get data from DB." << std::endl;

    // Call getFGA from DBInterface
    bool dbResult = this->db->getDataFromDB(req, res);
	if (!dbResult){
		// Throw error & return -1?
	}

	std::cout << "About to create temp playerRes." << std::endl;

    // get Player Res from res object
    PlayerRes * plyrRes = new PlayerRes;
    res->getPlayerRes(plyrRes, 0);
    std::cout << "through getPlayerRes." << std::endl;
    std::cout << "resVals size: " << plyrRes->getResValsSize() << std::endl;
	int retVal = static_cast<int>(plyrRes->getResVal(0));

	std::cout << "retVal in player.getTotalFromDB: " << retVal << std::endl;

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
void Player::calcPriors(){ // Should take a game date?

	auto * constraintNames = new std::vector<std::string>;
	auto * constraintVals = new std::vector<std::string>;
	// constraintNames->push_back("playerOnCourt");
	// constraintVals->push_back("462"); // Trevor Ariza = 462, if I'm testing player stuff
	// constraintNames->push_back("playerOffCourt");
	// constraintVals->push_back("413"); // Ryan Anderson
	// constraintNames->push_back("normalPoss");
	// constraintVals->push_back("0");

    //int FGA = getFGAFromDB();
    int FGA = getTotalFromDB("FGA", constraintNames, constraintVals);
	if (FGA == -1){
		// Error handle
	}
    int FGM = getTotalFromDB("FGM", constraintNames, constraintVals);
	int ThreePA = getTotalFromDB("3PA", constraintNames, constraintVals);
	int ThreePM = getTotalFromDB("3PM", constraintNames, constraintVals);
	int DREB = getTotalFromDB("DREB", constraintNames, constraintVals);
	int OREB = getTotalFromDB("OREB", constraintNames, constraintVals);
	int Steals = getTotalFromDB("Steals", constraintNames, constraintVals);
	int Blocks = getTotalFromDB("Blocks", constraintNames, constraintVals);
	int TOV = getTotalFromDB("Turnovers", constraintNames, constraintVals);
	int drawnFouls = getTotalFromDB("DrawnFouls", constraintNames, constraintVals);
	
	int offPlays = getTotalFromDB("offPlays", constraintNames, constraintVals);
	int defPlays = getTotalFromDB("defPlays", constraintNames, constraintVals);
	
	// Actual stats
	this->shootPct = FGA / static_cast<double>(offPlays);
	this->shoot3PtPct = ThreePA / static_cast<double>(offPlays);
	this->tovPct = TOV / static_cast<double>(offPlays);
	this->drawFoulPct = drawnFouls / static_cast<double>(offPlays);
	this->drebPct = DREB / static_cast<double>(defPlays);
	this->orebPct = OREB / static_cast<double>(offPlays);
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
