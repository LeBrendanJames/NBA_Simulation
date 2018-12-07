//
// Created by Brendan on 6/26/2018.
//

#include "team.h"


Team::Team(){
	
}

Team::Team(DBInterface * dbFace, std::string teamCode, std::string gameDate){
    this->dbFace = dbFace;

    // convert team code to teamID and load roster as of game date into players array
    //**Hard coding two teams for now since database isn't set up for this**
    std::vector<int> playerIDs;
    if (teamCode == "GSW"){
        playerIDs = {100, 100, 100, 100, 100,
                        100, 100, 100, 100, 100,
                        100, 100, 100, 100, 100};
    } else if (teamCode == "MIL"){
        playerIDs = {100, 100, 100, 100, 100,
                       100, 100, 100, 100, 100,
                       100, 100, 100, 100, 100};
    }

    players = new Player *[15];
    for (int i = 0; i < playerIDs.size(); i++){
        players[i] = new Player(dbFace, playerIDs[i]);
    }

    startingLineup = new Lineup(players[0], players[1], players[2], players[3], players[4]);

}

Team::~Team(){
	// Delete allocated players & pointer array

}


Lineup * Team::getStartingLineup() {
    return this->startingLineup;
}