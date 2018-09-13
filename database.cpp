//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
#include <string>
#include <iostream>


// Constraint constructor  
Constraint::Constraint(){
	constraintDate = new struct tm; // Allocate struct tm memory
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
	date->tm_mday = this->constraintDate->tm_mday; // Day
    date->tm_mon = this->constraintDate->tm_mon; // Months since January
    date->tm_year = this->constraintDate->tm_year; // Years since 1900
	
	return;
}

// SET constraint **Does struct tm have a copy constructor? Can I just do constraintDate = this->constraintDate;?**
void Constraint::setConstraint(std::string constraintType, int constraintNum, struct tm * constraintDate){
	this->constraintType = constraintType;
	this->constraintNum = constraintNum;
	this->constraintDate->tm_mday = constraintDate->tm_mday;
	this->constraintDate->tm_mon = constraintDate->tm_mon;
	this->constraintDate->tm_year = constraintDate->tm_year;
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
	this->constraints[constraintNum]->getConstraintDate(tempDate); // Set tempDate = constraintDate
	
	constraint->setConstraint(this->constraints[constraintNum]->getConstraintType(), this->constraints[constraintNum]->getConstraintNum(), tempDate);
	
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
	constraints.push_back(newConstraint); // Add pointer to constraint* vector
	
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
	return this->resVals[valNum];
}

// DBRes constructor
DBRes::DBRes(){
	
}

// DBRes destructor
DBRes::~DBRes(){
	
}

// GET playerRes 
void DBRes::getPlayerRes(PlayerRes * newPlayerRes, int resNum){
    newPlayerRes->addPID(this->playerRes[resNum]->getPID());
	for (int i = 0; i < this->playerRes[resNum]->getResValsSize(); i++){
		newPlayerRes->addResVal(this->playerRes[resNum]->getResVal(i));
	}
	
	return;
}

// SET playerResObj 
void DBRes::addPlayerRes(int pID){
    PlayerRes * newRes = new PlayerRes;
    newRes->addPID(pID);
    playerRes.push_back(newRes);
}

// SET playerResVal at back 
void DBRes::addPlayerResVal(double resVal){
	playerRes[playerRes.size() - 1]->addResVal(resVal);
}




// Default constructor
DBInterface::DBInterface(){
	conn = PQconnectdb("user=postgres dbname=nba_db password=Bcj--403");
}

// Destructor
DBInterface::~DBInterface() {
	PQfinish(conn);
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
	if (PQntuples(slctRes) <= 0){
		return false;
	}
	
    char * resStr = new char[256];
    strcpy(resStr, dbResult);

	// Place FGA in resFloat 
    double resFloat = atof(resStr);

	// add to DBRes 
    res->addPlayerRes(req->getPID(0), resFloat);

    delete [] resStr;
    return true;
}

bool DBInterface::getDataFromDB(DBReq * req, DBRes * res){
	// Build query (THIS CONSTRUCTOR CONTAINS HARD-CODED LOGIC BETWEEN PROGRAM AND TABLE/COLUMN NAMES IN DATABASE)
	Query * qry = new Query(req);
	
	// Execute query
	PGresult * slctRes = PQexec(conn, qry->createFullStr().c_str());
	
	// Place result in res
	fillRes(res, slctRes);
	

}

void DBInterface::fillRes(DBRes * res, PGresult * slctRes){
	// Find number of tuples and number of fields
	int numTuples = PQntuples(slctRes);
    int numCols = PQnfields(slctRes);
	
	for (int i = 0; i < numTuples; i++){
		// Add playerRes object w/ data from a single tuple, i 
		res->addPlayerRes(PQgetvalue(slctRes, i, 0));
		for (int j = 1; j < numCols; j++){
			// Add all associated vals from that tuple
			res->addPlayerResVal(PQgetvalue(slctRes, i, j));
		}
	}
	
}

Query::Query(DBReq * req){
	queryStr = "SELECT ";
	whereStr = "WHERE ";
	groupStr = "GROUP BY ";

	// Call functions to build up query 
	buildQueryFromReq(req);
}

Query::~Query(){
	
}

// HARD-CODED LOGIC FOR HOW STAT CATEGORIES ARE PULLED FROM DB 
Query::buildQueryFromReq(DbReq * req){
	cats[9] = {"FGA", "FGM", "3PA", "3PM", "DREB", "OREB", "Steals", "Blocks", "Turnovers"}
	
	// Loop over req PIDs to add them all to where clause
	whereStr += "("; // Open parentheses since all PID's are OR'd together in parentheses 
	whereStr += "player = ";
	whereStr += std::to_string(req->getPID(0));
	for (int i = 1; i < req->getNumPIDs(); i++){
		whereStr += " OR player = ";
		whereStr += std::to_string(req->getPID(i));
	}
	whereStr += ")";
	
	
	// Loop over req categories, adding to various query strings
	for (int i = 0; i < req->getNumCategories(); i++){
		// switch over all possible categories
		if (req->getCategory(i) == cats[0]){
			queryStr += " count(event_type)";
			whereStr += " event_type = 'shot'"
			groupStr += player;
		} else if (req->getCategory(i) == cats[1]){
			
		} else if (req->getCategory(i) == cats[2]){
			
		} else if (req->getCategory(i) == cats[3]){
			
		} else if (req->getCategory(i) == cats[4]){
			
		} else if (req->getCategory(i) == cats[5]){
			
		} else if (req->getCategory(i) == cats[6]){
			
		} else if (req->getCategory(i) == cats[7]){
			
		} else if (req->getCategory(i) == cats[8]){
			
		} else if (req->getCategory(i) == cats[9]){
			
		}
	}
	
	// Loop over constraints, adding to various query strings
	
} 

Query::createFullStr(){
	std::string fullStr = queryStr;
	if (whereStr.length() > 6){
		fullStr += " ";
		fullStr += whereStr;
	}
	if (groupStr.length() > 9){ // Should always be included if I'm always grouping by player?
		fullStr += " ";
		fullStr += groupStr;
	}
	
	return fullStr;
}
