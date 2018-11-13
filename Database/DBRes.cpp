//
// Created by Brendan on 11/13/2018.
//

#include "DBRes.h"


// PlayerRes constructor
PlayerRes::PlayerRes(){
    pID = 0;
}

// PlayerRes destructor
PlayerRes::~PlayerRes() = default;

// SET PID
void PlayerRes::addPID(int pID){
    this->pID = pID;
}

// SET resVal
void PlayerRes::addResVal(double val){
    this->resVals.push_back(val);
}

// GET resVals Size
int PlayerRes::getResValsSize(){
    return this->resVals.size();
}

// GET PID
int PlayerRes::getPID(){
    return this->pID;
}

// GET resVal
double PlayerRes::getResVal(int resValIndex){
    return this->resVals[resValIndex];
}

// DBRes constructor
DBRes::DBRes(){

}

// DBRes destructor
DBRes::~DBRes(){

}

// GET playerRes
void DBRes::getPlayerRes(PlayerRes * newPlayerRes, int resNum){
    newPlayerRes->addPID(this->playerRes[resNum]->getPID());
    for (int i = 0; i < this->playerRes[resNum]->getResValsSize(); i++){
        newPlayerRes->addResVal(this->playerRes[resNum]->getResVal(i));
    }

    return;
}

// SET playerResObj
void DBRes::addPlayerRes(){
    PlayerRes * newRes = new PlayerRes;
    playerRes.push_back(newRes);
}

// SET playerResVal at back
void DBRes::addPlayerResVal(double resVal){
    playerRes[playerRes.size() - 1]->addResVal(resVal);
}