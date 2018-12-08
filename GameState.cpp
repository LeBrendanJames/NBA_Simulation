#include "GameState.h"

GameState::GameState(std::string gameDate, int gameLoc){
	homeTeamScore = 0;
	awayTeamScore = 0;
	quarter = 1;
	timeRemaining = 12 * 60;
	this->gameDate = gameDate;
	this->gameLoc = gameLoc;
}

GameState::~GameState(){
	
}

int GameState::getHomeScore(){
	return homeTeamScore;
}

int GameState::getAwayScore(){
	return awayTeamScore;
}

int GameState::getTimeRemaining() {
	return timeRemaining;
}

int GameState::getQuarter(){
	return quarter;
}

int GameState::getOffensiveTeam(){
    return offensiveTeam;
}

bool GameState::setOffensiveTeam(int team){
    if (team == 0 || team == 1){
        offensiveTeam = team;
        return true;
    } else {
        return false;
    }
}

void GameState::changeOffenseTeam(){
    offensiveTeam = 1 - offensiveTeam;
}

void GameState::updateScore(int addToScore) {
	if (offensiveTeam == 1){
		homeTeamScore += addToScore;
	} else {
		awayTeamScore += addToScore;
	}
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

void GameState::moveToNextQuarter(){
    quarter++;
    if (quarter <= 4){
        timeRemaining = 12 * 60;
    } else {
        timeRemaining = 5 * 60;
    }
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