//
// Created by Brendan on 6/26/2018.
//

#include "team.h"


Team::Team(){
	
}

Team::Team(DBInterface * dbFace, std::string teamCode, std::string gameDate){
    this->dbFace = dbFace;

    // convert team code to teamID and load roster as of game date into players array

}

Team::~Team(){
	
}