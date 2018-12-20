#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "instance.hpp"
#include "GA.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using namespace std;

void performGA(int index, FILE* result, int ga_size, int ga_crossRate, int ga_mutationRate);

int main() {
	srand (time(NULL));
	
	int ga_size, ga_crossRate, ga_mutationRate;

    cout << "GA: Population Size, CrossOver Rate, Mutation Rate: " << endl;
    cin >> ga_size >> ga_crossRate >> ga_mutationRate;
	
	FILE* result = fopen("result.csv", "w");
	if (result == nullptr) {
		cout << "Fail to open file: result.csv" << endl;
		return 0;
	}
	fprintf(result, "%s\n", "Instance,Time,Cost,FacilityState,CustomerState");
	
	string filePath;
	for (int i = 1; i <= 71; ++i) {
		performGA(i, result, ga_size, ga_crossRate, ga_mutationRate);
	}
}

void performGA(int index, FILE* result, int ga_size, int ga_crossRate, int ga_mutationRate) {
	cout << "START Read File..." << endl;
    string instancePath = "../Instance/p" + std::to_string(index);
	Instance instance(instancePath);
    
    time_t start = time(nullptr);
	cout << "START GA..." << endl;
    GeneAlgorithm ga(ga_size, ga_crossRate, ga_mutationRate, instance.facility_num,
        instance.customer_num, instance.capicity, instance.demand);
    
    Solution ans, temp;
    int iteration = 0, min_cost = INT_MAX, cost = 0;
    do {
    	#if DEBUG
    	cout << "Iteration Times: " << iteration++ << endl;
    	cout << "Selection...";
    	ga.Selection(instance.open_cost, instance.allocate_cost, instance.demand);
        cout << "done!" << endl;
		cout << "Iterating...";
		ga.CrossAndMutate(instance.capicity, instance.open_cost, instance.allocate_cost);
		cout << "done!" << endl;

		#else
    	ga.Selection(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		ga.CrossAndMutate(instance.capicity, instance.open_cost, instance.allocate_cost, instance.demand);
		#endif
		
        cout << "Best Solution-" << index << ": ";
        temp = ga.bestSolution(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
        cost = temp.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		cout << cost << endl;
		if (min_cost > cost) {
			ans = temp;
			min_cost = cost;
			iteration = 0;
		} else {
			iteration++;
		}
    } while(iteration < 5000);
    
    string fs = "";
    for (auto &i : ans.getFacilityState()) {
    	fs += to_string(i) + " ";
	}
	string cs = "";
	for (auto &i : ans.allocate) {
		cs += to_string(i) + " ";
	}
    fprintf(result, "%d,%f,%d,%s,%s\n", index, (difftime(time(nullptr), start)), min_cost, fs.c_str(), cs.c_str());
}
