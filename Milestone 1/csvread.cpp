//file csvread.cpp
//usage : csvread csv-file-name csv-separator-character 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void trimSpace(string& s) {//have to call it by reference to update
	while (!s.empty() && s[0] == ' ')
		s.erase(0, 1);

	while (!s.empty() && s[s.size()-1] == ' ')
		s.erase(s.size()-1, 1);
}

void csvPrint (vector<vector<string>> &csvdata) {
	cout << "range-based for: \n";
	for (auto& row : csvdata) {
		for (auto& column : row) {
			cout << "<" << column << "> ";
		}
	cout << "\n"; 
	}
	
	cout << "\n";

	cout << "for with indicies: \n";
	for (size_t row = 0; row < csvdata.size(); row++) {
		for (size_t column = 0; column < csvdata[row].size(); column++) {
			cout << "{" << csvdata[row][column] << "} ";
		}
		cout << "\n";
	}
	cout << "\n";

	cout << "for with iterators: \n";
	for (auto row = csvdata.begin(); row != csvdata.end(); row++) {
		for (auto column = row->begin(); column != row->end(); column++) {
			cout << "[" << *column << "] ";
		}
		cout << "\n";
	}
	cout << "\n";
}

//void csvRead(string& filename, char separator, vector<vector<string>> &csvdata);
void csvRead(string& filename, char separator) {
	fstream in(filename, ios::in);
	if (!in.is_open()) {
		throw string("Cannot open file") + filename + "";
	}
	
	vector <vector<string>> csvdata;

	string line;
	while (getline(in, line)) {
		auto cr = line.find('\r');
		if (cr != string::npos)
			line.erase(cr, 1);
		//	cout << "line -->" << line << " <--\n";


		size_t			index = 0;
		string			field;
		vector <string> fields;
		while (index < line.size()) {
			if (line[index] != separator) {
				field += line[index];
			}
			else {
				//cout << "field --> " << field << " <--\n"; 
				trimSpace(field);
				//fields.push_back(field); //deep copy - pre c++11
				// field.clear(); // turn into zombie 
				fields.push_back(move(field)); //c++11 (new way) 
			}
			index++;
		}
		trimSpace(field);
		//fields.push_back(field);
		fields.push_back(move(field));
		//cout << "last field --> " << field << " <-- \n";

		//	for (auto column : fields) {
		//		cout << "[" << column << "] ";
		//	}

		//	cout << "\n";
		csvdata.push_back(move(fields)); //add fields to csvdata and TURN fields into a zombie
		
	} //while getline
	csvPrint(csvdata);
	in.close();
}
int app(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "usage: " << argv[0] << "csv-file-name csv-separator-character\n";
		return 1;
	}

	string filename = argv[1];
	char separator = argv[2][0];
	vector <vector<string>> csvdata;
	csvRead(filename, separator);
	//csvRead(filename, separator, csvdata);
}

int main(int argc, char* argv[]) {

	try {
		app(argc, argv);
		} catch (const string& e) {
			cerr << "It threw: " + e + "\n";
		}
}

