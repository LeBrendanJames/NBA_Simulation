//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
#include <libpq-fe.h>
#include <string>
#include <iostream>


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


