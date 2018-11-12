//
// Created by Brendan on 8/13/2018.
//

#include "database.h"
#include <ctime>
#include <string>
#include <iostream>


// Constraint constructor  
Constraint::Constraint(std::string constraintType, std::string constraintVal){
	this->constraintType = constraintType;
	this->constraintVal = constraintVal;
}

// Constraint destructor
Constraint::~Constraint() = default;

// GET constraintType 
std::string Constraint::getConstraintType(){
	return this->constraintType;
}

// GET constraintVal
std::string Constraint::getConstraintVal(){
	return this->constraintVal;
}



// DBReq constructor
DBReq::DBReq(){
	
}

DBReq::DBReq(int pID, std::string category,
             std::vector<std::string> * constraintNames, std::vector<std::string> * constraintVals){
    this->pID = pID;
    this->category = category;
    for (int i = 0; i < constraintNames->size(); i++) {
        addConstraint((*constraintNames)[i], (*constraintVals)[i]);
    }
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

// GET constraintType
std::string DBReq::getConstraintType(int constraintIndex){
    return this->constraints[constraintIndex]->getConstraintType();
}

// GET constraintVal
std::string DBReq::getConstraintVal(int constraintIndex){
	return this->constraints[constraintIndex]->getConstraintVal();
}



// SET PID 
void DBReq::setPID(int newPID){
	pID = newPID;
}

// SET Category 
void DBReq::setCategory(std::string newCategory) {
    category = newCategory;
}

// SET Constraint 
void DBReq::addConstraint(std::string newConstraintType, std::string newConstraintVal){
	Constraint * newConstraint = new Constraint(newConstraintType, newConstraintVal); // Allocate new constraint
	constraints.push_back(newConstraint); // Add pointer to constraint* vector
}




// PlayerRes constructor
PlayerRes::PlayerRes(){
	pID = 0;
}

// PlayerRes destructor
PlayerRes::~PlayerRes() = default;

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
double PlayerRes::getResVal(int resValIndex){
	return this->resVals[resValIndex];
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
	std::cout << qry->createFullStr(req) << std::endl;

	// Execute query
	PGresult * slctRes = PQexec(conn, qry->createFullStr(req).c_str());

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
	catStr = "";
	constrtStr = new std::deque<std::string>;

	// Call functions to build up query 
	buildQueryFromReq(req);
}

Query::~Query(){
	delete constrtStr; // **Is this more complicated?**
}

// HARD-CODED LOGIC FOR HOW STAT CATEGORIES ARE PULLED FROM DB 
void Query::buildQueryFromReq(DBReq * req){
	buildCats(req); // To start building query based on category and player info from DBReq
	buildConstrts(req); // Function to build in constraints from DBReq
} 

//**In order to get constraints to work, I may need to structure all of the category stuff as a subquery**
//**Then, constraints can just add on a where clause to the table returned from the subquery (and maybe what to SELECT)**
void Query::buildCats(DBReq * req){
	std::string cats[] = {"FGA", "FGM", "3PA", "3PM", "DREB", "OREB",
						  "Steals", "Blocks", "Turnovers", "DrawnFouls",
						  "offPlays", "defPlays"};

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
		buildDrawFoul(req);
	} else if (req->getCategory() == cats[10]){
		buildOffPlays(req);
	} else if (req->getCategory() == cats[11]){
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
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_rebounds WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND defensive = true";
}

void Query::buildOREB(DBReq * req){
	// Renaming player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_rebounds WHERE player = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND defensive = false";
}

void Query::buildSTL(DBReq * req){
	// Renaming steal_player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, steal_player as event_type FROM nba_turnovers WHERE steal_player = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildBLK(DBReq * req){
	catStr += "SELECT game_id, play_id, event_type FROM nba_shots WHERE block_player = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildTOV(DBReq * req){
	// Renaming player to event_type is a little bit of a hack to build larger queries where I know I'll just be selecting the count
	catStr += "SELECT game_id, play_id, player as event_type FROM nba_turnovers WHERE player = ";
	catStr += std::to_string(req->getPID());
}

void Query::buildDrawFoul(DBReq * req){
	catStr += "SELECT game_id, play_id, opponent as event_type FROM nba_fouls WHERE opponent = ";
	catStr += std::to_string(req->getPID());
	catStr += " AND offensive = false";
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
	catStr += ") AS play ";
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
	//BASICALLY ONLY CHANGE FROM OFFENSIVE IS TO FLIP g.home_team = true/false it the bottom two full lines of SQL
	
	catStr += "SELECT play.game_id, play.play_id, play.event_type, play.offensive, g.home_team_id, g.away_team FROM (";
	catStr += "(SELECT COALESCE(p.game_id, f.game_id) as game_id, COALESCE(p.play_id, f.play_id) as play_id, p.team_id, p.event_type, f.offensive FROM ";
	catStr += "(SELECT q.game_id, q.play_id, q.team_id, q.event_type FROM nba_plays q WHERE q.event_type = 'shot' OR q.event_type = 'miss' OR q.event_type = 'turnover' OR q.event_type = 'foul') p ";
	catStr += "LEFT JOIN nba_fouls f ";
	catStr += "ON p.game_id = f.game_id AND p.play_id = f.play_id ";
	catStr += ") AS play ";
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

	std::cout << "In buildConstrts." << std::endl;
	fflush(stdout);

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
    std::string tempStr = "SELECT game_id, play_id FROM nba_on_court_players oncourt WHERE oncourt.player_id = ";
    tempStr += req->getConstraintVal(i);

    constrtStr->push_back(tempStr);
}

void Query::buildPlayerOffCourt(DBReq * req, int i){
	// **Want all game_id, play_id that are not in table of all game_id, play_id that player was on court**
	//**SQL that works
	/*********************************************************************************
	select game_id, play_id from nba_on_court_players
	where player_id = 196 AND
			(game_id, play_id) NOT IN
			(select game_id, play_id from nba_on_court_players where player_id = 462)
	**********************************************************************************/
	std::string tempStr = "SELECT game_id, play_id FROM nba_on_court_players WHERE player_id = ";
	tempStr += std::to_string(req->getPID());
	tempStr += " AND (game_id, play_id) NOT IN (SELECT game_id, play_id FROM nba_on_court_players WHERE player_id = ";
	tempStr += req->getConstraintVal(i);
	tempStr += ")";

	// join with nba_on_court_players where PID is not on court
	// SELECT game_id, play_id FROM nba_on_court_players offcourt WHERE offcourt.player_id != [num] 
	// std::string tempStr = "SELECT game_id, play_id FROM nba_on_court_players offcourt WHERE offcourt.player_id != ";
	// tempStr += req->getConstraintVal(i);

    constrtStr->push_back(tempStr);
}

void Query::buildStartDate(DBReq * req, int i){
	// Join games
	// SELECT game_id FROM nba_games WHERE date >= [game date]
	std::string tempStr = "SELECT nba_games.game_id, nba_plays.play_id FROM nba_games ";
	tempStr += "INNER JOIN nba_plays ON nba_games.game_id = nba_plays.game_id ";
	tempStr += "WHERE nba_games.date >= '";
	tempStr += req->getConstraintVal(i);
	tempStr += "'";

    constrtStr->push_back(tempStr);
}

void Query::buildEndDate(DBReq * req, int i){
	// Join games
	// SELECT game_id FROM nba_games WHERE date <= [game date]
	std::string tempStr = "SELECT nba_games.game_id, nba_plays.play_id FROM nba_games ";
	tempStr += "INNER JOIN nba_plays ON nba_games.game_id = nba_plays.game_id ";
	tempStr += "WHERE nba_games.date <= '";
	tempStr += req->getConstraintVal(i);
	tempStr += "'";


    constrtStr->push_back(tempStr);
}

void Query::buildPlayerTeam(DBReq * req, int i){
	// Want to pull only game/play id's where the player was on a certain team 
		// so, the player can have done stuff, but on a different team, and that will be excluded
	// join plays and games, then join that table with filtered on_court_players (to player)
		// do where on that table for where (player = home_team, home_team_id = passed in id) OR (player != home_team, home_team_id = passed in id)

	std::string tempStr = "SELECT gp.game_id, gp.play_id FROM (";
	tempStr += "SELECT pl.game_id, pl.play_id, gm.home_team_id, gm.away_team_id FROM nba_plays pl ";
	tempStr += "INNER JOIN (";
	tempStr += "SELECT game_id, home_team as home_team_id, away_team as away_team_id FROM nba_games ";
	tempStr += ") gm ";
	tempStr += "ON pl.game_id = gm.game_id";
	tempStr += ") gp ";
	tempStr += "INNER JOIN (";
	tempStr += "SELECT game_id, play_id, home_team FROM nba_on_court_players WHERE player_id = ";
	tempStr += std::to_string(req->getPID());
	tempStr += ") plyr ";
	tempStr += "ON gp.game_id = plyr.game_id AND gp.play_id = plyr.play_id AND ";
	tempStr += "((plyr.home_team = true AND (gp.home_team_id = ";
	tempStr += req->getConstraintVal(i);
	tempStr += ")) OR (plyr.home_team = false AND (gp.away_team_id = ";
	tempStr += req->getConstraintVal(i);
	tempStr += ")))";

	constrtStr->push_back(tempStr);
	
	/* SQL
	//****************************************************************************************
	SELECT gp.game_id, gp.play_id FROM (
		SELECT pl.game_id, pl.play_id, gm.home_team_id, gm.away_team_id FROM nba_plays pl
		INNER JOIN (
			SELECT game_id, home_team as home_team_id, away_team as away_team_id FROM nba_games
			) gm
		ON pl.game_id = gm.game_id
	) gp
	INNER JOIN (
		SELECT game_id, play_id, home_team FROM nba_on_court_players WHERE player_id = 196
	) plyr
	ON gp.game_id = plyr.game_id AND gp.play_id = plyr.play_id AND
	((plyr.home_team = true AND (gp.home_team_id = 11))
	OR (plyr.home_team = false AND (gp.away_team_id = 11)))
	//*****************************************************************************************
	 */
	
}

void Query::buildPlayerOpponent(DBReq * req, int i){
	// Playing against a certain team 
		// Just same as above with home_team_id/away_team_id flipped?

	std::string tempStr = "SELECT gp.game_id, gp.play_id FROM (";
	tempStr += "SELECT pl.game_id, pl.play_id, gm.home_team_id, gm.away_team_id FROM nba_plays pl ";
	tempStr += "INNER JOIN (";
	tempStr += "SELECT game_id, home_team as home_team_id, away_team as away_team_id FROM nba_games ";
	tempStr += ") gm ";
	tempStr += "ON pl.game_id = gm.game_id";
	tempStr += ") gp ";
	tempStr += "INNER JOIN (";
	tempStr += "SELECT game_id, play_id, home_team FROM nba_on_court_players WHERE player_id = ";
	tempStr += std::to_string(req->getPID());
	tempStr += ") plyr ";
	tempStr += "ON gp.game_id = plyr.game_id AND gp.play_id = plyr.play_id AND ";
	tempStr += "((plyr.home_team = true AND (gp.away_team_id = "; //**THIS LINE CHANGED FROM buildPlayerTeam**
	tempStr += req->getConstraintVal(i);
	tempStr += ")) OR (plyr.home_team = false AND (gp.home_team_id = "; //**THIS LINE CHANGED FROM buildPlayerTeam**
	tempStr += req->getConstraintVal(i);
	tempStr += ")))";

	constrtStr->push_back(tempStr);
	


	// player home team and away team_id = passed in team_id
	// OR player away team and home team_id = passed in team_id
}

void Query::buildNormalPoss(DBReq * req, int i){
	// normalPoss filters out end of quarter heaves, blowouts, ATO's, and end of game fouling situations
	// just joining nba_plays with various restrictions (can be made as simple or complicated as I like)
	// std::string tempStr = "SELECT game_id, play_id FROM nba_plays WHERE ";
	// tempStr += "period < 4 OR remaining_time > 60";

	std::string tempStr = "SELECT nba_plays.game_id, nba_plays.play_id FROM nba_plays ";
	tempStr += "INNER JOIN (";
	tempStr += "SELECT game_id, play_id FROM nba_on_court_players WHERE player_id = ";
	tempStr += std::to_string(req->getPID());
	tempStr += ") ocfilter ";
	tempStr += "ON nba_plays.game_id = ocfilter.game_id AND nba_plays.play_id = ocfilter.play_id ";
	tempStr += "WHERE period < 4 OR remaining_time > 60";

	constrtStr->push_back(tempStr);
}




std::string Query::createFullStr(DBReq * req){

	std::cout << "In createFullStr." << std::endl;
	fflush(stdout);
	
	std::string fullStr = catStr; // Starting cat SELECT
	std::string catName = "cat";
	std::string constrtName = "cstrt";
	
	// Wrap in cat and add constraint join (**make into its own function?**)
	for (int i = 0; i < req->getNumConstraints(); i++){
		// Wrap current query in a select statement whose purpose is really to give the query a table name so that it can be joined 
		std::string preStr = "SELECT ";
		preStr += catName;
		preStr += ".game_id, ";
		preStr += catName;
		preStr += ".play_id, ";
		preStr += catName;
		preStr += ".event_type FROM (";
		fullStr = preStr + fullStr;
		fullStr += ") AS ";
		fullStr += catName;
		
		// Add constraint (join)
		fullStr += " INNER JOIN (";
		fullStr += constrtStr->at(i);
		// fullStr += constrtStr->front();
		// constrtStr->pop_front(); // delete off the first constraint
		fullStr += ") AS ";
		fullStr += constrtName;
		fullStr += " ON ";
		fullStr += catName;
		fullStr += ".game_id = ";
		fullStr += constrtName;
		fullStr += ".game_id AND ";
		fullStr += catName;
		fullStr += ".play_id = ";
		fullStr += constrtName;
		fullStr += ".play_id";
		
		// Update cat name, so that it proxies a new table next loop through
		catName = "cat";
		catName += std::to_string(i + 1);
		
		// Update cnstrnt name, so that it proxies a new table next loop through
		constrtName = "cstrt";
		constrtName += std::to_string(i + 1);
	}
	
	// Wrap in final SELECT
	fullStr = "SELECT count(event_type) FROM (" + fullStr + ") AS " + catName; // I think table name at end is required?
	
	
	return fullStr;
}
