#include "GameState.h"
#include <iostream>

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

GameState::GameState(const GameState &gameState){
	this->homeTeamScore = gameState.homeTeamScore;
	this->awayTeamScore = gameState.awayTeamScore;
	this->quarter = gameState.quarter;
	this->timeRemaining = gameState.timeRemaining;
	this->gameDate = gameState.gameDate;
	this->gameLoc = gameState.gameLoc;
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

std::string GameState::getGameDate() {
	return gameDate;
}

bool GameState::setOffensiveTeam(int team){
    if (team == 0 || team == 1){
        offensiveTeam = team;
        std::cout << "Offensive team set to " << team << std::endl;
        return true;
    } else {
    	std::cout << "Offensive team not set." << std::endl;
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

void GameState::moveToNextQuarter(){
    quarter++;
    if (quarter <= 4){
        timeRemaining = 12 * 60;
    } else {
        timeRemaining = 5 * 60;
    }
}

void GameState::updateTimeRemaining(int timeToSubtract){
	if (timeToSubtract > 0) { //&& timeToSubtract <= timeRemaining){
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