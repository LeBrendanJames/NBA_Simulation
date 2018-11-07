
#include "AdvStats.h"


AdvStats::AdvStats(){
	
}

AdvStats::~AdvStats(){
	
}


double AdvStats::calcTotalProb(){
	return shootPct + shoot3PtPct + tovPct + drawFoulPct;
}