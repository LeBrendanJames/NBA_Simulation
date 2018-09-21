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

// GET numConstraints
int DBReq::getNumConstraints(){
	return constraints.size();
}

// GET PID
int DBReq::getPID(){
    return pID;
}

// GET Category
std::string DBReq::getCategory(){
	return category;
}

// GET Constraint
void DBReq::getConstraint(Constraint * constraint, int constraintNum){
	struct tm * tempDate = new struct tm;
	this->constraints[constraintNum]->getConstraintDate(tempDate); // Set tempDate = constraintDate
	
	constraint->setConstraint(this->constraints[constraintNum]->getConstraintType(), this->constraints[constraintNum]->getConstraintNum(), tempDate);
}

// SET PID 
void DBReq::addPID(int newPID){
	pID = newPID;
}

// SET Category 
void DBReq::addCategory(std::string newCategory){
	category = newCategory;

// SET Constraint 
void DBReq::addConstraint(std::string newConstraintName, int newConstraintNum, struct tm * newConstraintDate){
	Constraint * newConstraint = new Constraint; // Allocate new constraint
	newConstraint->setConstraint(newConstraintName, newConstraintNum, newConstraintDate); // Fill constraint 
	constraints.push_back(newConstraint); // Add pointer to constraint* vector
}

std::string DBReq::getConstraintDate(int constraintNum){
	struct tm * tempDate = new struct tm;
	req->constraints[i]->getConstraintDate(tempDate);
	dateStr = std::to_string(1 + tempDate->tm_mon) + "/" + std::to_string(tempDate->tm_mday) + "/" + std::to_string(1900 + tempDate->tm_year)
	delete tempDate;
	
	return dateStr;
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

bool DBInterface::getDataFromDB(DBReq * req, DBRes * res){
	// Build query (THIS CONSTRUCTOR CONTAINS HARD-CODED LOGIC BETWEEN PROGRAM AND TABLE/COLUMN NAMES IN DATABASE)
	Query * qry = new Query(req);

	std::cout << "Created query." << std::endl;
	std::cout << qry->createFullStr() << std::endl;

	// Execute query
	PGresult * slctRes = PQexec(conn, qry->createFullStr().c_str());
	
	// Place result in res
	fillRes(res, slctRes);
	
    return true;
}

void DBInterface::fillRes(DBRes * res, PGresult * slctRes){
	// Find number of tuples and number of fields
	int numTuples = PQntuples(slctRes);
    int numCols = PQnfields(slctRes);

    std::cout << "numtuples = " << numTuples << std::endl;
    std::cout << "numcols = " << numCols << std::endl;
	
	for (int i = 0; i < numTuples; i++){
		// Add playerRes object w/ data from a single tuple, i 
		res->addPlayerRes(atoi(PQgetvalue(slctRes, i, 0)));
		for (int j = 1; j < numCols; j++){
			// Add all associated vals from that tuple
			res->addPlayerResVal(atoi(PQgetvalue(slctRes, i, j)));
		}
	}
	
}

Query::Query(DBReq * req){
	queryStr = "SELECT";
	joinStrs = new std::vector<std::string>;
	onStrs = new std::vector<std::string>;
	fromStr = " FROM";
	whereStr = "WHERE";
	groupStr = "GROUP BY";

	// Call functions to build up query 
	buildQueryFromReq(req);
}

Query::~Query(){
	delete joinStrs;
	delete onStrs;
}

// HARD-CODED LOGIC FOR HOW STAT CATEGORIES ARE PULLED FROM DB 
void Query::buildQueryFromReq(DBReq * req){
	std::string cats[11] = {"FGA", "FGM", "3PA", "3PM", "DREB", "OREB", "Steals", "Blocks", "Turnovers", "offPlays", "defPlays"};
	std::string constraints[7] = {"playerOnCourt", "playerOffCourt", "startDate", "endDate", "playerTeam", "playerOpponent", "normalPoss"};
	
	bool joined[15] = {false}; // bool array to mark whether a particular table has been joined into query
		// 0 = games, 1 = plays, 2 = on_court_players, 3 = players, 4 = teams, 5 = player_team, 
		// 6 = jump_balls, 7 = shots, 8 = turnovers, 9 = rebounds, 10 = free_throws, 
		// 11 = fouls, 12 = ejections, 13 = subs, 14 = timeouts, 15 = violations 
		
	int onCourtCount = 1; // count of how many times on_court_players has been joined
						  // Important for putting in table proxies programatically each time. Uses this number to make them all different
	
	
	// **need to proxy tables (at least on_court_players, since I can join that multiple times)**
	// switch over all possible categories
	// buildCats(req, &cats, &joined); // To start building query based on category and player info from DBReq 
	if (req->getCategory() == cats[0]){
		queryStr += " count(event_type)";
		fromStr += " shots";
		whereStr += " shots.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND (shots.event_type = 'shot' OR shots.event_type = 'miss')";
		groupStr += " shots.player";
	} else if (req->getCategory() == cats[1]){
		queryStr += " count(event_type)";
		fromStr += " shots";
		whereStr += " shots.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND shots.event_type = 'shot'";
		groupStr += " shots.player";
	} else if (req->getCategory() == cats[2]){
		queryStr += " count(event_type)";
		fromStr += " shots";
		whereStr += " shots.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND (shots.event_type = 'shot' OR shots.event_type = 'miss')";
		whereStr += " AND shots.three_pointer = true"; // THIS IS ONLY ADDED LINE TO FGA 
		groupStr += " shots.player";
	} else if (req->getCategory() == cats[3]){
		queryStr += " count(event_type)";
		fromStr += " shots";
		whereStr += " shots.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND shots.event_type = 'shot'";
		whereStr += " AND shots.three_pointer = true"; // THIS IS ONLY ADDED LINE TO FGM
		groupStr += " shots.player";
	} else if (req->getCategory() == cats[4]){
		queryStr += " count(rebounds.player)";
		fromStr += " rebounds";
		whereStr += " rebounds.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND rebounds.defensive = true";
		groupStr += " rebounds.player";
	} else if (req->getCategory() == cats[5]){
		queryStr += " count(rebounds.player)";
		fromStr += " rebounds";
		whereStr += " rebounds.player = ";
		whereStr += std::to_string(req->getPID());
		whereStr += " AND rebounds.defensive = false";
		groupStr += " rebounds.player";
	} else if (req->getCategory() == cats[6]){
		queryStr += " count(turnovers.steal_player)";
		fromStr += " turnovers";
		whereStr += " turnovers.steal_player = ";
		whereStr += std::to_string(req->getPID());
		groupStr += " turnovers.steal_player";
	} else if (req->getCategory() == cats[7]){
		queryStr += " count(shots.block_player)";
		fromStr += " shots";
		whereStr += " shots.block_player = ";
		whereStr += std::to_string(req->getPID());
		groupStr += " shots.block_player";
	} else if (req->getCategory() == cats[8]){
		queryStr += " count(turnovers..player)";
		fromStr += " turnovers";
		whereStr += " turnovers.player = ";
		whereStr += std::to_string(req->getPID());
		groupStr += " turnovers.player";
	} else if (req->getCategory() == cats[9]){ // Need to join liek 5 tables here to get offensive plays from team that player is on while player is on court
		queryStr += " count(event_type)";
		fromStr += " plays";
		
		// Join on_court_players 
		joinStr.push_back("INNER JOIN on_court_players");
		onStrs.push_back("ON plays.game_id = on_court_players.game_id AND plays.play_id = on_court_players.play_id");
		// Join games (for home/away team)
		joinStr.push_back("INNER JOIN games");
		onStrs.push_back("ON plays.game_id = games.game_id");
		// Join fouls (for checking offensive vs defensive fouls when counting offensive plays)
		joinStrs.push_back("LEFT JOIN fouls");
		onStrs.push_back("ON plays.game_id = fouls.game_id AND plays.play_id = fouls.play_id");
		
		// where player is on court 
		whereStr += " on_court_players.player_id = ";
		whereStr += std::to_string(req->getPID());
		
		// And player's team takes action
		whereStr += " AND (((plays.team_id = games.home_team AND on_court_players.home_team = true) "; // home team takes action and player is home team 
		whereStr += "OR (plays.team_id = games.away_team AND on_court_players.home_team = false)) "; // or away team takes action and player is away team 
		whereStr += "AND (plays.event_type = 'shot' OR plays.event_type = 'miss' OR plays.event_type = 'turnover') "; // actions where team is team on offense
		whereStr += "OR (plays.event_type = 'foul' AND fouls.offensive = false AND ((plays.team_id = games.home_team AND on_court_players.home_team = true) "; // foul team is team that commits foul, so we want the foul to be defensive and the team fouling the opposite of the player's team fror whom we are counting plays 
		whereStr += "OR (plays.team_id = games.away_team AND on_court_players.home_team = false)))"; // so team fouling opposite of player team 
		
		groupStr += " on_court_players.player_id";
	} else if (req->getCategory() == cats[10]){ 
		queryStr += " count(event_type)";
		fromStr += " plays";
		
		// Join on_court_players 
		joinStr.push_back("INNER JOIN on_court_players");
		onStrs.push_back("ON plays.game_id = on_court_players.game_id AND plays.play_id = on_court_players.play_id");
		// Join games (for home/away team)
		joinStr.push_back("INNER JOIN games");
		onStrs.push_back("ON plays.game_id = games.game_id");
		// Join fouls (for checking offensive vs defensive fouls when counting offensive plays)
		joinStrs.push_back("LEFT JOIN fouls");
		onStrs.push_back("ON plays.game_id = fouls.game_id AND plays.play_id = fouls.play_id");
		
		// where player is on court 
		whereStr += " on_court_players.player_id = ";
		whereStr += std::to_string(req->getPID());
		
		// And opponent takes action
		whereStr += " AND (((plays.team_id = games.home_team AND on_court_players.home_team = false) "; // home team takes action and player is away team 
		whereStr += "OR (plays.team_id = games.away_team AND on_court_players.home_team = true)) "; // or away team takes action and player is home team 
		whereStr += "AND (plays.event_type = 'shot' OR plays.event_type = 'miss' OR plays.event_type = 'turnover') "; // actions where team is team on offense
		whereStr += "OR (plays.event_type = 'foul' AND fouls.offensive = false AND ((plays.team_id = games.home_team AND on_court_players.home_team = false) "; // foul team is team that commits foul, so we want the foul to be defensive and the team fouling the player's team for whom we are counting plays 
		whereStr += "OR (plays.team_id = games.away_team AND on_court_players.home_team = true)))"; // so team fouling same as player team 
		
		groupStr += " on_court_players.player_id";
	} 
	
	// Loop over constraints, adding to various query strings
	// buildConstrts(req, &constraints, &joined); // Function to build in constraints from DBReq 
	
	// 1st thing to do is to join plays (if it hasn't been already), as all joins below join onto plays table
	if (joined[1] == false && req->getNumConstraints() > 0){ // only joining if I'm adding constraints and it hasn't been joined yet 
		joinStrs.push_back("INNER JOIN plays");
		onStrs.push_back("ON plays.game_id = t.game_id AND plays.play_id = t.game_id"); // Calling original table t 
		joined[1] = true;
	}
	
	for (int i = 0; i < req->getNumConstraints(); i++){ 
		if (req->getConstraintType(i) == constraints[0]){
			// Joins regardless of previous joins, 
			// since multiple on court players is constructed with multiple joins of on_court_players table.
			
			std::string proxyStmt = "oc" + std::to_string(onCourtCount); // Proxy == oc1, oc2, ..., depending on onCourtCount
			
			std::string joinStmt = "INNER JOIN on_court_players ";
			joinStmt += proxyStmt; 
			onCourtCount++; // so that proxy is 1 higher next time
			joinStrs.push_back(joinStmt);
			
			std::string onStmt = "ON plays.game_id = " + proxyStmt + ".game_id AND plays.play_id = " + proxyStmt + ".play_id";
			onStrs.push_back(onStmt);
			
			whereStr += " AND "
			whereStr += proxyStmt;
			whereStr += ".player_id = ";
			whereStr += std::to_string(req->getConstraintNum(i));
		} else if (req->getConstraintType(i) == constraints[1]){
			// left join + where player_id from this join of on_court_players = NULL
			// That combo will return only rows where player was off court (= oncourt NULL)
			std::string proxyStmt = "oc" + std::to_string(onCourtCount);
			
			std::string subQuery = "LEFT JOIN (SELECT on_court_players.game_id, on_court_players.play_id, on_court_players.player_id";
			subQuery += " FROM on_court_players WHERE player_id = ";
			subQuery += std::to_string(req->getPID());
			subQuery += ") as ";
			subQuery += proxyStmt;
			
			joinStrs.push_back(subquery);
			
			std::string onStmt = "ON plays.game_id = " + proxyStmt + ".game_id AND plays.play_id = " + proxyStmt + ".play_id";
			onStrs.push_back(onStmt);
			
			whereStr += " AND " + proxyStmt + ".player_id = NULL";
			
		} else if (req->getConstraintType(i) == constraints[2]){
			// Join games 
			if (joined[0] == false){
				joinStrs.push_back("INNER JOIN games g");
				onStrs.push_back("ON plays.game_id = g.game_id");
			}
			
			whereStr += " AND g.date >= ";
			whereStr += req->getConstraintDate(i); // getConstraintDate returns date as string 
		} else if (req->getConstraintType(i) == constraints[3]){
			// Join games 
			if (joined[0] == false){
				joinStrs.push_back("INNER JOIN games g");
				onStrs.push_back("ON plays.game_id = g.game_id");
			}
			
			std::string constrDate = getConstraintDate(i); // New function to write. i = constraint number 
			whereStr += " AND g.date <= ";
			whereStr += constrDate;		
		} else if (req->getConstraintType(i) == constraints[4]){
			// JOIN games table 
			if (joined[0] == false){
				joinStrs.push_back("INNER JOIN games g");
				onStrs.push_back("ON plays.game_id = g.game_id");
			}
			
			// player home team and home team_id = passed in team_id
			// OR player away team and away team_id = passed in away_id
		} else if (req->getConstraintType(i) == constraints[5]){
			// JOIN games table
			if (joined[0] == false){
				joinStrs.push_back("INNER JOIN games g");
				onStrs.push_back("ON plays.game_id = g.game_id");
			}
			
			// player home team and away team_id = passed in team_id
			// OR player away team and home team_id = passed in team_id
		} else if (req->getConstraintType(i) == constraints[6]){
			// normalPoss filters out end of quarter heaves, blowouts, ATO's, and end of game fouling situations
			
		}
	}
	
} 

std::string Query::getConstraintDate(DBReq * req, int constraintNum){
	return 
}

std::string Query::createFullStr(){
	std::string fullStr = queryStr;
	fullStr += fromStr;
	for (int i = 0; i < joinStrs.size(); i++){
		fullStr += " ";
		fullStr += joinStrs[i];
		fullStr += " ";
		fullStr += onStrs[i];
	}
	if (whereStr != "WHERE"){
		fullStr += " ";
		fullStr += whereStr;
	}
	if (groupStr != "GROUP BY"){ // Should always be included if I'm always grouping by player?
		fullStr += " ";
		fullStr += groupStr;
	}
	
	return fullStr;
}
