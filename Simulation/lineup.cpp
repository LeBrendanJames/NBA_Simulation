//
// Created by Brendan on 6/26/2018.
//

#include "lineup.h"


Lineup::Lineup(Player * plyr1, Player * plyr2, Player * plyr3, Player * plyr4, Player * plyr5){
	this->lineup = new Player * [5];
	lineup[0] = plyr1;
	lineup[1] = plyr2;
	lineup[2] = plyr3;
	lineup[3] = plyr4;
	lineup[4] = plyr5;

	stats = new AdvStats *[5];
	for (int i = 0; i < 5; i++){
		stats[i] = new AdvStats;
	}

	calcLineupStats();
}

Lineup::Lineup(const Lineup &lineup){
	// Set up player pointers (setting to NULL)
	this->lineup = new Player *[5];
	for (int i = 0; i < 5; i++){
		this->lineup[i] = NULL;
	}

	// Copy over stats
	this->stats = new AdvStats *[5];
	for (int i = 0; i < 5; i++){
		stats[i] = new AdvStats(*lineup.stats[i]);
	}
}

Lineup::~Lineup(){
	delete [] this->lineup;

	for (int i = 0; i < 5; i++){
		delete stats[i];
	}
	delete stats;
}


void Lineup::calcLineupStats(){
	// Copy over from player's AdvStats to lineup AdvStats
	// Eventually, lineup's AdvStats will incorporate more sophisticated lineup considerations, but just copying up for now
	for (int i = 0; i < 5; i++) { // TODO: Figure out if there's a better way to do this w/ copy constructor
		stats[i]->setShotFreq(lineup[i]->getShotFreq());
		stats[i]->setShot3PtFreq(lineup[i]->getShot3PtFreq());
		stats[i]->setTovFreq(lineup[i]->getTovFreq());
		stats[i]->setDrawFoulFreq(lineup[i]->getDrawFoulFreq());
		stats[i]->setDrebFreq(lineup[i]->getDrebFreq());
		stats[i]->setOrebFreq(lineup[i]->getOrebFreq());
		stats[i]->setStlFreq(lineup[i]->getStlFreq());
		stats[i]->setBlkFreq(lineup[i]->getBlkFreq());
		stats[i]->setNumOffPlays(lineup[i]->getNumOffPlays());
		stats[i]->setNumDefPlays(lineup[i]->getNumDefPlays());

		stats[i]->setShotPct(lineup[i]->getShotPct());
		stats[i]->setThreePtPct(lineup[i]->getThreePtPct());
	}
}


double Lineup::getShotFreq(int lineupPosition){
	return lineup[lineupPosition]->getShotFreq();
}

double Lineup::getShot3PtFreq(int lineupPosition){
	return lineup[lineupPosition]->getShot3PtFreq();
}

double Lineup::getTovFreq(int lineupPosition){
	return lineup[lineupPosition]->getTovFreq();
}

double Lineup::getDrawFoulFreq(int lineupPosition){
	return lineup[lineupPosition]->getDrawFoulFreq();
}

double Lineup::getDrebFreq(int lineupPosition){
	return lineup[lineupPosition]->getDrebFreq();
}

double Lineup::getOrebFreq(int lineupPosition){
	return lineup[lineupPosition]->getOrebFreq();
}

double Lineup::getStlFreq(int lineupPosition){
	return lineup[lineupPosition]->getStlFreq();
}

double Lineup::getBlkFreq(int lineupPosition){
	return lineup[lineupPosition]->getBlkFreq();
}

int Lineup::getNumOffPlays(int lineupPosition){
	return lineup[lineupPosition]->getNumOffPlays();
}

int Lineup::getNumDefPlays(int lineupPosition){
	return lineup[lineupPosition]->getNumDefPlays();
}

double Lineup::getShotPct(int lineupPosition){
	return lineup[lineupPosition]->getShotPct();
}

double Lineup::getThreePtPct(int lineupPosition){
	return lineup[lineupPosition]->getThreePtPct();
}

void Lineup::setNewPlayers(Player * plyr0, Player * plyr1, Player * plyr2, Player * plyr3, Player * plyr4) {
	this->lineup[0] = plyr0;
	this->lineup[1] = plyr1;
	this->lineup[2] = plyr2;
	this->lineup[3] = plyr3;
	this->lineup[4] = plyr4;
}