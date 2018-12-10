//
// Created by Brendan on 6/26/2018.
//

#include "player.h"


// Constructor
Player::Player(DBInterface * db, int pID, GameState * gmState){
	this->db = db;
	this->playerID = pID;

	plyrStats = new AdvStats;
	calcPriors(gmState);
}

// Destructor
Player::~Player(){
	delete plyrStats;
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

    //std::cout << "About to get data from DB." << std::endl;

    // Call getFGA from DBInterface
    bool dbResult = this->db->getDataFromDB(req, res);
	if (!dbResult){
		// Throw error & return -1?
	}

	//std::cout << "About to create temp playerRes." << std::endl;

    // get Player Res from res object
    PlayerRes * plyrRes = new PlayerRes;
    res->getPlayerRes(plyrRes, 0);
    //std::cout << "through getPlayerRes." << std::endl;
    //std::cout << "resVals size: " << plyrRes->getResValsSize() << std::endl;
	int retVal = static_cast<int>(plyrRes->getResVal(0));

	//std::cout << "retVal in player.getTotalFromDB: " << retVal << std::endl;

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
void Player::calcPriors(GameState * gmState){

	auto * constraintNames = new std::vector<std::string>;
	auto * constraintVals = new std::vector<std::string>;

	constraintNames->push_back("endDate");
	constraintVals->push_back(gmState->getGameDate());

	// constraintNames->push_back("playerOnCourt");
	// constraintVals->push_back("462"); // Trevor Ariza = 462, if I'm testing player stuff
	// constraintNames->push_back("playerOffCourt");
	// constraintVals->push_back("413"); // Ryan Anderson
	constraintNames->push_back("normalPoss");
	constraintVals->push_back("0");

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
	
	// Actual Stats (into AdvStats object)
	plyrStats->setShotFreq(FGA / static_cast<double>(offPlays));
	plyrStats->setShot3PtFreq(ThreePA / static_cast<double>(offPlays));
	plyrStats->setTovFreq(TOV / static_cast<double>(offPlays));
	plyrStats->setDrawFoulFreq(drawnFouls / static_cast<double>(offPlays));
	plyrStats->setDrebFreq(DREB / static_cast<double>(defPlays));
	plyrStats->setOrebFreq(OREB / static_cast<double>(offPlays));
	plyrStats->setStlFreq(Steals / static_cast<double>(defPlays));
	plyrStats->setBlkFreq(Blocks / static_cast<double>(defPlays));
	plyrStats->setNumOffPlays(offPlays);
	plyrStats->setNumDefPlays(defPlays);
	plyrStats->setShotPct(FGM / static_cast<double>(FGA));
	plyrStats->setThreePtPct(ThreePM / static_cast<double>(ThreePA));
}



// GETTERS
double Player::getShotFreq(){
	return plyrStats->getShotFreq();
}

double Player::getShot3PtFreq(){
		return plyrStats->getShot3PtFreq();
}

double Player::getTovFreq(){
	return plyrStats->getTovFreq();
}

double Player::getDrawFoulFreq(){
	return plyrStats->getDrawFoulFreq();
}

double Player::getDrebFreq(){
	return plyrStats->getDrebFreq();
}

double Player::getOrebFreq(){
	return plyrStats->getOrebFreq();
}

double Player::getStlFreq(){
	return plyrStats->getStlFreq();
}

double Player::getBlkFreq(){
	return plyrStats->getBlkFreq();
}

int Player::getNumOffPlays() {
	return plyrStats->getNumOffPlays();
}

int Player::getNumDefPlays() {
	return plyrStats->getNumDefPlays();
}

double Player::getShotPct(){
	return plyrStats->getShotPct();
}

double Player::getThreePtPct(){
	return plyrStats->getThreePtPct();
}


// SETTERS
void Player::setPID(int newPID){
    playerID = newPID;
}