//file iot.cpp
//usage: iot csv-item-file name csv-order-file-name csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "i.h"
#include "o.h"
#include "t.h"

using namespace std;

int app(int argc, char* argv[]) {
	if (argc != 5) {
		throw string("usage: ") + argv[0] + " iot csv-item-file name csv-order-file-name csv-task-file-name csv-separator-character\n";
		return 1;
	}

	string filenameItem = argv[1];
	string filenameOrder = argv[2];
	string filenameTask = argv[3];
	char separator = argv[4][0];
	
	vector <vector<string>> csvdataItem;
	vector <vector<string>> csvdataOrder;
	vector <vector<string>> csvdataTask;

	csvRead(filenameItem, separator, csvdataItem);
	csvRead(filenameOrder, separator, csvdataOrder);
	csvRead(filenameTask, separator, csvdataTask);
	

	ItemManager im(csvdataItem);
	cout << "\nItem Manager running\n";
	im.print();
	im.graph(filenameItem + ".gv");

	OrderManager om(csvdataOrder);
	cout << "\nOrder Manager running\n";
	om.print();
	om.graph(filenameOrder + ".gv");

	TaskManager tm(csvdataTask);
	cout << "\nTask Manager running\n";
	tm.print();
	tm.graph(filenameTask + ".gv");

	if (im.validate(tm) and tm.validate() and om.validate(im)) {
		cout << " Good to go! -- no errors \n";
	}
	else
		cout << "ERRORS!! ERRORS!! ERRORS!!\n";
;	//csvRead(filename, separator, csvdata);
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