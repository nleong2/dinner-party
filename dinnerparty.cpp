#include <iostream>
#include <fstream>

using namespace std;

void evaluate();
void printMatrix(int* matrix, int len);

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Please enter text file(s) to evaluate" << endl;
        return 0;
    }

    ifstream file;
    for (int i = 1; i < argc; i++) {
        cout << "file: " << argv[i] << endl;
        file.open(argv[i]);
        if (file.is_open()){

			int npeople;
			file >> npeople;

			int matrix[npeople][npeople];	
			for (int row = 0; row < npeople; row++) {
				for (int col = 0; col < npeople; col++)
					file >> matrix[row][col];
			}

			printMatrix((int*)matrix, npeople);

            file.close();
        }
        else
            cout << "Unable to open " << argv[i] << endl;
    }
    return 0;
}

void evaluate() {

    return;
}

void printMatrix(int* matrix, int len) {
	for (int row = 0; row < len; row++) {
		for (int col = 0; col < len; col++)
			cout << *((matrix + row * len) + col) << " ";
		cout << endl;
	}
}
