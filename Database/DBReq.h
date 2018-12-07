//
// Created by Brendan on 11/13/2018.
//

#ifndef SIMULATION_DBREQ_H
#define SIMULATION_DBREQ_H

#include <string>
#include <vector>

// REQUEST


class Constraint{
private:
    std::string constraintType;
    std::string constraintVal;
    // int constraintNum;
    // struct tm * constraintDate;

public:
    // Constructor/Destructor
    Constraint(std::string constraintType, std::string constraintVal);
    ~Constraint();

    // Getters
    std::string getConstraintType();
    std::string getConstraintVal();
};

class DBReq {
private:
    // playerID that request is for
    int pID;

    // Vector of categories to pull, so that a request for multiple categories can be sent at once
    // (will be written in bball terms, so will have to translate to table columns in DAL -
    // should be easy with a hard-coded translation map)
    std::string category;

    // Vector of constraints (Same deal - will have to translate to actual table & column names)
    // This is a list of constraints, both name and value (number or date in struct tm)
    std::vector<Constraint*> constraints;

public:
    // Constructor/Destructor
    DBReq();
    DBReq(int pID, std::string category,
          std::vector<std::string> * constraintNames, std::vector<std::string> * constraintVals);
    ~DBReq();

    // Get size of constraints vector
    int getNumConstraints();

    // Getters
    int getPID();
    std::string getCategory();
    std::string getConstraintType(int constraintIndex);
    std::string getConstraintVal(int constraintIndex);

    // Setters
    void setPID(int);
    void setCategory(std::string);
    void addConstraint(std::string constraintType, std::string constraintVal);
};

#endif //SIMULATION_DBREQ_H
