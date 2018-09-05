//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
#include <string>
#include <iostream>

int DBReq::getPID(int playerNum){
    return pIDs[playerNum];
}

void PlayerRes::addPID(int pID){
    this->pID = pID;
}

void PlayerRes::addResVal(double val){
    this->resVals.push_back(val);
}

PlayerRes * DBRes::getPlayerRes(int resNum){
    return playerRes[resNum];
}

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
    std::string queryStr = "SELECT count(event_type) FROM pbp WHERE event_type = 'shot' AND player = ";
    queryStr += std::to_string(req->getPID(0));
    queryStr += " GROUP BY player";

    PGresult * slctRes = PQexec(conn, queryStr.c_str());

    char * dbResult = PQgetvalue(slctRes, 0, 0);
    char * resStr = new char[256];
    strcpy(resStr, dbResult);

    double resFloat = atof(resStr);

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
