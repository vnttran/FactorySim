#pragma once

//file i.h
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "t.h"


class Item {
	std::string item, installer, remover, sequence, description;
public:
	Item();
	Item(std::vector<std::string> line);
	std::string& getItem()		{ return item; }
	std::string& getInstaller() { return installer; }
	std::string& getRemover()	{ return remover; }
	void print();
	void graph(std::fstream& gv);
};

class ItemManager {
	std::vector <Item> itemList;
public:
	ItemManager(std::vector<std::vector<std::string>>& csvdata);
	void print();
	void graph(std::string filename);
	Item* find(std::string name);
	bool validate(TaskManager& tm);
	std::vector<Item> getItems() { return itemList; }
};


