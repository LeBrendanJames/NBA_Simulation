//
// Created by Brendan on 12/15/2018.
//

#ifndef SIMULATION_TESTSUITE_H
#define SIMULATION_TESTSUITE_H

#include "libpq-fe.h"
#include "../Database/DBInterface.h"


void dbTests();
PGconn * testManualConn();
void closeManualConn(PGconn * testConn);

void setReqAndRes(DBReq ** req, DBRes ** res, int, std::string, std::vector<std::string>*, std::vector<std::string>*);
void clearReqAndRes(DBReq ** req, DBRes ** res);

void testGetDataFromDB(DBInterface * dbFace, DBReq * req, DBRes * res, double expectedVal, std::string qryType);

#endif //SIMULATION_TESTSUITE_H
