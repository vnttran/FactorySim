#pragma once
#include "j.h"
#include "t.h"
#include "i.h"
#include <queue>

class Machine : public Task {
	int incoming = 0;
	std::queue<Job> jobQueue;
	bool installer = false;
	bool remover = false;
public:
	Machine() {}
	Machine(Task& t, ItemManager im) : Task(t) {
		for (auto& i : im.getItems()) {
			if (i.getInstaller() == t.getTask()) {
				installer = true;
			}
			if (i.getRemover() == t.getTask()) {
				remover = true;
			}

			if (installer && remover) {
				throw std::string("both an installer and a remover: ") + t.getTask();
			}
		}
	}

	//source-sink functions
	void incIncoming() { incoming++; }
	bool isSource() { return incoming == 0; }
	bool isSink() { return getPass().empty() and getFail().empty(); }

	//JobQ functions
	void addJob(Job&& j) { jobQueue.push(std::move(j)); }
	size_t jobCount() { return jobQueue.size(); }
	Job&& getJob() { return std::move(jobQueue.front()); }
	void removeJob() { jobQueue.pop(); }

	//installer/remover functions
	bool isInstaller() { return installer; }
	bool isRemover() { return remover; }
};