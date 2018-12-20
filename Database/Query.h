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
public:
    Query(DBReq *);
    ~Query();

    // Takes a category and wraps in a select * to name the subtable 'cat'
    // Then, (inner) joins a constraint table and wraps everything to call the resulting table cat2
    // Continues to (inner) join constraint tables in this way for as many as there are to do
    std::string createFullStr(DBReq * req);

private:
    //std::string queryStr;
    std::string catStr;
    std::deque<std::string> * constrtStr;

    // Only called from constructor
    // Calls buildCats & buildConstrts, below
    void buildQueryFromReq(DBReq * req); 

    void buildCats(DBReq * req); // Build up SQL query based on what statistic is requested in DBReq, places in catStr
    void buildConstrts(DBReq * req); // Wrap subquery from buildCats in various constraints, 
                                     // places constraints in constrtStr 
                                     // (generally implemented by joining subtable returned from 
                                     // buildCats with a subtable of suitable game_id/play_id combos) 

    // Each of these returns a subtable for the given player that 
    // contains only (game_id, play_id, event_type) that fit their respective category
    void buildFGA(DBReq * req);
    void buildFGM(DBReq * req);
    void build2PA(DBReq * req);
    void build2PM(DBReq * req);
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
};

#endif //SIMULATION_QUERY_H
