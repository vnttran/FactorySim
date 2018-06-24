//file item.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "i.h"

using namespace std;

int app(int argc, char* argv[]) {
	if (argc != 3) {
		throw string("usage: ") + argv[0] + "csv-file-name csv-separator-character\n";
		return 1;
	}

	string filenameItem = argv[1];
	char separator = argv[2][0];
	vector <vector<string>> csvdata;

	csvRead(filenameItem, separator, csvdata);
	ItemManager tm(csvdata);
	tm.print();
	tm.graph(filenameItem + ".gv");

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