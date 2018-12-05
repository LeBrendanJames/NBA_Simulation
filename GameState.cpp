#include "GameState.h"

GameState::GameState(std::sting gameDate, int gameLoc){
	homeTeamScore = 0;
	awayTeamScore = 0;
	quarter = 1;
	timeRemaining = 12 * 60;
	this->gameDate = gameDate;
	this->gameLoc = gameLoc; // getTeamIDFromCode(gameLoc);
}

GameState::~GameState(){
	
}

int GameState::getHomeScore(){
	return homeTeamScore;
}

int getAwayScore(){
	return awayTeamScore;
}

void GameState::updateHomeScore(int addToScore){
	if (addToScore > 0 && addToScore <= 4){
		homeTeamScore += addToScore;
	}
}

void GameState::updateAwayScore(int addToScore){
	if (addToScore > 0 && addToScore <= 4){
		awayTeamScore += addToScore;
	}
}

void GameState::updateQuarter(){
	quarter++;
}

void GameState::updateTimeRemaining(int timeToSubtract){
	if (timeToSubtract >0 && timeToSubtract <= timeRemaining){
		timeRemaining -= timeToSubtract;
	}
}

bool GameState::isTied(){
	if (homeTeamScore == awayTeamScore){
		return true;
	} else {
		return false;
	}
}

bool GameState::is2for1(){
	if (timeRemaining < 50){
		return true;
	} else {
		return false;
	}
}