#include <iostream>
#include "Database/DBInterface.h"
#include "player.h"
#include "game.h"

/*
int main() {

    // Create DB Class, which opens connection 
    auto * dbFace = new DBInterface;
	
    // Create player, passing in DB class
    auto * playerOne = new Player(dbFace, 196); // 196 = James Harden
    
    // Get player FGA (for now - eventually this will calc everything and place in private variables)
    playerOne->calcPriors();

    // Print FGA
    std::cout << playerOne->getShootPct() << std::endl;

    delete playerOne;
    delete dbFace;

    return 0;
}
*/

int main(){
	// make DB connection
	auto * dbFace = new DBInterface;
	
	// Take user input to select teams
		// I'll need to have code to go from team names to current rosters
	std::string teamA, teamB;
	std::cout << "What is the first team's code? " << std::endl;
	std::cin >> teamA;
	std::cout << "What is the second team's code? " << std::endl;
	std::cin >> teamB;
	
	// Get user input for which team is at home 
	int homeTeam = 0;
	std::cout << "Which team will be the home team?" << std::endl;
	std::cout << "Enter '1' for " << teamA << ", '2' for " << teamB << ", or '0' for neutral: " << std::endl;
	std::cin >> homeTeam;
	
	// Take user input for game date
	std::stirng gameDate;
	std::cout << "What date is the game played (MM/DD/YYYY)? " << std::endl;
	std::cin >> gameDate;
	
	// Take user input for number of simulations
	int numSims = 0;
	std::cout << "How many simulations would you like to run? " << std::endl;
	std::cin >> numSims;
	
	// Set up score differences vector
	auto * scoreDifferences = new std::vector<int>;
	
	// Run sims 
	for (int i = 0; i < numSims; i++){
		// initialize game
		auto * curGm = new Game(dbFace, teamA, teamB, gameDate, gameLoc, homeTeam);
		
		// simGame
		curGm->simGame(); // returns gameState pointer
		
		// record results
		scoreDifferences->push_back(curGm->getHomeScore() - curGm->getAwayScore()); // have vector of score differences
		
		// delete game 
		delete curGm;
	}
	
	// Determine spread
	// Sort vector
	sort(scoreDifferences->begin(), scoreDifferences->end());
	// find middle
	if (scoreDifferences->size() % 2 != 0){ // odd number of sims
		double spreadNum = static_cast<double>(scoreDifferences[scoreDifferences->size() / 2]) // integer division by design
	} else { // even 
		double spreadNum = (scoreDifferences[scoreDifferences->size() / 2] + scoreDifferences[scoreDifferences->size() / 2 + 1]) / 2.0
	}
	
	// set to string
	if (spreadNum < 0){
		std::string spread = std::to_string(spreadNum);
	} else if (spreadNum > 0){
		std::string spread = '+' + std::to_string(spreadNum);
	} else {
		std::string spread = "pk"; 
	}
	
	// Print spread 
	std::cout << homeTeamName << " " << spread << std::endl;
	
	// delete scoreDifferences vector and database interface object 
	delete scoreDifferences;
	delete dbFace;
	
	return 0;
}


