#include <iostream>
#include <memory>
#include <cmath>
#include <random>
#include "DinnerParty.h"

using namespace std;

void
DinnerParty::findBestTable(string infile, ofstream& outfile, unsigned percent_random = 0)
{
	// Reset class. New file = new configuration
	resetConfiguration();

	ifstream file;
	file.open(infile);

	// Skip to next input file if current file cannot be opened
	if (!file.is_open()){
		cout << "Unable to open " << infile << endl;
		return;
	}

	// Capture number of people at the dinner party 
	file >> num_people;
	// Initialize the preference matrix from file
	initMatrix(file);

	file.close();

	// Initalize people into groups
	organizePeople();
	// Initialize starting table using a random generator
	initTable();

	// Score the table
	current_score = scoreCurrentTable();

	cout << "Running algorithm with " << percent_random << "% randomized for ~30 seconds..." << endl;

	// Time algorithm for 30 seconds
	time_t start_time = time(0);
	time_t current_time;
	int seconds = 0;
	int loop_count = 0;
	while(seconds < 30) {
		// Randomize the table for the percentage given, 
		//   otherwise adjust table using swaps and comparisons
		if ((percent_random == 0) || (loop_count % (100/percent_random) == 0))
			randomizeTable();
		else
			adjustTable();

		// Score the table
		current_score = scoreCurrentTable();

		// Save the best table arrangement that has the best score so far
		if (current_score > best_score) {
			best_table = current_table;
			best_score = current_score;
		}

		current_time = time(0);
		seconds = difftime(current_time, start_time);
		loop_count++;
	}
 
 	// Print the table with the best score
    cout << "Resulting table:" << endl;	
	printMatrix(best_table, 2, table_len);
	cout << "Resulting score: " << best_score << endl;

	return writeResults(outfile);
}

void
DinnerParty::initMatrix(ifstream& file) 
{
	// Saves the people's preferences to one another in matrix from file
	preference_matrix = vector<vector<int> >(num_people, vector<int>(num_people, 0));
	for (size_t row = 0; row < num_people; row++) {
		for (size_t col = 0; col < num_people; col++)
			file >> preference_matrix[row][col];
	}
}

void
DinnerParty::organizePeople()
{
	table_len = num_people/2;

	/* Organize people:
	*  - Everyone is added to the "people" vector for seat selection
	*  - Guests are added to the "guest" list
	*  - Hosts are added to the "host" list
	*/
	for (size_t i = 0; i < num_people; i++) {
		// 1 2 3 4 5 6 7 8 9 10.....
		people.push_back(i+1);

		if (i+1 > table_len)
			// Second half of people are guests
			guests.push_back(i+1);
		else 
			// First half of people are hosts
			hosts.push_back(i+1);
	}
}

void
DinnerParty::initTable()
{
	// Randomize inital table arrangement
	current_table = vector<vector<int> >(2, vector<int>(table_len, 0));
	randomizeTable();
}

void
DinnerParty::randomizeTable()
{
	// Shuffle list of people 
	// stackoverflow.com/questions/22105867/seeding-default-random-engine
	shuffle(people.begin(), people.end(), default_random_engine(random_device{}()));

	// Add shuffled list of everyone to the table
	unsigned person_idx = 0;
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len; col++) {
			current_table[row][col] = people[person_idx];
			person_idx++;
		}
	}
}

void
DinnerParty::adjustTable()
{
	/* Swap 2 people and recalculate the score
	*  - If the score before the swap is larger, swap back
	*/

	int curr_pref_sum = current_score;
	int new_pref_sum; 

	// Swap adjacent people
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len - 1; col++) {
			swap(row, col, row, col+1);
			new_pref_sum = scoreCurrentTable();

			if (new_pref_sum < curr_pref_sum)
				swap(row, col, row, col+1);
			else
				curr_pref_sum = new_pref_sum;
		}
	}

	// Swap opposite people
	for (size_t col = 0; col < table_len; col++) {
		swap(0, col, 1, col);
		new_pref_sum = scoreCurrentTable();

		if (new_pref_sum < curr_pref_sum)
			swap(0, col, 1, col);
		else
			curr_pref_sum = new_pref_sum;
	}
}

void
DinnerParty::swap(size_t row_p1, size_t col_p1, size_t row_p2, size_t col_p2)
{
	// Swaps people given their postition at the table for the current table
	unsigned tmp = current_table[row_p1][col_p1];
	current_table[row_p1][col_p1] = current_table[row_p2][col_p2];
	current_table[row_p2][col_p2] = tmp;
}

void 
DinnerParty::printMatrix(vector<vector<int> >& matrix, int rows, int cols) {
	// Prints the given matrix/table
	for (size_t r = 0; r < rows; r++) {
		for (size_t c = 0; c < cols; c++)
			cout << matrix[r][c] << " ";
		cout << endl;
	}
}

int
DinnerParty::scoreCurrentTable()
{
	int score = 0;
	score += scoreAdjacents();
	score += scoreOpposites();
	score += scorePreference();
	return score;
}

int
DinnerParty::scoreAdjacents() 
{
	// 1 point for every adjacent pair of people with one a host and the other a guest
	int person1;
	int person2;
	int score = 0;
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len - 1; col++) {
			person1 = current_table[row][col];
			person2 = current_table[row][col+1];

			if (isA(person1, hosts) && isA(person2, guests) ||
				isA(person1, guests) && isA(person2, hosts)) {
				score++;
			}
		}
	}
	return score;
}

int 
DinnerParty::scoreOpposites() 
{
	// 2 points for every opposute pair of people with one a host and the other a guest
	int person1;
	int person2;
	int score = 0;
	for (size_t col = 0; col < table_len; col++) {
		person1 = current_table[0][col];
		person2 = current_table[1][col];
		if (isA(person1, hosts) && isA(person2, guests) ||
			isA(person1, guests) && isA(person2, hosts)) {
			score += 2;
		}
	}
	return score;
}

int  
DinnerParty::scorePreference()
{
	// Add everyone's preference to the person they are sitting next to
	int person1_idx;
	int person2_idx;
	int score = 0;

	// Adds the preference of opposite pairs
	for (size_t col = 0; col < table_len; col++) {
		person1_idx = current_table[0][col] - 1;
		person2_idx = current_table[1][col] - 1;

		score += preference_matrix[person1_idx][person2_idx];
		score += preference_matrix[person2_idx][person1_idx];
	}

	// Adds the preference of adjacent pairs
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len - 1; col++) {
			person1_idx = current_table[row][col] - 1;
			person2_idx = current_table[row][col+1] - 1;

			score += preference_matrix[person1_idx][person2_idx];
			score += preference_matrix[person2_idx][person1_idx];
		}
	}

	return score;
}

bool
DinnerParty::isA(int person, vector<int> person_type)
{
	// Returns true or false if the given person is in the given vector of people (host or guest)
	vector<int>::iterator it;
	it = find(person_type.begin(), person_type.end(), person);
	
	if (it != person_type.end())
		return true;
	return false;
}

void
DinnerParty::resetConfiguration()
{
	people.clear();
	guests.clear();
	hosts.clear();
	preference_matrix.clear();
	current_table.clear();
	best_table.clear();
	current_score = 0;
	best_score = 0;
}

void
DinnerParty::writeResults(ofstream& outfile)
{
	outfile << best_score << endl;

	unsigned seat_num = 1;
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len; col++)
			outfile << best_table[row][col] << " " << seat_num << " ";
		outfile << endl;
		seat_num++;
	}
}
