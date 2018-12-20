//
// Created by Brendan on 12/15/2018.
//

#include <iostream>
#include <libpq-fe.h>
#include <string>
#include "testSuite.h"
#include "testdbcon.h"

void dbTests(){
    PGconn * testConn = testManualConn();
    closeManualConn(testConn);
    std::cout << std::endl;

    // Make connection through DBInterface
    auto * dbFace = new DBInterface(getUser(), getDBName(), getPW());
    DBReq * req = nullptr;
    DBRes * res = nullptr;

    // Set testPlyrs  and expectedVals vectors
    std::vector<int> testPlyrs;
    testPlyrs.insert(testPlyrs.end(), {196}); // Add additional players in here
    auto expectedVals = new std::vector<double>*[testPlyrs.size()]; // Each row holds a vector of expected vals for a given player
    for (int i = 0; i < testPlyrs.size(); i++){
        expectedVals[i] = new std::vector<double>;
    }
    expectedVals[0]->insert(expectedVals[0]->end(), {1717.0, 751.0});

    // Run through tests for each player
    for (int i = 0; i < testPlyrs.size(); i++) {
        // Test FGA DB query
        setReqAndRes(&req, &res, testPlyrs[i], "FGA", nullptr, nullptr);
        testGetDataFromDB(dbFace, req, res, (*expectedVals[i])[0], "FGA");
        clearReqAndRes(&req, &res);

        // Test FGM DB query
        setReqAndRes(&req, &res, testPlyrs[i], "FGM", nullptr, nullptr);
        testGetDataFromDB(dbFace, req, res, (*expectedVals[i])[1], "FGM");
        clearReqAndRes(&req, &res);

        // TODO: Write more tests for getDataFromDB
            // Different stats
            // Different combinations of constraints
            // Invalid DBReq objects
            // DBReq objects that should return no results

        std::cout << std::endl;
    }

    // Clear allocated expectedVals/dbFace memory
    for (int i = 0; i < testPlyrs.size(); i++){
        delete expectedVals[i];
    }
    delete expectedVals;
    delete dbFace;
}


PGconn * testManualConn(){
    // Create string with conneciton info
    std::string conninfo = "user=";
    conninfo += getUser();
    conninfo += " dbname=";
    conninfo += getDBName();
    conninfo += " password=";
    conninfo += getPW();

    // Make connection
    PGconn * testConn = PQconnectdb(conninfo.c_str());

    // Test if connection successfully made
    if (PQstatus(testConn) ==  CONNECTION_OK){
        std::cout << "Manual Connection: PASSED" << std::endl;
    } else {
        std::cout << "Manual Connection: FAILED" << std::endl;
        // Print error message?

        std::cout << "Exiting tests." << std::endl;
        fflush(stdout);
        exit(1);
    }

    return testConn;
}

void closeManualConn(PGconn * testConn){
    PQfinish(testConn);
    std::cout << "Connection closed." << std::endl;
}

void setReqAndRes(DBReq ** req, DBRes ** res, int pID, std::string category,
                  std::vector<std::string> * constraintNames, std::vector<std::string> * constraintVals){
    *req = new DBReq(pID, category, constraintNames, constraintVals); // 196 = James Harden
    *res = new DBRes();
}

void clearReqAndRes(DBReq ** req, DBRes ** res){
    delete *req;
    delete *res;
    *req = nullptr;
    *res = nullptr;
}

void testGetDataFromDB(DBInterface * dbFace, DBReq * req, DBRes * res, double expectedVal, std::string qryType){
    dbFace->getDataFromDB(req, res);
    if (res->getResVal(0, 0) == expectedVal){
        std::cout << "Get " << qryType << ": PASSED" << std::endl;
    } else {
        std::cout << "Get " << qryType << ": FAILED" << std::endl;
    }
}