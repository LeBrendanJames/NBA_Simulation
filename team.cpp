//
// Created by Brendan on 6/26/2018.
//

#include "team.h"


Team::Team(){
	
}

Team::Team(DBInterface * dbFace, std::string teamCode, GameState * gmState){
    this->dbFace = dbFace;

    // convert team code to teamID and load roster as of game date into players array
    //**Hard coding two teams for now since database isn't set up for this**
    std::vector<int> playerIDs;
    if (teamCode == "GSW"){
        // Stephen Curry, Klay Thompson, Harrison Barnes, Draymond Green, Andrew Bogut,
        // Leandro Barbosa, Ian Clark, Festus Ezili, Andre Iguodala, Shaun Livingston,
        // James Michael McAdoo, Brandon Rush, Marreese Speights, Jason Thompson, Anderson Varejao
        playerIDs = {436, 283, 174, 126, 26,
                     302, 180, 150, 21, 426,
                     199, 54, 326, 212, 18};
    } else if (teamCode == "MIL"){
        // Michael Carter-Williams, Khris Middleton, Giannis Antetokounmpo, Jabari Parker, Greg Monroe,
        // Jerryd Bayless, O.J. Mayo, Rashad Vaughn, John Henson, Miles Plumlee,
        // Johnny O'Bryant, Tyler Ennis, Greivis Vasquez, Chris Copeland, Damien Inglis
        playerIDs = {336, 280, 161, 188, 170,
                     227, 365, 1789, 241, 343,
                     246, 468, 173, 73, 1750};
    }
    /*std::cout << "playerIDs: ";
    for (int i = 0; i < 15; i++){
        std::cout << playerIDs[i] << ",";
    }
    std::cout << std::endl;*/

    players = new Player *[15];
    for (int i = 0; i < playerIDs.size(); i++){
        players[i] = new Player(dbFace, playerIDs[i], gmState);
    }

    startingLineup = new Lineup(players[0], players[1], players[2], players[3], players[4]);

}

Team::~Team(){
	// Delete allocated players & pointer array

}


Lineup * Team::getStartingLineup() {
    return this->startingLineup;
}