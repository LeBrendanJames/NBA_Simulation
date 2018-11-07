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

std::string DBReq::getConstraintType(int constraintNum){
    return this->constraints[constraintNum]->getConstraintType();
}

int DBReq::getConstraintNum(int constraintNum){
    return this->constraints[constraintNum]->getConstraintNum(); // should maybe be getConstraintVal to be clearer???
}


// SET PID 
void DBReq::addPID(int newPID){
	pID = newPID;
}

// SET Category 
void DBReq::addCategory(std::string newCategory) {
    category = newCategory;
}

// SET Constraint 
void DBReq::addConstraint(std::string newConstraintName, int newConstraintNum, struct tm * newConstraintDate){
	Constraint * newConstraint = new Constraint; // Allocate new constraint
	newConstraint->setConstraint(newConstraintName, newConstraintNum, newConstraintDate); // Fill constraint 
	constraints.push_back(newConstraint); // Add pointer to constraint* vector
}

std::string DBReq::getConstraintDate(int constraintNum){
	struct tm * tempDate = new struct tm;
	this->constraints[constraintNum]->getConstraintDate(tempDate);
	std::string dateStr = std::to_string(1 + tempDate->tm_mon) + "/" + std::to_string(tempDate->tm_mday) + "/" + std::to_string(1900 + tempDate->tm_year);
	delete tempDate;
	
	return dateStr;
}



