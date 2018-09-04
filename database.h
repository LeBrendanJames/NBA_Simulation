//
// Created by Brendan on 8/13/2018.
//

// Wrapper functions for database interaction
// that return a bool (true if they succeed, false otherwise)

#ifndef SIMULATION_DATABASE_H
#define SIMULATION_DATABASE_H

#include <libpq-fe.h>
#include <string>
#include <vector>

class Constraint{
private:
    std::string constraintType;
    int constraintNum;
    struct tm constraintDate;
public:
    std::string getConstraintType();
    int getConstraintNum();
};

class DBReq {
private:
	// Vector of playerID's
    std::vector<int> pIDs;

    // Vector of categories to pull
    // (will be written in bball terms, so will have to translate to table columns in DAL -
    // should be easy with a hard-coded translation map)
	std::vector<std::string> categories;

    // Vector of constraints (Same deal - will have to translate to actual table & column names)
    std::vector<Constraint*> constraints;

public:
    std::string getCategory(int);
    Constraint* getConstraint(int);
    int getNumCategories();
    int getNumConstraints();
};


class PlayerRes{
private:
	// PlayerID
	int pID;
	
	// Vector of response values
    std::vector<float> resVals;

public:
	
};

class DBRes {
	// Vector of pointers to playerResponse objects?
		// Player response objects contain playerID and vector of response values? **Class PlayerRes**
		// How do I handle the type of the response? Will it always be int or float? Probably float, since that's still fine for ints
	std::vector<PlayerRes*> playerRes;

	// Can just create with the first vector, then create response objects as I go?
};


class DBInterface{
	
	private:
		PGconn * conn;

	
	public:
		// Connection open upon constructing and close upon deconstruction 
		DBInterface(); // Use dependency injection?
		~DBInterface();
		
		// dbInterface will expose only one function, which takes a DBReq object containing a vector of categories to pull, 
		// a vector of constraint objects, and maybe some other stuff (to be determined later as I write code)
		bool getData(DBReq * req, DBRes * res);
	
	
	
	
	
	
		// Base function through which all SELECT requests are sent 
	// This is the only function that directly interacts with the database. All other functions build DBReq objects and pass them to 'getDataFromDB'
	// **This function allocates memory to the passed in DBRes pointer, which needs to be deleted by the calling function**
	//bool getDataFromDB(PGconn * conn, DBReq * req, DBRes * res);
	
	
	
	
	/*bool getMinutes();
	bool getFGA();
	bool getFGM();
	bool get3PM();
	bool get3PA();
	bool getDREB();
	bool getOREB();
	bool getSteals();
	bool getBlocks();
	bool getTurnovers();

	bool getPossessionCount(DBReq * req, DBRes * res); // Will take an array of player ID's so this will return possessions for which all players were on the court (for the same team)
	bool getFGA(DBReq * req, DBRes * res);
	bool getFGM(DBReq * req, DBRes * res);
	bool get3FGM(DBReq * req, DBRes * res);
	bool getDREB(DBReq * req, DBRes * res);
	// etc...


	bool getPlayerTotalPoints(PGconn * conn, char * resultString, int playerID, struct tm * startDate, struct tm * endDate);
	bool getTeamPossessionCount(PGconn * conn, char * resultString, std::string teamName, struct tm * startDate, struct tm * endDate);*/
		

};

#endif //SIMULATION_DATABASE_H
