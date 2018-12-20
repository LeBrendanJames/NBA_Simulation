//
// Created by Brendan on 11/13/2018.
//

#ifndef SIMULATION_DBRES_H
#define SIMULATION_DBRES_H

#include <vector>

// RESPONSE


class PlayerRes {
private:
    // PlayerID
    int pID;

    // Vector of response values
    std::vector<double> resVals;

public:
    // Constructor/Destructor
    PlayerRes();
    ~PlayerRes();

    // Setters
    void addPID(int);
    void addResVal(double);

    // Getters
    int getResValsSize();
    int getPID();
    double getResVal(int resValIndex);
};

class DBRes {
private:
    // Vector of pointers to playerResponse objects?
    // Player response objects contain playerID and vector of response values? **Class PlayerRes**
    // How do I handle the type of the response? Will it always be int or float? Probably float, since that's still fine for ints
    std::vector<PlayerRes*> playerRes;

    // Can just create with the first vector, then create response objects as I go?
public:
    // Constructor/Destructor
    DBRes();
    ~DBRes();

    // Getter
    void getPlayerRes(PlayerRes *, int);
    double getResVal(int playerResIndex, int resValIndex);

    // Setter
    void addPlayerRes();
    void addPlayerResVal(double);
};

#endif //SIMULATION_DBRES_H
