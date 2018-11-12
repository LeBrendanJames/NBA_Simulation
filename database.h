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
#include <deque>



// REQUEST


class Constraint{
private:
    std::string constraintType;
    std::string constraintVal;
    // int constraintNum;
    // struct tm * constraintDate;

public:
	// Constructor/Destructor
	Constraint(std::string constraintType, std::string constraintVal);
	~Constraint();
	
	// Getters 
    std::string getConstraintType();
    std::string getConstraintVal();
};

class DBReq {
private:
	// Vector of playerID's, so that a req can be sent for multiple players at once
    int pID;

    // Vector of categories to pull, so that a request for multiple categories can be sent at once 
    // (will be written in bball terms, so will have to translate to table columns in DAL -
    // should be easy with a hard-coded translation map)
	std::string category;

    // Vector of constraints (Same deal - will have to translate to actual table & column names)
	// This is a list of constraints, both name and value (number or date in struct tm)
    std::vector<Constraint*> constraints;

public:
	// Constructor/Destructor
	DBReq();
	DBReq(int pID, std::string category,
		  std::vector<std::string> * constraintNames, std::vector<std::string> * constraintVals);
	~DBReq();
	
	// Get size of constraints vector 
    int getNumConstraints();
	
	// Getters
    int getPID();
    std::string getCategory();
    std::string getConstraintType(int constraintIndex);
	std::string getConstraintVal(int constraintIndex);

	// Setters
	void setPID(int);
	void setCategory(std::string);
	void addConstraint(std::string constraintType, std::string constraintVal);
};


// RESPONSE


class PlayerRes {
private:
	// PlayerID
	int pID;
	
	// Vector of response values
    std::vector<double> resVals;

public:
    // Constructor/Destructor
	PlayerRes();
	~PlayerRes();

	// Setters 
    void addPID(int);
    void addResVal(double);
	
	// Getters 
	int getResValsSize();
	int getPID();
	double getResVal(int resValIndex);
};

class DBRes {
private:
	// Vector of pointers to playerResponse objects?
		// Player response objects contain playerID and vector of response values? **Class PlayerRes**
		// How do I handle the type of the response? Will it always be int or float? Probably float, since that's still fine for ints
	std::vector<PlayerRes*> playerRes;

	// Can just create with the first vector, then create response objects as I go?
public:
	// Constructor/Destructor 
	DBRes();
	~DBRes();
	
	// Getter 
    void getPlayerRes(PlayerRes *, int);
	
	// Setter 
    void addPlayerRes();
	void addPlayerResVal(double);
};


// INTERFACE


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


// QUERY


class Query {
private:
	std::string queryStr;
	std::string catStr;
	std::deque<std::string> * constrtStr;
	
	void buildQueryFromReq(DBReq * req);

	void buildCats(DBReq * req);
	void buildConstrts(DBReq * req);

	// Each of these returns a subtable for the given player that contains only (game_id, play_id, event_type) that fit their respective category
	void buildFGA(DBReq * req);
	void buildFGM(DBReq * req);
	void build3PA(DBReq * req);
	void build3PM(DBReq * req);
	void buildDREB(DBReq * req);
	void buildOREB(DBReq * req);
	void buildSTL(DBReq * req);
	void buildBLK(DBReq * req);
	void buildTOV(DBReq * req);
	void buildDrawFoul(DBReq * req);
	void buildOffPlays(DBReq * req);
	void buildDefPlays(DBReq * req);
	
	// Each of these returns a subtable of only (game_id, play_id) that satisfy the constraint
	void buildPlayerOnCourt(DBReq * req, int i); // 'i' needed so that I can place the resulting string in the constraintStr array
	void buildPlayerOffCourt(DBReq * req, int i);
	void buildStartDate(DBReq * req, int i);
	void buildEndDate(DBReq * req, int i);
	void buildPlayerTeam(DBReq * req, int i);
	void buildPlayerOpponent(DBReq * req, int i);
	void buildNormalPoss(DBReq * req, int i);

public:
	Query(DBReq *);
	~Query();

	// Takes a category and wraps in a select * to name the subtable 'cat'
	// Then, (inner) joins a constraint table and wraps everything to call the resulting table cat2
	// Continues to (inner) join constraint tables in this way for as many as there are to do 
	std::string createFullStr(DBReq * req);
};

#endif //SIMULATION_DATABASE_H
