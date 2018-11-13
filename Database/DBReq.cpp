//
// Created by Brendan on 11/13/2018.
//

#include "DBReq.h"
#include <string>

// Constraint constructor
Constraint::Constraint(std::string constraintType, std::string constraintVal){
    this->constraintType = constraintType;
    this->constraintVal = constraintVal;
}

// Constraint destructor
Constraint::~Constraint() = default;

// GET constraintType
std::string Constraint::getConstraintType(){
    return this->constraintType;
}

// GET constraintVal
std::string Constraint::getConstraintVal(){
    return this->constraintVal;
}



// DBReq constructor
DBReq::DBReq(){

}

DBReq::DBReq(int pID, std::string category,
             std::vector<std::string> * constraintNames, std::vector<std::string> * constraintVals){
    this->pID = pID;
    this->category = category;
    for (int i = 0; i < constraintNames->size(); i++) {
        addConstraint((*constraintNames)[i], (*constraintVals)[i]);
    }
}

// DBReq destructor
DBReq::~DBReq(){

}

// GET numConstraints
int DBReq::getNumConstraints(){
    return constraints.size();
}

// GET PID
int DBReq::getPID(){
    return pID;
}

// GET Category
std::string DBReq::getCategory(){
    return category;
}

// GET constraintType
std::string DBReq::getConstraintType(int constraintIndex){
    return this->constraints[constraintIndex]->getConstraintType();
}

// GET constraintVal
std::string DBReq::getConstraintVal(int constraintIndex){
    return this->constraints[constraintIndex]->getConstraintVal();
}



// SET PID
void DBReq::setPID(int newPID){
    pID = newPID;
}

// SET Category
void DBReq::setCategory(std::string newCategory) {
    category = newCategory;
}

// SET Constraint
void DBReq::addConstraint(std::string newConstraintType, std::string newConstraintVal){
    Constraint * newConstraint = new Constraint(newConstraintType, newConstraintVal); // Allocate new constraint
    constraints.push_back(newConstraint); // Add pointer to constraint* vector
}