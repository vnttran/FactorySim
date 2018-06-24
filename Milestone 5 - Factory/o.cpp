//file item.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "o.h"

using namespace std;

Order::Order() {

	};

Order::Order(vector<string> line) {
		 /* cout << "order (";
		for (auto& col : line)
			cout << " " << col;
		cout << ")\n";
		cout << line.size() << " columns\n"; */
		if (line.size() < 3) {
			throw string("Expected at least 3 fields, found ") + to_string(line.size());
		}

		customer = line[0];
		product = line[1];
		
		for (int i = 2; i < line.size(); i++)
			if (itemValid(line[i]))
				itemsOrdered.push_back(line[i]);
			else
				throw string("Expected an item, found '") + line[i] + "'";
	}

void Order::print() {
		cout << "/customer/product/items ordered .../ = /";
		cout << customer << "/";
		cout << product << "/";
		for (auto& i : itemsOrdered)
			cout << i << "/";
		cout << "\n";

	}

void Order::graph(fstream &gv) {
		for (auto& item : itemsOrdered) {
			gv << '"' << customer << "\n" << product << '"' << "->" << '"' << item << '"' << "[color=purple];\n";
		}
	}



OrderManager::OrderManager(vector<vector<string>> &csvdata) {
		for (auto& line : csvdata) {
			try {
				orderList.push_back(Order(line));
			}
			catch (string &e) {
				cerr << "Problem: " << e << "\n";
			}
		}
	}

	void OrderManager::print() {
		for (auto& order: orderList)
			order.print();
	}

	void OrderManager::graph(string filename) {
		fstream gv(filename, ios::out | ios::trunc);
		if (gv.is_open()) {
			gv << "digraph task {\n";
			for (auto &order: orderList)
				order.graph(gv);

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


	bool OrderManager::validate(ItemManager& im) {
		int errors = 0;
		for (auto& order : orderList) {
			size_t size = order.itemsOrderedSize();
			for (size_t i = 0; i < size; i++) {
				if (im.find(order.itemsOrderedItem(i)) == nullptr) {
					errors++;
					cerr << "Cannot find item ' " << order.itemsOrderedItem(i) << "' (missing)\n";
				}
			}
		}

		return errors ? false : true;
	}
