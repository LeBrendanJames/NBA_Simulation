//
// Created by Brendan on 6/26/2018.
//

#ifndef SIMULATION_ADV_STATS_H
#define SIMULATION_ADV_STATS_H

// Stat template
	// Will be used by various classes to hold stats at different levels of predictive power

class AdvStats {
private:
	double shootPct;
	double shoot3PtPct;
    double tovPct;
    double drawFoulPct;
    double drebPct;
    double orebPct;
	
public:
	AdvStats();
	~AdvStats();
	
	double calcTotalOffProb();
	
};


#endif //SIMULATION_ADV_STATS_H