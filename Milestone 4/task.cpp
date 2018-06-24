 //file task.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "t.h"

using namespace std;


Task::Task() {};

Task::Task(vector<string> line) {
		//cout << "Task ("; 
		//for (auto& col : line)
		//	cout << " " << col; 
		// cout << ")\n";
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
				slots = line[1];
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

	void Task::print() {
		cout << "/task/slots/pass/fail = /";
		cout << task << "/";
		cout << slots << "/";
		cout << pass << "/";
		cout << fail << "/";
		cout << "\n";

	}

	void Task::graph(fstream &gv) {
		if (not pass.empty()) {
			gv << '"' << task << '"' << " -> " << '"' << pass << '"' << "[color=green];" << "\n";
		}
		if (not fail.empty()) {
			gv << '"' << task << '"' << " -> " << '"' << fail << '"' << "[color=red];" << "\n";
		}

		if (pass.empty() and fail.empty())
			gv << '"' << task << '"' << ";\n";
	}


	TaskManager::TaskManager(vector<vector<string>> &csvdata) {
		for (auto& line : csvdata) {
			try {
				taskList.push_back(Task(line));
			}
			catch (string &e) {
				cerr << "Problem: " << e << "\n";
			}
		}
	}

	void TaskManager::print() {
		for (auto& t : taskList)
			t.print();
	}

	void TaskManager::graph(string filename) {
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
			cmd += " -Tpng -O " + filename; 
			cout << "running command '" << cmd << "\n"; 
			system(cmd.c_str());
		}
	}

	bool TaskManager::validate() {
		int errors = 0;
		for (auto& t : taskList) {
			string pass = t.getPass();
			if (not pass.empty()) {
				if (find(pass) == nullptr) {
					errors++; 
					cerr << " Pass task ' " << pass << "' is missing (not found)\n"; 
				}
			}

			string fail = t.getFail();
			if (not fail.empty()) {
				if (find(fail) == nullptr) {
					errors++;
					cerr << " Fail task ' " << fail << "' is missing (not found)\n";
				}
			}
		}

		return errors ? false : true; 
}

	Task* TaskManager::find(string name) {
		for (size_t i = 0; i < taskList.size(); i++) {
			if (taskList[i].getTask() == name)
				return &taskList[i];
		}
		return nullptr;
	}
