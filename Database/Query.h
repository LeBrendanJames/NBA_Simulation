//
// Created by Brendan on 11/13/2018.
//

#ifndef SIMULATION_QUERY_H
#define SIMULATION_QUERY_H

#include "DBReq.h"
#include <string>
#include <deque>

// QUERY


class Query {
private:
    std::string queryStr;
    std::string catStr;
    std::deque<std::string> * constrtStr;

    void buildQueryFromReq(DBReq * req);

    void buildCats(DBReq * req);
    void buildConstrts(DBReq * req);

    // Each of these returns a subtable for the given player that contains only (game_id, play_id, event_type) that fit their respective category
    void buildFGA(DBReq * req);
    void buildFGM(DBReq * req);
    void build3PA(DBReq * req);
    void build3PM(DBReq * req);
    void buildDREB(DBReq * req);
    void buildOREB(DBReq * req);
    void buildSTL(DBReq * req);
    void buildBLK(DBReq * req);
    void buildTOV(DBReq * req);
    void buildDrawFoul(DBReq * req);
    void buildOffPlays(DBReq * req);
    void buildDefPlays(DBReq * req);

    // Each of these returns a subtable of only (game_id, play_id) that satisfy the constraint
    void buildPlayerOnCourt(DBReq * req, int i); // 'i' needed so that I can place the resulting string in the constraintStr array
    void buildPlayerOffCourt(DBReq * req, int i);
    void buildStartDate(DBReq * req, int i);
    void buildEndDate(DBReq * req, int i);
    void buildPlayerTeam(DBReq * req, int i);
    void buildPlayerOpponent(DBReq * req, int i);
    void buildNormalPoss(DBReq * req, int i);

public:
    Query(DBReq *);
    ~Query();

    // Takes a category and wraps in a select * to name the subtable 'cat'
    // Then, (inner) joins a constraint table and wraps everything to call the resulting table cat2
    // Continues to (inner) join constraint tables in this way for as many as there are to do
    std::string createFullStr(DBReq * req);
};

#endif //SIMULATION_QUERY_H
