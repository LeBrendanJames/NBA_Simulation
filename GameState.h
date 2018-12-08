//
// Created by Brendan on 11/14/2018.
//

#ifndef SIMULATION_GAME_STATE_H
#define SIMULATION_GAME_STATE_H

#include <string>

class GameState {
public:
	GameState(std::string gameDate, int gameLoc);
	~GameState();
	
	int getHomeScore();
	int getAwayScore();
	int getTimeRemaining();
	int getQuarter();
	int getOffensiveTeam();

    bool setOffensiveTeam(int team);
    void changeOffenseTeam();

	void updateScore(int addToScore); // Figures out which team to update based on offensiveTeam variable
	void updateHomeScore(int addToScore);
	void updateAwayScore(int addToScore);
	void moveToNextQuarter();
	void updateTimeRemaining(int timeToSubtract);
	
	bool isTied();
	bool is2for1();
	
private:
	int homeTeamScore;
	int awayTeamScore;
	int quarter;
	int timeRemaining; // Seconds left in quarter
	int offensiveTeam; // Which team in on offense (1 = home, 0 = away)
	std::string gameDate;
	int gameLoc;
};


#endif //SIMULATION_GAME_STATE_H