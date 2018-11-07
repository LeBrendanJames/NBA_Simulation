//
// Created by Brendan on 6/26/2018.
//

#include "game.h"
#include <stdlib.h>
#include <time.h>


Game::Game(){
	this->homeLineup = NULL;
	this->awayLineup = NULL;
	
	homeTeamScore = 0;
	awayTeamScore = 0;
	quarter = 1;
	timeRemaining = 12 * 60;
	
	srand(time(NULL));
}

Game::Game(std::string homeTeam, std::string awayTeam, std::string gameDate){
	this->homeLineup = homeLineup;
	this->awayLineup = awayLineup;
	
	// Set initial game state 
	homeTeamScore = 0;
	awayTeamScore = 0;
	quarter = 1;
	timeRemaining = 12 * 60;
	
	srand(time(NULL));
}

Game::~Game(){
	
}


int Game::simGame(){
	// Randomly determine who has possession first
	int possTeam = rand() % 2;
	
	do {
		// Loop for quarter
		while (timeRemaining > 0){
			// Determine what happens
			determinePossOutcome();
			
			// Flip team in possession
			possTeam = 1 - possTeam; // Flips between 0 and 1
			
			// Determine any action between possessions. This is where stats will be recalculated if changes are large enough to trigger that.
			determineInterPossessionChanges();
		}
		
		// Update to next quarter, setting time based on whether 1 - 4 or OT
		quarter++;
		if (quarter <= 4){
			timeRemaining = 12 * 60;
		} else {
			timeRemaining = 5 * 60;
		}
	} while (quarter <= 4 || homeTeamScore == awayTeamScore);
	
	// Return results up to another object or back out to main?
	return homeTeamScore - awayTeamScore;
}




// This should be the function that houses all of the logic that i'll build up over time 
void Game::determinePossOutcome(){
	Lineup * offensiveLineup = NULL;
	if (possTeam == 0){
		offensiveLineup = homeLineup;
	} else {
		offensiveLineup = awayLineup;
	}
	
	double totalPercentage = 0
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
	bool match = false;
	// shooting
	while (i < 5 && match = false){
		totalPercentage += offensiveLineup[i]->getShootPct();
		if (totalPercentage * 100 >= action){
			match = true;
		}
		i++;
	}
	if (match == true){
		int shotMake = rand() % 100 + 1;
		if (shotMake >= offensiveLineup[i]->getFGPct()){
			if (possTeam == 0){
				homeTeamScore += 2;
			} else {
				awayTeamScore += 2;
			}
		}
	}
	// turnover
	i = 0;
	while (i < 5 && match = false){
		totalPercentage += offensiveLineup[i]->getTovPct();
		if (totalPercentage * 100 >= action){
			match = true;
		}
		i++;
	}
	
	if (match == false){ // Meaning the only available option left is drawing a foul 
		// draw foul (making assumption of 2 ft shots @ 80% FT%)
		ftPts = rand() % 100 + 1;
		if (possTeam == 0){
			if (ftPts <= 64){
				homeTeamScore += 2;
			} else if (ftPts <= 96){
				homeTeamScore += 1;
			}
		} else {
			if (ftPts <= 64){
				awayTeamScore += 2;
			} else if (ftPts <= 96){
				awayTeamScore += 1;
			}
		}
	}
	
	// updateTime
	updateTime();
}

// ***Very simplistic taking off 16 seconds each time. Just for testing - obviously needs to be way more sophisticated.***
void Game::updateTime(){
	timeRemaining -= 16;
}

void Game::determineInterPossessionChanges(){
	// Player subs, etc. Nothing that I'm doing right now.
}

