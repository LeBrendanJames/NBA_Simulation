//
// Created by Brendan on 6/26/2018.
//

#include "player.h"


// Default constructor
Player::Player(){
	playerID = 0;
	
	startDate = new struct tm;
    setDate(startDate, 1, 1, 1970);
	
	endDate = new struct tm;
	setDate(endDate, 1, 1, 3000);
	
	
}

Player::Player(int pID){
	playerID = pID;
	
	startDate = new struct tm;
    setDate(startDate, 1, 1, 1970);
	
	endDate = new struct tm;
	setDate(endDate, 1, 1, 3000);
}

Player::Player(int pID, struct tm * startDate, struct tm * endDate){
	playerID = pID;
	this->startDate = startDate;
	this->endDate = endDate;
	
	// **Maybe chaeck that the passed in start and end date are valid??**
}



void setDate(struct tm * date, int day, int month, int year){
	date->tm_mday = day; // Day
    date->tm_mon = month - 1; // Months since January
    date->tm_year = year - 1900; // Years since 1900
	
	return;
}



// GETTERS
float getShootPct(){
	return shootPct;
}

float getTovPct(){
	return tovPct;
}

float getDrawFoulPct(){
	return drawFoulPct;
}

float getDrebPct(){
	return drebPct;
}

float getOrebPct(){
	return orebPct;
}
