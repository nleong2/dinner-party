#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <memory>

#include "DinnerParty.h"

using namespace std;

int main(int argc, char** argv) {
	// Check to ensure files have been provided as input
    if (argc < 2) {
        cout << "Please enter text file(s) to evaluate" << endl;
        return -1;
   	}

	DinnerParty dinner;

	// Loop through each file given
    ifstream file;
    for (size_t i = 1; i < argc; i++) {
        cout << "file: " << argv[i] << endl;

		dinner.findBestTable(argv[i]);

		cout << endl;
    }
    return 0;
}

