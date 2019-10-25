#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

void evaluate();
void printMatrix(int* matrix, size_t row, size_t col);

int main(int argc, char** argv) {
	// Check to ensure files have been provided as input
    if (argc < 2) {
        cout << "Please enter text file(s) to evaluate" << endl;
        return -1;
    }

	// Loop through each file given
    ifstream file;
    for (size_t i = 1; i < argc; i++) {
        cout << "file: " << argv[i] << endl;

        file.open(argv[i]);
		// Skip to next input file if current file cannot be opened
        if (!file.is_open()){
            cout << "Unable to open " << argv[i] << endl;
			break;
		}

		// Capture number of people at the dinner party 
		size_t npeople;
		file >> npeople;

		// Save perference of people to others into a matrix
		int matrix[npeople][npeople];	
		for (size_t row = 0; row < npeople; row++) {
			for (size_t col = 0; col < npeople; col++)
				file >> matrix[row][col];
		}

		//printMatrix((int*)matrix, npeople, npeople);

		// Save number of hosts
		size_t hosts = npeople/2;

		// create vector of all of the people
		vector<int> people;
		for (size_t j = 0; j < npeople; j++)
			people.push_back(j+1);

		// shuffle people
		random_shuffle(people.begin(), people.end());

		// print the shuffled people
		for (size_t j = 0; j < npeople; j++)
			cout << people[j] << " ";
		cout << endl;

		// put people into a matrix of the table
		unsigned count = 0;
		unsigned n = 0;
		int table[2][hosts];
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < hosts; col++) {
				table[row][col] = people[n];
				count++;
				n++;
			}
		}

		// print table
		printMatrix((int*)table, 2, hosts);

		file.close();
		cout << endl;
    }
    return 0;
}

void evaluate() {

    return;
}

void printMatrix(int* matrix, size_t ROW, size_t COL) {
	for (size_t row = 0; row < ROW; row++) {
		for (size_t col = 0; col < COL; col++)
			cout << *((matrix + row * COL) + col) << " ";
		cout << endl;
	}
}
