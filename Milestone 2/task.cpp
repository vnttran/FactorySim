//file task.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"

using namespace std;

class Task {
	string task, slot, pass, fail;
public:
	Task() {

	};

	Task(vector<string> line) {
		//cout << "Task ("; 
		//for (auto& col : line)
		//	cout << " " << col; 
		//cout << ")\n";
		//cout << line.size() << " columns\n";
		
		switch (line.size()) {
		case 4: //pass
			if (taskValid(line[3]))
				fail = line[3];
			else
				throw string("Expected a fail task, found '") + line[3] + "' ";
		case 3: //pass
			if (taskValid(line[2]))
				pass = line[2];
			else
				throw string("Expected a pass task, found '") + line[2] + "' ";
		case 2: //slots
			if (slotsValid(line[1]))
				slot = line[1];
			else
				throw string("Expected slots, found '") + line[1] + "' ";
		case 1:
			if (taskValid(line[0]))
				task = line[0];
			else
				throw string("Expected a task, found '") + line[0] + "' ";
			break;
		default: 
			throw string("Expected 1, 2, 3, or 4 fields, found " + to_string(line.size()));
			break;
		}
	}

	void print() {
		cout << "/task/slots/pass/fail = /";
		cout << task << "/";
		cout << slot << "/";
		cout << pass << "/";
		cout << fail << "/";
		cout << "\n";

	}

	void graph(fstream &gv) {
		if (not pass.empty()) {
			gv << '"' << task << '"' << " -> " << '"' << pass << '"' << "[color=green]" << ";\n";
		}
		if (not fail.empty()) {
			gv << '"' << task << '"' << " -> " << '"' << fail << '"' << "[color=red]" << ";\n";
		}

		if (pass.empty() and fail.empty())
			gv << '"' << task << '"' << ";\n";
	}
};

class TaskManager {
	vector <Task> taskList;
public: 
	TaskManager(vector<vector<string>> &csvdata) {
		for (auto& line : csvdata) {
			try {
				taskList.push_back(Task(line));
			}
			catch (string &e) {
				cerr << "Problem: " << e << "\n";
			}
		}
	}

	void print() {
		for (auto& t : taskList)
			t.print();
	}

	void graph(string filename) {
		fstream gv(filename, ios::out | ios::trunc);
		if (gv.is_open()) {
			gv << "digraph task {\n";
			for (auto &t : taskList)
				t.graph(gv);

			gv << "}\n";
			gv.close();

#ifdef __unix // defined by gnu g++ compiler (Linux, Androis, OsX, IoS, cygwin, wingw)
			string cmd = "dot";
#else
			string cmd = "C:/\"Program Files (x86)\"/Graphviz2.38/bin/dot.exe ";
#endif
			cmd += "-Tpng -O " + filename;
			cout << "running command '" << cmd << "\n";
			system(cmd.c_str());
		}
	}
};


int app(int argc, char* argv[]) {
	if (argc != 3) {
		throw string("usage: ") + argv[0] + "csv-file-name csv-separator-character\n";
		return 1;
	}

	string filename = argv[1];
	char separator = argv[2][0];
	vector <vector<string>> csvdata;

	csvRead(filename, separator, csvdata);
	TaskManager tm(csvdata);
	tm.print();
	tm.graph(filename + ".gv");
	
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