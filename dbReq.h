


#ifndef SIMULATION_DBREQ_H
#define SIMULATION_DBREQ_H

// Need all these:
// https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)

Class DBReq{
	// Array of what to select (**Vector?**)
	string * cols;
	
	// Array of tables to select from (maybe matching array above so that name can be prepended to each column to select) (**Vector?**)
	string * tables;
	
	// constraints
		// start_date
		struct tm * start_date;
		// end_date
		struct tm * end_date;
		// team
		string * teamCode;
		
	// array of ints for joinType (default to inner join, but can change int for left join/right join)
		// Tells how each successive table should be joined
		
	// Array of join criteria
	
	// Group by
	
	// Having?
	
	
};


#endif //SIMULATION_DBREQ_H