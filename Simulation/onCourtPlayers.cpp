
#include "onCourtPlayers.h"


OnCourtPlayers::OnCourtPlayers(Lineup * homeLineup, Lineup * awayLineup){
	this->homeLineup = homeLineup;
	this->awayLineup = awayLineup;

	homeStats = new AdvStats *[5];
	for (int i = 0; i < 5; i++){
	    homeStats[i] = new AdvStats;
	}
	awayStats = new AdvStats *[5];
	for (int i = 0; i < 5; i++){
	    awayStats[i] = new AdvStats;
	}

    calcStats();
}

OnCourtPlayers::OnCourtPlayers(const OnCourtPlayers &onCourtPlyrs) {
    this->homeLineup = onCourtPlyrs.homeLineup;
    this->awayLineup = onCourtPlyrs.awayLineup;

    homeStats = new AdvStats *[5];
    for (int i = 0; i < 5; i++){
        homeStats[i] = new AdvStats(*onCourtPlyrs.homeStats[i]);
    }

    awayStats = new AdvStats *[5];
    for (int i = 0; i < 5; i++){
        awayStats[i] = new AdvStats(*onCourtPlyrs.awayStats[i]);
    }
}

OnCourtPlayers::~OnCourtPlayers(){
	for (int i = 0; i < 5; i++){
	    delete homeStats[i];
	    delete awayStats[i];
	}
	delete homeStats;
	delete awayStats;
}


void OnCourtPlayers::calcStats(){
    // For now, just copying the AdvStats up from each player
    // Will have more work here in the future to look at lineup vs opponent lineup effects on stats
    for (int i = 0; i < 5; i++) { // TODO: Figure out if there's a better way to do this w/ copy constructor
        setAdvStats(homeStats, homeLineup, i);
        setAdvStats(awayStats, awayLineup, i);
    }
}

void OnCourtPlayers::setAdvStats(AdvStats ** stats, Lineup * lineup, int lineupPosition){
    stats[lineupPosition]->setShotFreq(lineup->getShotFreq(lineupPosition));
    stats[lineupPosition]->setShot3PtFreq(lineup->getShot3PtFreq(lineupPosition));
    stats[lineupPosition]->setTovFreq(lineup->getTovFreq(lineupPosition));
    stats[lineupPosition]->setDrawFoulFreq(lineup->getDrawFoulFreq(lineupPosition));
    stats[lineupPosition]->setDrebFreq(lineup->getDrebFreq(lineupPosition));
    stats[lineupPosition]->setOrebFreq(lineup->getOrebFreq(lineupPosition));
    stats[lineupPosition]->setStlFreq(lineup->getStlFreq(lineupPosition));
    stats[lineupPosition]->setBlkFreq(lineup->getBlkFreq(lineupPosition));
    stats[lineupPosition]->setNumOffPlays(lineup->getNumOffPlays(lineupPosition));
    stats[lineupPosition]->setNumDefPlays(lineup->getNumDefPlays(lineupPosition));

    stats[lineupPosition]->setShotPct(lineup->getShotPct(lineupPosition));
    stats[lineupPosition]->setThreePtPct(lineup->getThreePtPct(lineupPosition));
}

void OnCourtPlayers::setOffTeamPtr(AdvStats *** statsPtr, GameState * gmState){
    if (gmState->getOffensiveTeam() == 1){
        *statsPtr = homeStats;
    } else {
        *statsPtr = awayStats;
    }
}

double OnCourtPlayers::getTotalFrequencies(GameState * gmState){
    AdvStats ** statsPtr;
    setOffTeamPtr(&statsPtr, gmState);

    double totalProb = 0.0;
    for (int i = 0; i < 5; i++){
        totalProb += statsPtr[i]->calcTotalOffProb();
    }

    return totalProb;
}

double OnCourtPlayers::getTotalShotFreq(GameState * gmState){
    AdvStats ** statsPtr;
    setOffTeamPtr(&statsPtr, gmState);

    double shotFreq = 0.0;
    for (int i = 0; i < 5; i++){
        shotFreq += statsPtr[i]->getShotFreq();
    }

    return shotFreq;
}

double OnCourtPlayers::getFGPct(GameState * gmState){
    AdvStats ** statsPtr;
    setOffTeamPtr(&statsPtr, gmState);

    double fgPct = 0.0;
    for (int i = 0; i < 5; i++){
        fgPct += statsPtr[i]->getShotPct();
    }

    return fgPct;
}

double OnCourtPlayers::getTotalTOVFreq(GameState * gmState){
    AdvStats ** statsPtr;
    setOffTeamPtr(&statsPtr, gmState);

    double tovPct = 0.0;
    for (int i = 0; i < 5; i++){
        tovPct += statsPtr[i]->getTovFreq();
    }

    return tovPct;
}
