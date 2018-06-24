//file item.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "i.h"

using namespace std;


	Item::Item() {

	};

	Item::Item(vector<string> line) {
		//cout << "Item (";
		//for (auto& col : line)
		//	cout << " " << col;
		//cout << ")\n";
		//cout << line.size() << " columns\n";

		switch (line.size()) {
		case 5: //description
			description = line[4];
		case 4: //sequence number
			if (sequenceValid(line[3]))
				sequence = line[3];
			else
				throw string("Expected a sequence number, found '") + line[3] + "'";
		case 3: //remover
			if (taskValid(line[2]))
				remover = line[2];
			else
				throw string("Expected remover task name, found '") + line[2] + "' ";
		case 2: //installer
			if (taskValid(line[1]))
				installer = line[1];
			else
				throw string("Expected slots, found '") + line[1] + "' ";
		case 1: //item name
			if (itemValid(line[0]))
				item = line[0];
			else
				throw string("Expected a task, found '") + line[0] + "' ";
			break;
		default:
			throw string("Expected 4  or 5 fields, found " + to_string(line.size()));
			break;
		}
	}

	void Item::print() {
		cout << "item/installer/remover/sequence/description = /";
		cout << item << "/";
		cout << installer << "/";
		cout << remover << "/";
		cout << sequence << "/";
		cout << description << "/";
		cout << "\n";

	}

	void Item::graph(fstream &gv) { 
		gv << '"' << item << '"' << "->" << '"' << installer << '"' << "[color=green];\n";
		gv << '"' << item << '"' << "->" << '"' << remover << '"' << "[color=red];\n";
	}



	ItemManager::ItemManager(vector<vector<string>> &csvdata) {
		for (auto& line : csvdata) {
			try {
				itemList.push_back(Item(line));
			}
			catch (string &e) {
				cerr << "Problem: " << e << "\n";
			}
		}
	}

	void  ItemManager::print() {
		for (auto& item: itemList)
			item.print();
	}

	void  ItemManager::graph(string filename) {
		fstream gv(filename, ios::out | ios::trunc);
		if (gv.is_open()) {
			gv << "digraph task {\n";
			for (auto &item : itemList)
				item.graph(gv);

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

	bool ItemManager::validate(TaskManager& tm) {
		int errors = 0;
		for (auto& item : itemList) {
			string installer = item.getInstaller();
			if (tm.find(installer) == nullptr) {
				errors++;
				cerr << "Installer task '" << installer << "' is missing (not found)\n";
			}

			string remover = item.getRemover();
			if (tm.find(remover) == nullptr) {
				errors++;
				cerr << "Remover task '" << remover << "' is missing (not found)\n";
			}
		}

		return errors ? false : true;
	}


	Item* ItemManager::find(std::string name) {
		for (size_t i = 0; i < itemList.size(); i++) {
			if (itemList[i].getItem() == name) {
				return &itemList[i];
			}
		}
		return nullptr;
	}