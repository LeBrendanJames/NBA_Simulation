//
// Created by Brendan on 11/10/2018.
//

#ifndef SIMULATION_ADV_STATS_H
#define SIMULATION_ADV_STATS_H

// Stat template
	// Will be used by various classes to hold a standard template of stats

class AdvStats {
public:
	// Constructor/destructor
	AdvStats();
	~AdvStats();
	
	// Setters w/ error checking
	bool setShotFreq(double shotFreq); // **Could also add 'int possType' here to make setters into an array that holds stat for different possession types
	bool setShot3PtFreq(double shot3PtFreq);
	bool setTovFreq(double tovFreq);
	bool setDrawFoulFreq(double drawFoulFreq);
	bool setDrebFreq(double drebFreq);
	bool setOrebFreq(double orebFreq);
	bool setStlFreq(double stlFreq);
	bool setBlkFreq(double blkFreq);
	bool setNumOffPlays(int numOffPlays);
	bool setNumDefPlays(int numDefPlays);
	
	// Getters
	double getShotFreq();
	double getShot3PtFreq();
	double getTovFreq();
	double getDrawFoulFreq();
	double getDrebFreq();
	double getOrebFreq();
	double getStlFreq();
	double getBlkFreq();
	int getNumOffPlays();
	int getNumDefPlays();
	
	double calcTotalOffProb();
	
private:
	double shotFreq;
	double shot3PtFreq;
    double tovFreq;
    double drawFoulFreq;
    double drebFreq;
    double orebFreq;
	double stlFreq;
	double blkFreq;
	int numOffPlays; // For sample size
	int numDefPlays; // For sample size
};


#endif //SIMULATION_ADV_STATS_H