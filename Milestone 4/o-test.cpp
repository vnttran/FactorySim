//file item.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "o.h"

using namespace std;


int app(int argc, char* argv[]) {
	if (argc != 3) {
		throw string("usage: ") + argv[0] + "csv-file-name csv-separator-character\n";
		return 1;
	}

	string filenameOrder = argv[1];
	char separator = argv[2][0];
	vector <vector<string>> csvdata;

	csvRead(filenameOrder, separator, csvdata);
	OrderManager tm(csvdata);
	tm.print();
	tm.graph(filenameOrder + ".gv");

	//csvRead(filename, separator, csvdata);
}

int main(int argc, char* argv[]) {

	try {
		app(argc, argv);
	}
	catch (const string& e) {
		cerr << "It threw: " + e + "\n";
	}

	system("pause");
}