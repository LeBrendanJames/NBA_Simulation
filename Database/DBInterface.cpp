//
// Created by Brendan on 8/13/2018.
//

#include "DBInterface.h"
#include <ctime>
#include <string>
#include <iostream>


// Default constructor
DBInterface::DBInterface(){
	conn = PQconnectdb("user=postgres dbname=nba_db password=Bcj--403");
}

// Destructor
DBInterface::~DBInterface() {
	PQfinish(conn);
}

bool DBInterface::getDataFromDB(DBReq * req, DBRes * res){
	// Build query (THIS CONSTRUCTOR CONTAINS HARD-CODED LOGIC BETWEEN PROGRAM AND TABLE/COLUMN NAMES IN DATABASE)
	Query * qry = new Query(req);

	//std::cout << "Created query." << std::endl;
	//std::cout << qry->createFullStr(req) << std::endl;

	// Execute query
	PGresult * slctRes = PQexec(conn, qry->createFullStr(req).c_str());

	// Check result status & print error
    if (PQresultStatus(slctRes) == PGRES_TUPLES_OK){
        //std::cout << "Results OK." << std::endl;
    } else {
        char * slctErr = PQresultErrorMessage(slctRes);
        std::wcout << "SELECT error: " << slctErr << std::endl;
    }
	
	// Place result in res
	fillRes(res, slctRes);
	
    return true;
}

void DBInterface::fillRes(DBRes * res, PGresult * slctRes){
	// Find number of tuples and number of fields
	int numTuples = PQntuples(slctRes);
    int numCols = PQnfields(slctRes);

    //std::cout << "numtuples = " << numTuples << std::endl;
    //std::cout << "numcols = " << numCols << std::endl;
	
	for (int i = 0; i < numTuples; i++){
		// Add playerRes object w/ data from a single tuple, i
        res->addPlayerRes();
		// res->addPlayerRes(atoi(PQgetvalue(slctRes, i, 0)));
		for (int j = 0; j < numCols; j++){
			// Add all associated vals from that tuple
			res->addPlayerResVal(atoi(PQgetvalue(slctRes, i, j)));
		}
	}
	//std::cout << "exiting fillRes." << std::endl;
}

int DBInterface::getTeamIDFromCode(std::string code) {
	if (code == "GSW"){
		return 10;
	} else if (code == "MIL"){
		return 17;
	} else {
		return 0;
	}
}
