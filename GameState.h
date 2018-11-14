//
// Created by Brendan on 11/14/2018.
//

#ifndef SIMULATION_GAME_STATE_H
#define SIMULATION_GAME_STATE_H

#include <string>

class GameState {
public:
	GameState(std::sting gameDate, int gameLoc);
	~GameState();
	
	int getHomeScore();
	int getAwayScore();

	void updateHomeScore(int addToScore);
	void updateAwayScore(int addToScore);
	void updateQuarter();
	void updateTimeRemaining(int timeToSubtract);
	
	bool isTied();
	bool is2for1();
	
private:
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	int timeRemaining; // Seconds left in quarter 
	std::string gameDate
	int gameLoc
};


#endif //SIMULATION_GAME_STATE_H