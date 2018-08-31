//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
//#include <libpq-fe.h>
#include <string>
#include <iostream>



bool DBInterface::getData(DBReq * req, DBRes * res){
	// Build query here
	std::string queryStr = "SELECT ";
	
	
	
	// query DB 
	PGresult * selectResult = PQexec(conn, queryStr.c_str());
	
	// Place results into res
	
	
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