// PlayerRes constructor
PlayerRes::PlayerRes(){
	pID = 0;
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
void DBRes::addPlayerRes(){
    PlayerRes * newRes = new PlayerRes;
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

	// Check result status & print error
    if (PQresultStatus(slctRes) == PGRES_TUPLES_OK){
        std::cout << "Results OK." << std::endl;
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

    std::cout << "numtuples = " << numTuples << std::endl;
    std::cout << "numcols = " << numCols << std::endl;
	
	for (int i = 0; i < numTuples; i++){
		// Add playerRes object w/ data from a single tuple, i
        res->addPlayerRes();
		// res->addPlayerRes(atoi(PQgetvalue(slctRes, i, 0)));
		for (int j = 0; j < numCols; j++){
			// Add all associated vals from that tuple
			res->addPlayerResVal(atoi(PQgetvalue(slctRes, i, j)));
		}
	}
	std::cout << "exiting fillRes." << std::endl;
}

Query::Query(DBReq * req){
	queryStr = "SELECT";
	joinStrs = new std::deque<std::string>;
	onStrs = new std::deque<std::string>;
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
	bool joined[15] = {false}; // bool array to mark whether a particular table has been joined into query
		// 0 = games, 1 = plays, 2 = on_court_players, 3 = players, 4 = teams, 5 = player_team, 
		// 6 = jump_balls, 7 = shots, 8 = turnovers, 9 = rebounds, 10 = free_throws, 
		// 11 = fouls, 12 = ejections, 13 = subs, 14 = timeouts, 15 = violations 

	// **need to proxy tables (at least on_court_players, since I can join that multiple times)**
	// switch over all possible categories
	buildCats(req); // To start building query based on category and player info from DBReq

	
	// Loop over constraints, adding to various query strings
	buildConstrts(req, joined); // Function to build in constraints from DBReq
} 

//**In order to get constraints to work, I may need to structure all of the category stuff as a subquery**
//**Then, constraints can just add on a where clause to the table returned from the subquery (and maybe what to SELECT)**
void Query::buildCats(DBReq * req){
	std::string cats[] = {"FGA", "FGM", "3PA", "3PM", "DREB", "OREB", "Steals", "Blocks", "Turnovers", "offPlays", "defPlays"};

	if (req->getCategory() == cats[0]){
		buildFGA(req);
	} else if (req->getCategory() == cats[1]){
		buildFGM(req);
	} else if (req->getCategory() == cats[2]){
		build3PA(req);
	} else if (req->getCategory() == cats[3]){
		build3PM(req);
	} else if (req->getCategory() == cats[4]){
		buildDREB(req);
	} else if (req->getCategory() == cats[5]){
		buildOREB(req);
	} else if (req->getCategory() == cats[6]){
		buildSTL(req);
	} else if (req->getCategory() == cats[7]){
		buildBLK(req);
	} else if (req->getCategory() == cats[8]){
		buildTOV(req);
	} else if (req->getCategory() == cats[9]){
		buildOffPlays(req);
	} else if (req->getCategory() == cats[10]){
		buildDefPlays(req);
	}
}

void Query::buildFGA(DBReq * req){
	queryStr += " count(nba_shots.event_type)";
	fromStr += " nba_shots";
	whereStr += " nba_shots.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND (nba_shots.event_type = 'shot' OR nba_shots.event_type = 'miss')";
	groupStr += " nba_shots.player";
}

void Query::buildFGM(DBReq * req){
	queryStr += " count(nba_shots.event_type)";
	fromStr += " nba_shots";
	whereStr += " nba_shots.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND nba_shots.event_type = 'shot'";
	groupStr += " nba_shots.player";
}

void Query::build3PA(DBReq * req){
	queryStr += " count(nba_shots.event_type)";
	fromStr += " nba_shots";
	whereStr += " nba_shots.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND (nba_shots.event_type = 'shot' OR nba_shots.event_type = 'miss')";
	whereStr += " AND nba_shots.three_pointer = true"; // THIS IS ONLY ADDED LINE TO FGA
	groupStr += " nba_shots.player";
}

void Query::build3PM(DBReq * req){
	queryStr += " count(nba_shots.event_type)";
	fromStr += " nba_shots";
	whereStr += " nba_shots.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND nba_shots.event_type = 'shot'";
	whereStr += " AND nba_shots.three_pointer = true"; // THIS IS ONLY ADDED LINE TO FGM
	groupStr += " nba_shots.player";
}

void Query::buildDREB(DBReq * req){
	queryStr += " count(nba_rebounds.player)";
	fromStr += " nba_rebounds";
	whereStr += " nba_rebounds.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND nba_rebounds.defensive = true";
	groupStr += " nba_rebounds.player";
}

void Query::buildOREB(DBReq * req){
	queryStr += " count(nba_rebounds.player)";
	fromStr += " nba_rebounds";
	whereStr += " nba_rebounds.player = ";
	whereStr += std::to_string(req->getPID());
	whereStr += " AND nba_rebounds.defensive = false";
	groupStr += " nba_rebounds.player";
}

void Query::buildSTL(DBReq * req){
	queryStr += " count(nba_turnovers.steal_player)";
	fromStr += " nba_turnovers";
	whereStr += " nba_turnovers.steal_player = ";
	whereStr += std::to_string(req->getPID());
	groupStr += " nba_turnovers.steal_player";
}

void Query::buildBLK(DBReq * req){
	queryStr += " count(nba_shots.block_player)";
	fromStr += " nba_shots";
	whereStr += " nba_shots.block_player = ";
	whereStr += std::to_string(req->getPID());
	groupStr += " nba_shots.block_player";
}

void Query::buildTOV(DBReq * req){
	queryStr += " count(nba_turnovers.player)";
	fromStr += " nba_turnovers";
	whereStr += " nba_turnovers.player = ";
	whereStr += std::to_string(req->getPID());
	groupStr += " nba_turnovers.player";
}

void Query::buildOffPlays(DBReq * req){
	// Need to join liek 5 tables here to get offensive plays from team that player is on while player is on court
    // https://dba.stackexchange.com/questions/87249/understanding-multiple-table-join-with-aggregation
	// https://community.modeanalytics.com/sql/tutorial/sql-subqueries/
	
	// Looking for: Number of offensive plays where player was on court on offensive team 
		// Want:
			// 1. All offensive plays
			// 2. Filtered to where player on court
			// 3. Filtered to where player team = offensive play team 
			
			
	SELECT * FROM nba_plays p WHERE p.event_type = 'shot' OR p.event_type = 'miss' OR p.event_type = 'turnover' OR p.event_type = 'foul' // Get Offensive Plays
	
	// ******************************************************************************************************************************************************************************************************************
	SELECT * FROM (
		(
			(SELECT * FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p
			LEFT JOIN nba_fouls f
				ON p.game_id = f.game_id AND p.play_id = f.game_id 
		) AS play
		INNER JOIN 
			(SELECT * FROM nba_games  
			INNER JOIN (SELECT * FROM on_court_players oc1 WHERE oc1.player_id = std::to_string(req->getPID())) AS oc 
				ON oc.game_id = nba_games.game_id AND oc.play_id = nba_games.play_id
			) AS g
			ON play.game_id = g.game_id AND play.play_id = g.play_id AND 
			( ((play.event_type = 'shot' OR play.event_type = 'miss' OR play.event_type = 'turnover') AND ((play.team_id = g.home_team AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false)))
			OR ((play.event_type = 'foul' AND play.offensive = false) AND ((play.team_id = g.home_team AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false)))
			) 
	) cat // Will call whatever table is created 'cat', so that adding constraints code knows name of table 
	// *******************************************************************************************************************************************************************************************************************
	
	
	// 1. All offensive plays (shot, miss, turnover, defensive foul drawn)
	SELECT * FROM nba_plays 
	INNER JOIN (
		// Selecting for fouls here  
	) 
	WHERE nba_plays.event_type = 'shot' or nba_plays.event_type = 'miss' or nba_plays.event_type = 'turnover' or nba_plays.event_type = 'foul'
	// The above gets me to all offensive plays
	// Then, make table with on_court_players and whether they were home or away team

		
		INNER JOIN 
		// Get player I want and every play they were on court (cols = game_id, play_id, player_id, home_team) as well as game info tacked on the end 
		(SELECT * FROM nba_games  
		INNER JOIN (SELECT * FROM on_court_players oc1 WHERE oc1.player_id = std::to_string(req->getPID())) AS oc 
			ON oc.game_id = nba_games.game_id AND oc.play_id = nba_games.play_id
		) AS g
		ON 
	
	
	
	
	// Plain SQL
	SELECT count(nba_plays.event_type) FROM nba_plays
	INNER JOIN nba_on_court_players oc ON nba_plays.game_id = oc.game_id AND nba_plays.play_id = oc.play_id
	INNER JOIN nba_games ON nba_plays.game_id = nba_games.game_id
	LEFT JOIN nba_fouls ON nba_plays.game_id = nba_fouls.game_id AND nba_plays.play_id = nba_fouls.play_id 
	
	WHERE // Can only contain stuff from the original table in FROM clause (nba_plays)
	

	queryStr += " count(nba_plays.event_type)";
	fromStr += " nba_plays";

	// Join on_court_players
	joinStrs->push_back("INNER JOIN nba_on_court_players oc");
	onStrs->push_back("ON nba_plays.game_id = nba_on_court_players.game_id AND nba_plays.play_id = nba_on_court_players.play_id");
    // Join games (for home/away team)
	joinStrs->push_back("INNER JOIN nba_games");
	onStrs->push_back("ON nba_plays.game_id = nba_games.game_id");
	// Join fouls (for checking offensive vs defensive fouls when counting offensive plays)
	joinStrs->push_back("LEFT JOIN nba_fouls");
	onStrs->push_back("ON nba_plays.game_id = nba_fouls.game_id AND nba_plays.play_id = nba_fouls.play_id");

	// where player is on court
    // whereStr += " nba_on_court_players.player_id = ";
    whereStr += " oc.player_id = ";
	whereStr += std::to_string(req->getPID());

	// And player's team takes action
	whereStr += " AND (((nba_plays.team_id = nba_games.home_team AND nba_on_court_players.home_team = true) "; // home team takes action and player is home team
	whereStr += "OR (nba_plays.team_id = nba_games.away_team AND nba_on_court_players.home_team = false)) "; // or away team takes action and player is away team
	whereStr += "AND ((nba_plays.event_type = 'shot' OR nba_plays.event_type = 'miss' OR nba_plays.event_type = 'turnover') "; // actions where team is team on offense
	whereStr += "OR (nba_plays.event_type = 'foul' AND nba_fouls.offensive = false AND ((nba_plays.team_id = nba_games.home_team AND nba_on_court_players.home_team = true) "; // foul team is team that commits foul, so we want the foul to be defensive and the team fouling the opposite of the player's team fror whom we are counting plays
	whereStr += "OR (nba_plays.team_id = nba_games.away_team AND nba_on_court_players.home_team = false)))))"; // so team fouling opposite of player team

	groupStr += " nba_on_court_players.player_id";
}

void Query::buildDefPlays(DBReq * req){
	queryStr += " count(nba_plays.event_type)";
	fromStr += " nba_plays";

	// Join on_court_players
	joinStrs->push_back("INNER JOIN nba_on_court_players");
	onStrs->push_back("ON nba_plays.game_id = nba_on_court_players.game_id AND nba_plays.play_id = nba_on_court_players.play_id");
	// Join games (for home/away team)
	joinStrs->push_back("INNER JOIN nba_games");
	onStrs->push_back("ON nba_plays.game_id = nba_games.game_id");
	// Join fouls (for checking offensive vs defensive fouls when counting offensive plays)
	joinStrs->push_back("LEFT JOIN nba_fouls");
	onStrs->push_back("ON nba_plays.game_id = nba_fouls.game_id AND nba_plays.play_id = nba_fouls.play_id");

	// where player is on court
	whereStr += " nba_on_court_players.player_id = ";
	whereStr += std::to_string(req->getPID());

	// And opponent takes action
	whereStr += " AND (((nba_plays.team_id = nba_games.home_team AND nba_on_court_players.home_team = false) "; // home team takes action and player is away team
	whereStr += "OR (nba_plays.team_id = nba_games.away_team AND nba_on_court_players.home_team = true)) "; // or away team takes action and player is home team
	whereStr += "AND (nba_plays.event_type = 'shot' OR nba_plays.event_type = 'miss' OR nba_plays.event_type = 'turnover') "; // actions where team is team on offense
	whereStr += "OR (nba_plays.event_type = 'foul' AND nba_fouls.offensive = false AND ((nba_plays.team_id = nba_games.home_team AND nba_on_court_players.home_team = false) "; // foul team is team that commits foul, so we want the foul to be defensive and the team fouling the player's team for whom we are counting plays
	whereStr += "OR (nba_plays.team_id = nba_games.away_team AND nba_on_court_players.home_team = true)))"; // so team fouling same as player team

	groupStr += " nba_on_court_players.player_id";
}



void Query::buildConstrts(DBReq * req, bool * joined){
	std::string constraints[] = {"playerOnCourt", "playerOffCourt", "startDate", "endDate",
								 "playerTeam", "playerOpponent", "normalPoss"};

	int onCourtCount = 1; // count of how many times on_court_players has been joined
		// Important for putting in table proxies programatically each time.
		// Uses this number to make them all different

	// 1st thing to do is to join plays (if it hasn't been already), as all joins below join onto plays table
	if (joined[1] == false && req->getNumConstraints() > 0){ // only joining if I'm adding constraints and it hasn't been joined yet
		joinStrs->push_back("INNER JOIN nba_plays");
		onStrs->push_back("ON nba_plays.game_id = t.game_id AND nba_plays.play_id = t.game_id"); // Calling original table t
		joined[1] = true;
	}

	for (int i = 0; i < req->getNumConstraints(); i++){
		if (req->getConstraintType(i) == constraints[0]){
			// Joins regardless of previous joins,
			// since multiple on court players is constructed with multiple joins of on_court_players table.

			std::string proxyStmt = "oc" + std::to_string(onCourtCount); // Proxy == oc1, oc2, ..., depending on onCourtCount

			std::string joinStmt = "INNER JOIN nba_on_court_players ";
			joinStmt += proxyStmt;
			onCourtCount++; // so that proxy is 1 higher next time
			joinStrs->push_back(joinStmt);

			std::string onStmt = "ON nba_plays.game_id = " + proxyStmt + ".game_id AND nba_plays.play_id = " + proxyStmt + ".play_id";
			onStrs->push_back(onStmt);

			whereStr += " AND ";
			whereStr += proxyStmt;
			whereStr += ".player_id = ";
			whereStr += std::to_string(req->getConstraintNum(i));
		} else if (req->getConstraintType(i) == constraints[1]){
			// left join + where player_id from this join of on_court_players = NULL
			// That combo will return only rows where player was off court (= oncourt NULL)
			std::string proxyStmt = "oc" + std::to_string(onCourtCount);

			std::string subQuery = "LEFT JOIN (SELECT nba_on_court_players.game_id, nba_on_court_players.play_id, nba_on_court_players.player_id";
			subQuery += " FROM nba_on_court_players WHERE player_id = ";
			subQuery += std::to_string(req->getPID());
			subQuery += ") as ";
			subQuery += proxyStmt;

			joinStrs->push_back(subQuery);

			std::string onStmt = "ON nba_plays.game_id = " + proxyStmt + ".game_id AND nba_plays.play_id = " + proxyStmt + ".play_id";
			onStrs->push_back(onStmt);

			whereStr += " AND " + proxyStmt + ".player_id = NULL";

		} else if (req->getConstraintType(i) == constraints[2]){
			// Join games
			if (joined[0] == false){
				joinStrs->push_back("INNER JOIN nba_games g");
				onStrs->push_back("ON nba_plays.game_id = g.game_id");
			}

			whereStr += " AND g.date >= ";
			whereStr += req->getConstraintDate(i); // getConstraintDate returns date as string
		} else if (req->getConstraintType(i) == constraints[3]){
			// Join games
			if (joined[0] == false){
				joinStrs->push_back("INNER JOIN nba_games g");
				onStrs->push_back("ON nba_plays.game_id = g.game_id");
			}

			std::string constrDate = req->getConstraintDate(i); // New function to write. i = constraint number
			whereStr += " AND g.date <= ";
			whereStr += constrDate;
		} else if (req->getConstraintType(i) == constraints[4]){
			// JOIN games table
			if (joined[0] == false){
				joinStrs->push_back("INNER JOIN nba_games g");
				onStrs->push_back("ON nba_plays.game_id = g.game_id");
			}

			// player home team and home team_id = passed in team_id
			// OR player away team and away team_id = passed in away_id
		} else if (req->getConstraintType(i) == constraints[5]){
			// JOIN games table
			if (joined[0] == false){
				joinStrs->push_back("INNER JOIN nba_games g");
				onStrs->push_back("ON nba_plays.game_id = g.game_id");
			}

			// player home team and away team_id = passed in team_id
			// OR player away team and home team_id = passed in team_id
		} else if (req->getConstraintType(i) == constraints[6]){
			// normalPoss filters out end of quarter heaves, blowouts, ATO's, and end of game fouling situations

		}
	}
}

std::string Query::createFullStr(){
	std::string fullStr = queryStr;
	fullStr += fromStr;
	while (!joinStrs->empty()){ // Will successively pop of front value of joinStrs and onStrs into fullStr
	//for (int i = 0; i < joinStrs->size(); i++){
		fullStr += " ";
		fullStr += joinStrs->front();
		joinStrs->pop_front();
		fullStr += " ";
		fullStr += onStrs->front();
		onStrs->pop_front();
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
