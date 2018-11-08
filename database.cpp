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
	queryStr = "SELECT count(event_type) FROM (";
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
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND (event_type = 'shot' OR event_type = 'miss')";
}

void Query::buildFGM(DBReq * req){
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND event_type = 'shot'";
}

void Query::build3PA(DBReq * req){
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND (event_type = 'shot' OR event_type = 'miss')";
	catStr += " AND three_pointer = true";
}

void Query::build3PM(DBReq * req){
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND event_type = 'shot'";
	catStr += " AND three_pointer = true";
}

void Query::buildDREB(DBReq * req){
	// Renaming player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_shots WHERE event_type = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND defensive = true"
}

void Query::buildOREB(DBReq * req){
	// Renaming player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_shots WHERE event_type = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND defensive = false"
}

void Query::buildSTL(DBReq * req){
	// Renaming steal_player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, steal_player as event_type FROM nba_turnovers WHERE event_type = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildBLK(DBReq * req){
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE block_player = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildTOV(DBReq * req){
	// Renaming player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_turnovers WHERE event_type = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildOffPlays(DBReq * req){
	// Need to join liek 5 tables here to get offensive plays from team that player is on while player is on court
    // https://dba.stackexchange.com/questions/87249/understanding-multiple-table-join-with-aggregation
	// https://community.modeanalytics.com/sql/tutorial/sql-subqueries/
	
	catStr += "SELECT play.game_id, play.play_id, play.event_type, play.offensive, g.home_team_id, g.away_team FROM (";
	catStr += "(SELECT COALESCE(p.game_id, f.game_id) as game_id, COALESCE(p.play_id, f.play_id) as play_id, p.team_id, p.event_type, f.offensive FROM ";
	catStr += "(SELECT q.game_id, q.play_id, q.team_id, q.event_type FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p ";
	catStr += "LEFT JOIN nba_fouls f ";
	catStr += "ON p.game_id = f.game_id AND p.play_id = f.play_id ";
	catStr += " INNER JOIN (";
	catStr += "SELECT nba_games.game_id, nba_games.home_team as home_team_id, oc.home_team, oc.play_id, nba_games.away_team FROM nba_games ";
	catStr += "INNER JOIN (SELECT * FROM nba_on_court_players oc1 WHERE oc1.player_id = ";
	catStr += std::to_string(req->getPID());
	catStr += ") AS oc ";
	catStr += "ON oc.game_id = nba_games.game_id";
	catStr += ") AS g ";
	catStr += "ON play.game_id = g.game_id AND play.play_id = g.play_id AND ";
	catStr += "( ((play.event_type = 'shot' OR play.event_type = 'miss' OR play.event_type = 'turnover') AND ((play.team_id = g.home_team_id AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false))) ";
	catStr += "OR ((play.event_type = 'foul' AND play.offensive = false) AND ((play.team_id = g.home_team_id AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false)))";
	catStr += ")";
	catStr += ")";


	/*
    //********************************************
    //WORKS IN PGADMIN WITH A JOIN FOR ONCOURT PLAYER AT BOTTOM
    SELECT * FROM (
	SELECT COALESCE(play.game_id, g.game_id) as game_id, COALESCE(play.play_id, g.play_id) as play_id, play.event_type, play.offensive, g.home_team_id, g.away_team FROM (
            (SELECT COALESCE(p.game_id, f.game_id) as game_id, COALESCE(p.play_id, f.play_id) as play_id, p.team_id, p.event_type, f.offensive FROM
            (SELECT q.game_id, q.play_id, q.team_id, q.event_type FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p
		LEFT JOIN nba_fouls f
		ON p.game_id = f.game_id AND p.play_id = f.play_id
		) AS play
		INNER JOIN (
			SELECT nba_games.game_id, nba_games.home_team as home_team_id, oc.home_team, oc.play_id, nba_games.away_team FROM nba_games
			INNER JOIN (SELECT * FROM nba_on_court_players oc1 WHERE oc1.player_id = 196) AS oc
			ON oc.game_id = nba_games.game_id
		) AS g
		ON play.game_id = g.game_id AND play.play_id = g.play_id AND
			( ((play.event_type = 'shot' OR play.event_type = 'miss' OR play.event_type = 'turnover') AND ((play.team_id = g.home_team_id AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false)))
			OR ((play.event_type = 'foul' AND play.offensive = false) AND ((play.team_id = g.home_team_id AND g.home_team = true) OR (play.team_id = g.away_team AND g.home_team = false)))
			)
    )
    ) cat
	//*********************************************
    INNER JOIN (
        SELECT * FROM nba_on_court_players oc2
		WHERE player_id = 462
    ) oncourt2
    ON cat.game_id = oncourt2.game_id AND cat.play_id = oncourt2.play_id
    //*******************************************************************************
	*/

}

void Query::buildDefPlays(DBReq * req){
	//**BASICALLY ONLY CHANGE FROM OFFENSIVE IS TO FLIP g.home_team = true/false it the bottom two full lines of SQL**
	
	catStr += "SELECT play.game_id, play.play_id, play.event_type, play.offensive, g.home_team_id, g.away_team FROM (";
	catStr += "(SELECT COALESCE(p.game_id, f.game_id) as game_id, COALESCE(p.play_id, f.play_id) as play_id, p.team_id, p.event_type, f.offensive FROM ";
	catStr += "(SELECT q.game_id, q.play_id, q.team_id, q.event_type FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p ";
	catStr += "LEFT JOIN nba_fouls f ";
	catStr += "ON p.game_id = f.game_id AND p.play_id = f.play_id ";
	catStr += " INNER JOIN (";
	catStr += "SELECT nba_games.game_id, nba_games.home_team as home_team_id, oc.home_team, oc.play_id, nba_games.away_team FROM nba_games ";
	catStr += "INNER JOIN (SELECT * FROM nba_on_court_players oc1 WHERE oc1.player_id = ";
	catStr += std::to_string(req->getPID());
	catStr += ") AS oc ";
	catStr += "ON oc.game_id = nba_games.game_id";
	catStr += ") AS g ";
	catStr += "ON play.game_id = g.game_id AND play.play_id = g.play_id AND ";
	catStr += "( ((play.event_type = 'shot' OR play.event_type = 'miss' OR play.event_type = 'turnover') AND ((play.team_id = g.home_team_id AND g.home_team = false) OR (play.team_id = g.away_team AND g.home_team = true))) ";
	catStr += "OR ((play.event_type = 'foul' AND play.offensive = false) AND ((play.team_id = g.home_team_id AND g.home_team = false) OR (play.team_id = g.away_team AND g.home_team = true)))";
	catStr += ")";
	catStr += ")";
	
	/*
	//********************************************************************************************
	SELECT play.game_id, play.play_id, play.event_type, play.offensive, g.home_team_id, g.away_team FROM (
            (SELECT p.game_id, p.play_id, p.team_id, p.event_type, f.offensive FROM
            (SELECT q.game_id, q.play_id, q.team_id, q.event_type FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p
		LEFT JOIN nba_fouls f
		ON p.game_id = f.game_id AND p.play_id = f.play_id
		) AS play
		INNER JOIN (
			SELECT nba_games.game_id, nba_games.home_team as home_team_id, oc.home_team, oc.play_id, nba_games.away_team as away_team_id FROM nba_games
			INNER JOIN (SELECT * FROM nba_on_court_players oc1 WHERE oc1.player_id = 196) AS oc
			ON oc.game_id = nba_games.game_id
		) AS g
		ON play.game_id = g.game_id AND play.play_id = g.play_id AND
			( ((play.event_type = 'shot' OR play.event_type = 'miss' OR play.event_type = 'turnover') AND ((play.team_id = g.home_team_id AND g.home_team = false) OR (play.team_id = g.away_team_id AND g.home_team = true)))
			OR ((play.event_type = 'foul' AND play.offensive = false) AND ((play.team_id = g.home_team_id AND g.home_team = false) OR (play.team_id = g.away_team_id AND g.home_team = true)))
			)
    )
	//******************************************************************************************
	*/
	
}



void Query::buildConstrts(DBReq * req){
	std::string constraints[] = {"playerOnCourt", "playerOffCourt", "startDate", "endDate",
								 "playerTeam", "playerOpponent", "normalPoss"};

	for (int i = 0; i < req->getNumConstraints(); i++){
		// Call functions to build subquery strings (just the subquery - joining handled seperately)
		if (req->getConstraintType(i) == constraints[0]){
			buildPlayerOnCourt(req, i); // 'i' needed so that I can place the resulting string in the constraintStr array
		} else if (req->getConstraintType(i) == constraints[1]){
			buildPlayerOffCourt(req, i);
		} else if (req->getConstraintType(i) == constraints[2]){
			buildStartDate(req, i);
		} else if (req->getConstraintType(i) == constraints[3]){
			buildEndDate(req, i);
		} else if (req->getConstraintType(i) == constraints[4]){
			buildPlayerTeam(req, i);
		} else if (req->getConstraintType(i) == constraints[5]){
			buildPlayerOpponent(req, i);
		} else if (req->getConstraintType(i) == constraints[6]){
			buildNormalPoss(req, i);
		}
	}
}

void Query::buildPlayerOnCourt(DBReq * req, int i){
	// join with nba_on_court_players where PID is on court
	// SELECT game_id, play_id FROM nba_on_court_players oncourt WHERE oncourt.player_id = [num] 
	constraintStr[i] += "SELECT game_id, play_id FROM nba_on_court_players oncourt WHERE oncourt.player_id = ";
	constraintStr[i] += req->getConstraintNum(i); 
}

void Query::buildPlayerOffCourt(DBReq * req, int i){
	// join with nba_on_court_players where PID is not on court
	// SELECT game_id, play_id FROM nba_on_court_players offcourt WHERE offcourt.player_id != [num] 
	constraintStr[i] += "SELECT game_id, play_id FROM nba_on_court_players offcourt WHERE offcourt.player_id != ";
	constraintStr[i] += req->getConstraintNum(i); 
}

void Query::buildStartDate(DBReq * req, int i){
	// Join games
	// SELECT game_id FROM nba_games WHERE date >= [game date]
	constraintStr[i] += "SELECT game_id FROM nba_games WHERE date >= ";
	constraintStr[i] += req->getConstraintDate(i); 
}

void Query::buildEndDate(DBReq * req, int i){
	// Join games
	// SELECT game_id FROM nba_games WHERE date <= [game date]
	constraintStr[i] += "SELECT game_id FROM nba_games WHERE date <= ";
	constraintStr[i] += req->getConstraintDate(i); 
}

void Query::buildPlayerTeam(DBReq * req, int i){
	// Want to pull only game/play id's where the player was on a certain team 
		// so, the player can have done stuff, but on a different team, and that will be excluded
	// join plays and games, then join that table with filtered on_court_players (to player)
		// do where on that table for where (player = home_team, home_team_id = passed in id) OR (player != home_team, home_team_id = passed in id)
	
	
	
	//****************************************************************************************
	SELECT gp.game_id, gp.play_id FROM (
		SELECT pl.game_id, pl.play_id, gm.home_team_id FROM plays pl 
		INNER JOIN (
			SELECT game_id, home_team_id FROM games
			) gm 
		ON pl.game_id = gm.game_id
	) gp 
	INNER JOIN (
		SELECT game_id, play_id, home_team FROM nba_on_court_players WHERE player_id = std::to_string(req->getPID())
	) plyr
	ON gp.game_id = plyr.game_id AND gp.play_id = plyr.play_id AND ((plyr.home_team = true AND (gp.home_team_id = req->getConstraintNum(i))) OR (plyr.home_team = false AND (gp.home_team_id != req->getConstraintNum(i))))
	//*****************************************************************************************
	
}

void Query::buildPlayerOpponent(DBReq * req, int i){
	// Playing against a certain team 
		// Just same as above with home_team = true/false flipped?
	
	
	
	// JOIN games table
	if (joined[0] == false){
		joinStrs->push_back("INNER JOIN nba_games g");
		onStrs->push_back("ON nba_plays.game_id = g.game_id");
	}

	// player home team and away team_id = passed in team_id
	// OR player away team and home team_id = passed in team_id
}

void Query::buildNormalPoss(DBReq * req, int i){
	// normalPoss filters out end of quarter heaves, blowouts, ATO's, and end of game fouling situations
	// just joining nba_plays with various restrictions
}




std::string Query::createFullStr(DBReq * req){
	
	fullStr = catStr; // Starting cat SELECT
	std::string catName = "cat";
	std::string constrtName = "cstrt"
	
	// Wrap in cat and add constraint join (**make into its own function?**)
	for (int i = 0; i < req->getNumConstraints()){
		// Wrap current query in a select statement whose purpose is really to give the query a table name so that it can be joined 
		preStr = "SELECT game_id, play_id, event_type FROM (";
		fullStr = preStr + fullStr;
		fullStr += ") AS ";
		fullStr += catName;
		
		// Add constraint (join)
		fullStr += " INNER JOIN (";
		fullStr += constraintStr[i];
		fullStr += ") AS ";
		fullStr += constrtName;
		fullStr += " ON ";
		fullStr += catName;
		fullStr += ".game_id = ";
		fullStr += constrtName;
		fullStr += ".game_id AND ";
		fullStr += catName;
		fullStr += ".play_id = "
		fullStr += constrtName;
		fullStr += ".play_id";
		
		// Update cat name, so that it proxies a new table next loop through
		catName = "cat";
		catName += i + 1;
		
		// Update cnstrnt name, so that it proxies a new table next loop through
		constrtName = "cstrt";
		constrtName += i + 1;
	}
	
	// Wrap in final SELECT
	fullStr = "SELECT count(event_type) FROM (" + fullStr + ") AS " + catName; // I think table name at end is required?
	
	
	return fullStr;
}
