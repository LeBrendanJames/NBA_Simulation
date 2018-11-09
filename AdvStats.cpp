
#include "AdvStats.h"


AdvStats::AdvStats(){
	shotFreq = 0.0;
	shot3PtFreq = 0.0;
    tovFreq = 0.0;
    drawFoulFreq = 0.0;
    drebFreq = 0.0;
    orebFreq = 0.0;
	
	numOffPlays = 0.0;
	numDefPlays = 0.0;
}

AdvStats::~AdvStats(){
	
}

// Setters (w/ error checking?)
bool AdvStats::setShotFreq(double shotFreq){
	if (shotFreq > 0.0 && shotFreq < 1.0){
		this->shotFreq = shotFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setShot3PtFreq(double shot3PtFreq){
	if (shot3PtFreq > 0.0 && shot3PtFreq < 1.0){
		this->shot3PtFreq = shot3PtFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setTovFreq(double tovFreq){
	if (tovFreq > 0.0 && tovFreq < 1.0){
		this->tovFreq = tovFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setDrawFoulFreq(double drawFoulFreq){
	if (drawFoulFreq > 0.0 && drawFoulFreq < 1.0){
		this->drawFoulFreq = drawFoulFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setDrebFreq(double drebFreq){
	if (drebFreq > 0.0 && drebFreq < 1.0){
		this->drebFreq = drebFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setOrebFreq(double orebFreq){
	if (orebFreq > 0.0 && orebFreq < 1.0){
		this->orebFreq = orebFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setNumOffPlays(int numOffPlays){
	if (numOffPlays > 0){
		this->numOffPlays = numOffPlays;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setNumDefPlays(int numDefPlays){
	if (numDefPlays > 0){
		this->numDefPlays = numDefPlays;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setStlFreq(double stlFreq){
	if (stlFreq > 0.0 && stlFreq < 1.0){
		this->stlFreq = stlFreq;
		return true;
	} else {
		return false;
	}
}

bool AdvStats::setBlkFreq(double blkFreq){
	if (blkFreq > 0.0 && blkFreq < 1.0){
		this->blkFreq = blkFreq;
		return true;
	} else {
		return false;
	}
}


// Getters
double AdvStats::getShotFreq(){
	return shotFreq;
}

double AdvStats::getShot3PtFreq(){
	return shot3PtFreq;
}

double AdvStats::getTovFreq(){
	return tovFreq;
}

double AdvStats::getDrawFoulFreq(){
	return drawFoulFreq;
}

double AdvStats::getDrebFreq(){
	return drebFreq;
}

double AdvStats::getOrebFreq(){
	return orebFreq;
}

int AdvStats::getNumOffPlays(){
	return numOffPlays;
}

int AdvStats::getNumDefPlays(){
	return numDefPlays;
}


double AdvStats::calcTotalProb(){
	return shotFreq + shot3PtFreq + tovFreq + drawFoulFreq;
}