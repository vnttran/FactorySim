#pragma once 
//file item.cpp
//usage: task csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "i.h"


class Order {
	std::string customer, product;
	std::vector <std::string> itemsOrdered;
public:
	Order();
	Order(std::vector<std::string> line);
	std::string& getCustomer() { return customer; }
	std::string& getProduct() { return product; }
	void print();
	void graph(std::fstream &gv);
	size_t itemsOrderedSize() { return  itemsOrdered.size(); }
	std::string itemsOrderedItem(size_t i) { return itemsOrdered[i]; }
};

class OrderManager {
	std::vector <Order> orderList;
public:
	OrderManager(std::vector<std::vector<std::string>> &csvdata);
	void print();
	void graph(std::string filename);
	bool validate(ItemManager& im);
	size_t count() { return orderList.size(); }
	Order& getOrder(size_t i) { return orderList[i]; }
	
};
