//
// Created by Brendan on 12/15/2018.
//

#include <iostream>
#include <libpq-fe.h>
#include <string>
#include "testSuite.h"
#include "testdbcon.h"

void dbTests(){
    //std::cout << "Hello from dbTests." << std::endl;

    PGconn * testConn = testManualConn();
    closeManualConn(testConn);
    std::cout << std::endl;

    // Check connection through DBInterface
    auto * dbFace = new DBInterface;
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