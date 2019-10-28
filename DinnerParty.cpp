#include <iostream>
#include <memory>
#include "DinnerParty.h"

using namespace std;

void
DinnerParty::findBestTable(string filename)
{
	ifstream file;

	file.open(filename);
	// Skip to next input file if current file cannot be opened
	if (!file.is_open()){
		cout << "Unable to open " << filename << endl;
		return;
	}

	// Capture number of people at the dinner party 
	file >> num_people;

	// Initialize the preference matrix from file
	initMatrix(file);

	// Close file now that information has been stored
	file.close();

	cout << "Preference Matrix:" << endl;
	printMatrix(preference_matrix, num_people, num_people);

	// Initialize the first table by randomizing the seating arragement
	initTable();

	// Score the table
	scoreOpposites();
	scoreAdjacents();
	scorePreference();
	cout << "starting score: " << current_score << endl;

/*
	// Time algorithm for 30 seconds
	time_t start_time = time(0);
	time_t current_time;
	int seconds = 0;
	while(seconds < 1) {
		// Adjust the current seating arrangement
		adjustTable();

		// Score the table
		scoreOpposites();
		scoreAdjacents();
		scorePreference();
		cout << "current score: " << current_score << endl;

		// Save the best table arrangement that has the best score so far
		if (current_score > best_score) {
			best_table = current_table;
			best_score = current_score;
		}
		current_score = 0;

		current_time = time(0);
		seconds = difftime(current_time, start_time);
		//seconds = 30;
	}
 
 	// Print the table with the best score
    cout << "Best table:" << endl;	
	printMatrix(best_table, 2, table_len);
	cout << "best score: " << best_score << endl;
*/
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
DinnerParty::initTable()
{
	table_len = num_people/2;

	/* Organize people:
	*  - Everyone is added to the "people" vector for seat selection
	*  - Guests are added to the "guest" list
	*  - Hosts are added to the "host" list
	*/
	for (size_t i = 0; i < num_people; i++) {
		people.push_back(i+1);

		if (i+1 > table_len)
			guests.push_back(i+1);
		else 
			hosts.push_back(i+1);
	}

	// Randomize the initialize table arrangment by shuffling them
	random_shuffle(people.begin(), people.end());

	// Add shuffled list of everyone to the table
	unsigned person_idx = 0;
	current_table = vector<vector<int> >(2, vector<int>(table_len, 0));
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len; col++) {
			current_table[row][col] = people[person_idx];
			person_idx++;
		}
	}

	cout << "Randomize init table:" << endl;
	printMatrix(current_table, 2, table_len);
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
DinnerParty::scoreAdjacents() 
{
	// 1 point for every adjacent pair of people with one a host and the other a guest
	int person1;
	int person2;
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len - 1; col++) {
			person1 = current_table[row][col];
			person2 = current_table[row][col+1];

			if (isA(person1, hosts) && isA(person2, guests) ||
				isA(person1, guests) && isA(person2, hosts)) {
				current_score++;
			}
		}
	}
}


int 
DinnerParty::scoreOpposites() 
{
	// 2 points for every opposute pair of people with one a host and the other a guest
	int person1;
	int person2;
	for (size_t col = 0; col < table_len; col++) {
		person1 = current_table[0][col];
		person2 = current_table[1][col];
		if (isA(person1, hosts) && isA(person2, guests) ||
			isA(person1, guests) && isA(person2, hosts)) {
			current_score += 2;
		}
	}
}

int  
DinnerParty::scorePreference()
{
	// Add everyone's preference to the person they are sitting next to
	int person1_idx;
	int person2_idx;

	// Adds the preference of opposite pairs
	for (size_t col = 0; col < table_len; col++) {
		person1_idx = current_table[0][col] - 1;
		person2_idx = current_table[1][col] - 1;

		current_score += preference_matrix[person1_idx][person2_idx];
		current_score += preference_matrix[person2_idx][person1_idx];
	}

	// Adds the preference of adjacent pairs
	for (size_t row = 0; row < 2; row++) {
		for (size_t col = 0; col < table_len - 1; col++) {
			person1_idx = current_table[row][col] - 1;
			person2_idx = current_table[row][col+1] - 1;

			current_score += preference_matrix[person1_idx][person2_idx];
			current_score += preference_matrix[person2_idx][person1_idx];
		}
	}
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
