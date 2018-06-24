//file task.cpp
//usage: task csv-task-file-name csv-separator-character
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"


class Task {
	std::string task, slots, pass, fail;
public:
	Task();
	std::string& getTask() { return task; }
	std::string& getPass() { return pass; }
	std::string& getFail() { return fail; }
	Task(std::vector<std::string> line);


	void print();

	void graph(std::fstream &gv);
};

class TaskManager {
	std::vector <Task> taskList;
public:
	TaskManager(std::vector<std::vector<std::string>> &csvdata);
	void print();
	void graph(std::string filename);
	Task* find(std::string name);
	bool validate();
};


