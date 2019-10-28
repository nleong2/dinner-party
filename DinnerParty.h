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

	void findBestTable(string, ofstream&, unsigned);

private:
	void initMatrix(ifstream&);
	void initTable();
	void organizePeople();
	void randomizeTable();
	void adjustTable();
	int  scoreCurrentTable();
	int  scoreAdjacents();
	int  scoreOpposites();
	int  scorePreference();
	int  getScore();
	void printMatrix(vector<vector<int> >&, int, int);
	bool isA(int, vector<int>);
	void swap(size_t, size_t, size_t, size_t);
	void resetConfiguration();
	void writeResults(ofstream&);

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

