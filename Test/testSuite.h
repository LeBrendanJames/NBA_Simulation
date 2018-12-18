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

#endif //SIMULATION_TESTSUITE_H
