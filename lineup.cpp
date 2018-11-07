//
// Created by Brendan on 6/26/2018.
//


Lineup::Lineup(Player * plyr1, Player * plyr2, Player * plyr3, Player * plyr4, Player * plyr5){
	this->lineup = new Player * [5];
	lineup[0] = plyr1;
	lineup[1] = plyr2;
	lineup[2] = plyr3;
	lineup[3] = plyr4;
	lineup[4] = plyr5;
}

Lineup::~Lineup(){
	delete [] this->lineup;
}

