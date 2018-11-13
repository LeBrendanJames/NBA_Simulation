//
// Created by Brendan on 8/13/2018.
//
// Wrapper functions for database interaction
// that return a bool (true if they succeed, false otherwise)

#ifndef SIMULATION_DATABASE_H
#define SIMULATION_DATABASE_H

#include <libpq-fe.h>
#include "DBReq.h"
#include "DBRes.h"
#include "Query.h"

class DBInterface{
private:
	PGconn * conn;
	
	void fillRes(DBRes * res, PGresult * slctRes);

public:
	// Connection open upon constructing and close upon deconstruction 
	DBInterface(); // **Use dependency injection?
	~DBInterface();
	
	// dbInterface will expose only one function, which takes a DBReq object containing a player ID, 
	// a category to pull, and a vector of constraint objects
	bool getDataFromDB(DBReq * req, DBRes * res);
};

#endif //SIMULATION_DATABASE_H
