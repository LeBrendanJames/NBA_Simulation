//
// Created by Brendan on 6/26/2018.
//

#include "game.h"
#include <stdlib.h>
#include <time.h>
#include "team.h"
#include "onCourtPlayers.h"


Game::Game(){
	this->homeTeam = NULL;
	this->awayTeam = NULL;
	
	srand(time(NULL));
}

Game::Game(DBInterface * dbFace, std::string homeTeam, std::string awayTeam, std::string gameDate, std::string gameLoc){
	this->dbFace = dbFace;
	this->homeTeam = new Team(dbFace, homeTeam, gameDate);
	this->awayTeam = new Team(dbFace, awayTeam, gameDate);
	this->onCourtPlyrs = new OnCourtPlayers(this->homeTeam->getStartingLineup(), this->awayTeam->getStartingLineup());
	
	gmState = new GameState(gameDate, dbFace->getTeamIDFromCode(gameLoc));
	
	srand(time(NULL));
}

Game::~Game(){
	
}


void Game::simGame(){
	// Randomly determine who has possession first
	int possTeam = rand() % 2;
	
	do {
		// Loop for quarter
		while (gmState->getTimeRemaining() > 0){
			// Determine what happens
			determinePossOutcome();
			
			// Flip team in possession
			possTeam = 1 - possTeam; // Flips between 0 and 1
			
			// Determine any action between possessions. This is where stats will be recalculated if changes are large enough to trigger that.
			determineInterPossessionChanges();
		}
		
		// Update to next quarter, setting time based on whether 1 - 4 or OT
		gmState->updateQuarter();
		if (gmState->getQuarter() <= 4){
			bool OT = false;
			gmState->resetTimeRemaining(OT);
		} else {
			bool OT = true;
			gmState->resetTimeRemaining(OT);
		}
	} while (gmState->getQuarter() <= 4 || gmState->getHomeScore() == gmState->getAwayScore());
	
}

int Game::getHomeScore(){
	return gmState->getHomeScore();
}

int Game::getAwayScore(){
	return gmState->getAwayScore();
}




// This should be the function that houses all of the logic that i'll build up over time 
void Game::determinePossOutcome(){ //**Pass possession type here (ENUM?)**
	// **Want to be able to just call functions from onCourtPlayers and pass offensiveTeam from gmState**
	// **So, won't need any of this offensiveLineup business**
	Lineup * offensiveLineup = NULL;
	if (possTeam == 0){
		offensiveLineup = homeLineup;
	} else {
		offensiveLineup = awayLineup;
	}
	
	double totalPercentage = 0;
	for (int i = 0; i < 5; i++){
		totalPercentage += offensiveLineup[i]->getShootPct();
		totalPercentage += offensiveLineup[i]->getTovPct();
		totalPercentage += offensiveLineup[i]->getDrawFoulPct();
	}
	totalPercentage *= 100; // Easier to work with percentages as whole numbers
	
	int action = rand() % totalPercentage + 1; // Determine action 
	
	// Figure out what action was and who did it. Do stuff based on that.
	totalPercentage = 0;
	int i = 0;
	bool isMatch = false;
	// shooting
	while (i < 5 && !isMatch){
		totalPercentage += offensiveLineup[i]->getShootPct();
		if (totalPercentage * 100 >= action){
			isMatch = true;
		}
		i++;
	}
	if (isMatch){
		int shotMake = rand() % 100 + 1;
		if (shotMake >= offensiveLineup[i]->getFGPct()){
			gmState->updateScore(2);
			/*if (possTeam == 0){
				gmState->updateHomeScore(2);
			} else {
				gmState->updateAwayScore(2);
			}*/
		}
	}
	// turnover
	i = 0;
	while (i < 5 && !isMatch){
		totalPercentage += offensiveLineup[i]->getTovPct();
		if (totalPercentage * 100 >= action){
			isMatch = true;
		}
		i++;
	}
	
	if (!isMatch){ // Meaning the only available option left is drawing a foul
		// draw foul (making assumption of 2 ft shots @ 80% FT%)
		int ftPts = rand() % 100 + 1;
		if (possTeam == 0){
			if (ftPts <= 64){
				gmState->updateHomeScore(2);
			} else if (ftPts <= 96){
				gmState->updateHomeScore(1);
			}
		} else {
			if (ftPts <= 64){
				gmState->updateAwayScore(2);
			} else if (ftPts <= 96){
				gmState->updateAwayScore(1);
			}
		}
	}
	
	// updateTime
	updateTime();
}

// ***Very simplistic taking off 16 seconds each time. Just for testing - obviously needs to be way more sophisticated.***
void Game::updateTime(){
	gmState->updateTimeRemaining(16);
}

void Game::determineInterPossessionChanges(){
	// Player subs, etc. Nothing that I'm doing right now.
}

