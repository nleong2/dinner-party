#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <memory>

using namespace std; 

class DinnerParty {
public:
	DinnerParty()  = default;
	~DinnerParty() = default;

	void  			 findBestTable(string);
	unique_ptr<int*> getBestTable();
	void             printMatrix(vector<vector<int> >&, int, int);
	int              getScore();

private:
	void initMatrix(ifstream&);
	void initTable();
	int  scoreAdjacents();
	int  scoreOpposites();
	int  scorePreference();
	bool isA(int, vector<int>);

	int 		         num_people;
	int			         table_len;
	int  			     best_score = 0;
	int  		         current_score = 0;
	vector<int>          guests;
	vector<int>          hosts;
	vector<int>          people;
	vector<vector<int> > preference_matrix;
	vector<vector<int> > best_table;
	vector<vector<int> > current_table;
};

