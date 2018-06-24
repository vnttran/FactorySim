#pragma once
#include <vector>
#include "o.h"

class Job : public Order {
	std::vector<bool> installed; 
public:
	Job() {}
	Job(Order& o) : Order(o) {
		installed.assign(o.itemsOrderedSize(), false);
	}
	std::vector<bool> getInstalled() { return installed; }
	void install(int i) { installed[i] = true; }
	void remove(int i) { installed[i] = false; }
	bool complete() {
		for (auto e : installed) {
			if (!e) return false;
		}
		return true;
	}
};