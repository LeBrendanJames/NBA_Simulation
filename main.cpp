#include <iostream>
#include <libpq-fe.h> //**Not necessary, since already included in database.h?**
#include "database.h"
#include "player.h"
#include <ctime> //**Not necessary, since already included in player.h?**

/*void do_exit(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}*/

int main() {

    // Create DB Class, which opens connection 
    DBInterface * dbFace = new DBInterface;
	
    // Create player, passing in DB class
    Player * playerOne = new Player(dbFace);

    // set all the stuff for the player
    playerOne->setPID(196); // 196 = James Harden

    std::cout << "Everything up to calcStats." << std::endl;
    
    // Get player FGA (for now - eventually this will calc everything and place in private variables)
    playerOne->calcPriors();

    // Print FGA
    std::cout << playerOne->getShootPct() << std::endl;

    delete playerOne;
    delete dbFace;

    // INPUTS:
    // Players for each team
    // Starters for each team
    // Coach for each team
    // Referees for each team, eventually
    // Number of simulations

    // OUTPUTS:
    // Number of Games won by each team, average score of each team, and average total score

    // Create DB connection
    // TODO: Abstract away connection string before uploading to Github
    // TODO: Seems like 'passfile' parameter can be used for this
    /*PGconn *conn = PQconnectdb("user=postgres dbname=nba_db password=superSecretPassword");

    // Check for connection failure
    if (PQstatus(conn) == CONNECTION_BAD) {
        std::cout << "Connection to database failed: " << PQerrorMessage(conn);
        do_exit(conn);
    }

    int ver = PQserverVersion(conn);
    std::cout << "Server version: " << ver << std::endl;

    PGresult * selectResult = PQexec(conn, "SELECT * FROM players WHERE first_name='James'");
    // TODO: Error checking...
    int numTuples = PQntuples(selectResult);
    int numCols = PQnfields(selectResult);

    std::cout << "Query returned " << numTuples << " tuples.";
    std::cout << " Each tuple has " << numCols << " columns." << std::endl;
    std::cout << "Results: " << std::endl;

    for (int i = 0; i < numTuples; i++){
        for (int j = 0; j < numCols; j++){
            //std::cout << "In printing loop." << std::endl;
            char * resultString;
            resultString = PQgetvalue(selectResult, i, j);
            std::wcout << resultString << " ";
        }
        std::cout << std::endl;
    }


    struct tm * startDate = new struct tm;
    startDate->tm_mday = 10; // Day
    startDate->tm_mon = 9; // Months since January
    startDate->tm_year = 114; // Years since 1900
    struct tm * endDate = new struct tm;
    startDate->tm_mday = 20; // Day
    startDate->tm_mon = 3; // Months since January
    startDate->tm_year = 115; // Years since 1900
    char * resultString = new char[256];
    if (getPlayerTotalPoints(conn, resultString, 196, startDate, endDate)){
        std::cout << "James Harden Points: ";
        std::wcout << resultString << std::endl;
    } else {
        std::cout << "ERROR: Database call returned an error." << std::endl;
    }
    delete[] resultString;

    PQfinish(conn);*/

    // Given all needed inputs, I want to be able to estimate probabilities of all possible plays happening
    // on a given possession
    // Then, after each possession, I want to be able to estimate the probability of anything
    // happening before the next possession (which depends on the result of the previous possession)
    // After, I run another possession, and continue until time runs out

    // ****Need a backtesting suite to know how I should calculate probabilities****

    /*int timeRemaining = 0;
    for (int i = 1; i <= 4; i++) { // Quarter
        timeRemaining = 60 * 12;
        while (timeRemaining > 0) {
            prePossessionCalcs();
            findPossessionProbs();
        }
    }*/



    return 0;
}