//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
#include <string>
#include <iostream>


// Constraint constructor  
Constraint::Constraint(){
	constraintDate = new struct tm *; // Allocate struct tm memory 
}

// Constraint destructor
Constraint::~Constraint(){
	delete constraintDate; // Free memory
}

// GET constraintType 
std::string Constraint::getConstraintType(){
	return this->constraintType;
}

// GET constraintNum
int Constraint::getConstraintNum(){
	return this->constraintNum;
}

// GET constraintDate 
void Constraint::getConstraintDate(struct tm * date){
	date->tm_mday = this->constraintDate->tm_day; // Day
    date->tm_mon = this->constraintDate->tm_mon; // Months since January
    date->tm_year = this->constraintDate->tm_year; // Years since 1900
	
	return;
}

// SET constraint **Does struct tm have a copy constructor? Can I just do constraintDate = this->constraintDate;?**
void setConstraint(std::string constraintType, int constraintNum, struct tm * constraintDate){
	this->constraintType = constraintType;
	this->constraintNum = constraintNum;
	this->constraintDate->tm_day = constraintDate->tm_day;
	this->constraintDate->tm_mon = constraintDate->tm_mon;
	this->constraintDate->tm_year = constraintDate->tm_year;
	
	return;
}


// DBReq constructor
DBReq::DBReq(){
	
}

// DBReq destructor 
DBReq::~DBReq(){
	
}

// GET numPIDs
int DBReq::getNumPIDs(){
	return pIDs.size();
}

// GET numCategories
int DBReq::getNumCategories(){
	return categories.size();
}

// GET numConstraints
int DBReq::getNumConstraints(){
	return constraints.size();
}

// GET PID
int DBReq::getPID(int playerNum){
    return pIDs[playerNum];
}

// GET Category
std::string DBReq::getCategory(int categoryNum){
	return categories[categoryNum];
}

// GET Constraint
void DBReq::getConstraint(Constraint * constraint, int constraintNum){
	struct tm * tempDate = new struct tm;
	this->constraints[constraintNum]->getconstraintDate(tempDate); // Set tempDate = constraintDate 
	
	constraint->setConstraint(this->constraints[constraintNum]->getConstraintType(), this->constraints[constraintNum]->getConstraintNum(), tempDate)
	
	return;
}

// SET PID 
void DBReq::addPID(int newPID){
	pIDs.push_back(newPID);
	
	return;
}

// SET Category 
void DBReq::addCategory(std::string newCategory){
	categories.push_back(newCategory);
	
	return;
}

// SET Constraint 
void DBReq::addConstraint(std::string newConstraintName, int newConstraintNum, struct tm * newConstraintDate){
	Constraint * newConstraint = new Constraint; // Allocate new constraint
	newConstraint->setConstraint(newConstraintName, newConstraintNum, newConstraintDate); // Fill constraint 
	constraints->push_back(newConstraint); // Add pointer to constraint* vector 
	
	return;
}



// PlayerRes constructor
PlayerRes::PlayerRes(){
	
}

// PlayerRes destructor
PlayerRes::~PlayerRes(){
	
}

// SET PID 
void PlayerRes::addPID(int pID){
    this->pID = pID;
}

// SET resVal 
void PlayerRes::addResVal(double val){
    this->resVals.push_back(val);
}

// GET resVals Size
int PlayerRes::getResValsSize(){
	return this->resVals.size();
}

// GET PID
int PlayerRes::getPID(){
	return this->pID;
}

// GET resVal
double PlayerRes::getResVal(int valNum){
	return this->resVals[valNum]
}

// DBRes constructor
DBRes::DBRes(){
	
}

// DBRes destructor
DBRes::~DBRes(){
	
}

// GET playerRes 
void DBRes::getPlayerRes(PlayerRes * newPlayerRes, int resNum){
    newPlayerRes->addPID(this->getPID());
	for (int i = 0; i < this->getResValsSize(); i++){
		newPlayerRes->resVals.push_back(this->getResVal(i));
	}
	
	return;
}

// SET playerRes 
void DBRes::addPlayerRes(int pID, double resVal){
    PlayerRes * newRes = new PlayerRes;
    newRes->addPID(pID);
    newRes->addResVal(resVal);
    playerRes.push_back(newRes);
    return;
}




// Map of categories to database tables
// Categories are submitted by all upstream code, and are translated into tables and cols here
// This makes it easy to change underlying database structure without having to change any upstream code
std::string statCategories[5] = {"minutes", "FGA", "FGM", "3PA", "3PM"};
std::string tableNames[5] = {"pbp", "pbp", "pbp", "pbp", "pbp"};
std::string colNames[5] = {"elapsed time", "shot", "shot", "shot", "shot"};
std::string constraintCategories[5] = {"something here"};
std::string constraintNames[5] = {"something here"};

