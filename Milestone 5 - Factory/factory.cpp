//file iot.cpp
//usage: iot csv-item-file name csv-order-file-name csv-task-file-name csv-separator-character

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "util.h"
#include "i.h"
#include "o.h"
#include "t.h"
#include "j.h"
#include "m.h"

using namespace std;

class Factory {
	vector<Machine> machineList;
public:
	Factory(ItemManager& im, OrderManager& om, TaskManager& tm) {
		//step 1 - for each task create a machine on the machineList
		for (size_t i = 0; i < tm.count(); i++) {
			Machine newMachine(tm.getTask(i), im);
			machineList.push_back(newMachine);
		}

		auto find = [&](std::string name) -> Machine* {
			for (size_t i = 0; i < machineList.size(); i++) {
				if (machineList[i].getTask() == name) {
					return &machineList[i];
				}
			}
			return nullptr;
		};

		//step 2 - Identify sink node
		for (auto& machine : machineList) {
			string pass = machine.getPass();
			if (not pass.empty()) {
				find(pass)->incIncoming();
			}
		}

		for (auto& machine : machineList) {
			string fail = machine.getFail();
			if (not fail.empty()) {
				find(fail)->incIncoming();
			}
		}

		for (auto& machine : machineList) {
			cout << machine.getTask();
			if (machine.isSource()) {
				cout << "<-- is source node";
			}
			if (machine.isSink()) {
				cout << "<-- is sink node";
			}
			cout << '\n';
		}

		int sourceNode = -1;
		for (int i = 0; i < machineList.size(); i++) {
			if (machineList[i].isSource()) {
				if (sourceNode == -1) {
					sourceNode = i;
				}
				else {
					throw string("Multiple source nodes. fix task data and resubmit!");
				}
			}
		}
		if (sourceNode == -1) {
			throw string("Missing source node. fix task data and resubmit!");
		}
		cout << "sourceNode = " << sourceNode << " (" << machineList[sourceNode].getTask() << ")\n";

		//step 3 - Add job onto sourceNode jobQueue
		for (size_t i = 0; i < om.count(); i++) {
			machineList[sourceNode].addJob(move(Job(om.getOrder(i))));
		}

		auto activeMachines = [&] {
			int ret = 0;
			for (auto& machine : machineList) {
				if (machine.jobCount()) {
					ret += machine.jobCount();
					cout << machine.getTask() << " has " << machine.jobCount() << " jobs.\n";
				}
			}
			return ret;
		};

		int time = 0;
		int loop = 0;
		while (true) {
			if (activeMachines() == 0) {
				cout << "no active machines\n";
				break;
			}

			for (auto& machine : machineList) {
				while (machine.jobCount() != 0) {
					cout << machine.jobCount() << " jobs for machine " << machine.getTask() << "\n";

					//cout << "Active Machines = " << activeMachines() << "\n";
					//cout << "jobCount = " << machine.jobCount() << " for " << machine.getTask() << "\n";
					Job job(move(machine.getJob()));
					machine.removeJob();

					bool didSomething = false;
					//sink - no job to do
					if (machine.isSink()) {
						if (job.complete()) {
							cout << "Job " << job.getCustomer() << "/" << job.getProduct() << " finished at machine " << machine.getTask() << "\n";
							break;
						}

						//installer
					}
					else if (machine.isInstaller()) {
						cout << "Job " << job.getCustomer() << "/" << job.getProduct() << " finished at machine " << machine.getTask() << "\n";
						cout << machine.getTask() << " is an installer()\n";
						for (int i = 0; i < job.getInstalled().size(); i++) {
							if (job.getInstalled()[i] == false) {
								if (im.find(job.itemsOrderedItem(i))->getInstaller() == machine.getTask()) {
									job.install(i);
									time += machine.getSlots();
									didSomething = true;
								}
							}
						}

						//remover
					}
					else if (machine.isRemover()) {
						cout << "Job " << job.getCustomer() << "/" << job.getProduct() << " finished at machine " << machine.getTask() << "\n";
						cout << machine.getTask() << " is a remover()\n";
						for (int i = job.getInstalled().size()-1; i > 0; i--) {
							if (job.getInstalled()[i] == true) {
								if (im.find(job.itemsOrderedItem(i))->getRemover() == machine.getTask()) {
									cout << "removing installed job" << "\n";
									job.remove(i);
									time += machine.getSlots();
									didSomething = true;
								}
							}
						}
					}

					auto route = [&](string& name) {
						find(name)->addJob(move(job));
					};

					//inspector
					//std::cout << "inspector\n";
					if (machine.getFail().empty() || didSomething == false) {
						cout << "getPass() no fail\n";
						route(machine.getPass());
						loop += 1;
						cout << loop << " loops so far" << "\n";
					}
					else {
						if (std::rand() & 1) {
							cout << "getPass() with fail\n";
							route(machine.getPass());
							loop += 1;
							cout << loop << " loops so far" << "\n";
						}
						else {
							cout << "getFail()\n";
							route(machine.getFail());
							loop += 1;
							cout << loop << " loops so far" << "\n";
						}
					}
				}
			} //end (bottom) of machine loop

		}

		cout << "all done! and only after: " << time << " time units" << std::endl;
	}

};

int app(int argc, char* argv[]) {
	if (argc != 5) {
		throw string("usage: ") + argv[0] + " iot csv-item-file name csv-order-file-name csv-task-file-name csv-separator-character\n";
		return 1;
	}

	vector < vector<string> > csvData;
	string filenameItem = argv[1];
	string filenameOrder = argv[2];
	string filenameTask = argv[3];
	char separator = argv[4][0];

	vector <vector<string>> csvdataItem;
	vector <vector<string>> csvdataOrder;
	vector <vector<string>> csvdataTask;

	csvRead(filenameItem, separator, csvdataItem);
	csvRead(filenameOrder, separator, csvdataOrder);
	csvRead(filenameTask, separator, csvdataTask);


	ItemManager im(csvdataItem);
	cout << "Item Manager running\n";
	im.print();
	im.graph(filenameItem + ".gv");

	OrderManager om(csvdataOrder);
	cout << "Order Manager running\n";
	om.print();
	om.graph(filenameOrder + ".gv");

	TaskManager tm(csvdataTask);
	cout << "Task Manager running\n";
	tm.print();
	tm.graph(filenameTask + ".gv");

	if (im.validate(tm) and tm.validate() and om.validate(im)) {
		cout << " Good to go! -- no errors \n";
	}
	else
		throw string ("ERRORS!! ERRORS!! ERRORS!!");
		//csvRead(filename, separator, csvdata);

	Factory factory(im, om, tm);
}


int main(int argc, char* argv[]) {

	try {
		app(argc, argv);
	}
	catch (const string& e) {
		cerr << "It threw: " + e + "\n";
	}

}
