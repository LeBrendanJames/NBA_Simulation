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
	// TODO: Replace with an actual jumpball simulation between the two team's centers
	gmState->setOffensiveTeam(rand() % 2);
	
	do {
		// Loop for quarter
		while (gmState->getTimeRemaining() > 0){
			// Determine what happens
			determinePossOutcome();
			
			// Flip team in possession
			gmState->changeOffenseTeam(); // Flips between 0 and 1
			
			// Determine any action between possessions. This is where stats will be recalculated if changes are large enough to trigger that.
			determineInterPossChgs();
		}
		
		// Update to next quarter (quarter number + time remaining)
		gmState->moveToNextQuarter();

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

	double totalPct = onCourtPlyrs->getTotalFrequencies(gmState) * 100; // Easier to work with percentage as full number
	
	int action = rand() % static_cast<int>(totalPct) + 1; // Determine action

	// Figure out what action was and who did it. Do stuff based on that.
	totalPct = 0;
	int i = 0;
	bool isMatch = false;
	// shooting
	while (i < 5 && !isMatch){
		totalPct += onCourtPlyrs->getTotalShotFreq(gmState);
		if (totalPct * 100 >= action){
			isMatch = true;
		}
		i++;
	}
	if (isMatch){
		int shotMake = rand() % 100 + 1;
		if (shotMake >= onCourtPlyrs->getFGPct(gmState)){
			gmState->updateScore(2);
		}
	}
	// turnover
	i = 0;
	while (i < 5 && !isMatch){
		totalPct += onCourtPlyrs->getTotalTOVFreq(gmState);
		if (totalPct * 100 >= action){
			isMatch = true;
		}
		i++;
	}
	
	if (!isMatch){ // Meaning the only available option left is drawing a foul
		// draw foul (making assumption of 2 ft shots @ 80% FT%)
		int ftPts = rand() % 100 + 1;
        if (ftPts <= 64){
            gmState->updateScore(2);
        } else if (ftPts <= 96){
            gmState->updateScore(1);
        }
	}

	updateTime();
}

// **Very simplistic taking off 16 seconds each time. Just for testing - obviously needs to be way more sophisticated.**
void Game::updateTime(){
	gmState->updateTimeRemaining(16);
}

void Game::determineInterPossChgs(){
	// Player subs, etc. Nothing that I'm doing right now.
}