// Default constructor
DBInterface::DBInterface(){
	conn = PQconnectdb("user=postgres dbname=nba_db password=superSecretPassword");
}

// Destructor
DBInterface::~DBInterface() {
	PQfinish(conn);
}


bool DBInterface::getData(DBReq * req, DBRes * res){
	// Build query here
	std::string queryStr = "SELECT ";

	for (int i = 0; i < req->getNumCategories(); i++) {
        int j = 0;
        while (req->getCategory(i) !=statCategories[j]) {
            j++;
        }
        // check that they have found a match here, and add category to SQL
        if (req->getCategory(i) == statCategories[j]){
            queryStr += colNames[j];
            queryStr += ' ';
            // **Update 'fromStr' in here, with joining? 'joinStr' **
        }
    }

    queryStr += "FROM pbp "; // **Temp measure, since everything I'm selecting comes from pbp**

    /*
	// First table should be automatically included, then any others must be (inner?) joined on
	std::vector<std::string> tablesUsed;
	for (int i = 0; i < req->getNumCategories(); i++){
        int j = 0;
        while (req->getCategory(i) != statCategories[j]) {
            j++;
        }
        // check that they have found a match here, and add category to SQL
        if (req->getCategory(i) == statCategories[j]){
            bool prevIncl = false;
            for (int k = 0; k < tablesUsed.size(); k++){
                if (tableNames[j] == tablesUsed[k]){
                    prevIncl = true;
                }
            }
            if (!prevIncl) {
                // JOIN
            }
        }
	}*/


    // Joins

    // where
    std::string whereStr;
    whereStr += "WHERE ";
    for (int i = 0; i < req->getNumConstraints(); i++){
        int j = 0;
        while (req->getConstraint(i)->getConstraintType() != constraintCategories[j]) {
            j++;
        }
        // check that they have found a match here, and add category to SQL
        if (req->getConstraint(i)->getConstraintType() == constraintCategories[j]){
            whereStr += constraintNames[j];
            whereStr += " = ";
            whereStr += req->getConstraint(i)->getConstraintNum();
        }
    }
    queryStr += whereStr;

    // group by (** This will have to be refined as I have SELECT statments that aren't grouped by player**)
    std::string groupStr;
    groupStr += "GROUP BY player_id ";
    queryStr += groupStr;

    // having
	
	
	// query DB 
	PGresult * selectResult = PQexec(conn, queryStr.c_str());
	
	// Place results into res
	
	
	return true;
}

bool DBInterface::getFGA(DBReq * req, DBRes * res){
	// Set query
    std::string queryStr = "SELECT count(event_type) FROM pbp WHERE event_type = 'shot' AND player = ";
    queryStr += std::to_string(req->getPID(0));
    queryStr += " GROUP BY player";

	// Execute query
    PGresult * slctRes = PQexec(conn, queryStr.c_str());

	// Get FGA out of query result
    char * dbResult = PQgetvalue(slctRes, 0, 0);
    char * resStr = new char[256];
    strcpy(resStr, dbResult);

	// Place FGA in resFloat 
    double resFloat = atof(resStr);

	// add to DBRes 
    res->addPlayerRes(req->getPID(0), resFloat);

    delete [] resStr;
    return true;
}

/*

bool getDataFromDB(PGconn * conn, DBReq * req, DBRes * res){
	// Build query string from DBReq object
	std::string queryStr = "SELECT ";
	for (int i = 0; i < req->cols.size(); i++){
		// Add cols to select 
		queryStr += req->cols[i] + ", ";
	}
	queryStr += "FROM " + req->tables[0];
	// Joins
	// where
	// group by
	// having
	
	// Execute query & error check
	PGresult * selectResult = PQexec(conn, queryStr.c_str());
	
	// Place result in DBRes object
	// if result is a single number then
	// else if result is just one row then
	// else
		// make a 2D array
	
	// Return
	return true;
}


bool getPlayerTotalPoints(PGconn * conn, char * resStr, int playerID, struct tm * startDate, struct tm * endDate){
    std::string queryString = "SELECT SUM(points) FROM pbp WHERE result = 'made' AND player = ";
    queryString += std::to_string(playerID);
    queryString += " GROUP BY player";

    PGresult * selectResult = PQexec(conn, queryString.c_str());

    char * dbResult = PQgetvalue(selectResult, 0, 0);
    strcpy(resStr, dbResult);

    return true;
}

bool getTeamPossessionCount(PGconn * conn, char * resultString, std::string teamName, struct tm * startDate, struct tm * endDate){
    std::string queryString = "SELECT COUNT(";

    return true;
}

bool getPlayerMins(){
	
	
	
	if(!getDataFromDB()){
		// Error Handling done at top level?
		return false;
	}
	
	return true;
}

*/
