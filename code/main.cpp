#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "instance.hpp"
#include "GA.hpp"
#include "HC.hpp"

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using namespace std;

void performGA(Instance &instance, int index, FILE* result, int ga_size, int ga_crossRate, int ga_mutationRate);
void performHC(Instance &instance, int index, FILE* result);

int main() {
	srand (time(NULL));
	
	int ga_size, ga_crossRate, ga_mutationRate;

    cout << "GA: Population Size, CrossOver Rate, Mutation Rate: " << endl;
    cin >> ga_size >> ga_crossRate >> ga_mutationRate;
	
	FILE* resultGA = fopen("resultGA.csv", "w");
	FILE* resultHC = fopen("resultHC.csv", "w");
	if (resultGA == nullptr) {
		cout << "Fail to open file: resultGA.csv" << endl;
		return 0;
	}
	if (resultHC == nullptr) {
		cout << "Fail to open file: resultHC.csv" << endl;
		return 0;
	}
	fprintf(resultGA, "%s\n", "Instance,Time,Cost,FacilityState,CustomerState");
	fprintf(resultHC, "%s\n", "Instance,Time,Cost,FacilityState,CustomerState");	
	
	string filePath;
	for (int i = 1; i <= 71; ++i) {
		cout << "START Read File..." << endl;
		string instancePath = "../Instance/p" + std::to_string(i);
		Instance instance(instancePath);
		performHC(instance, i, resultHC);
		performGA(instance, i, resultGA, ga_size, ga_crossRate, ga_mutationRate);
	}

	fclose(resultGA);
	fclose(resultHC);
}

void performGA(Instance &instance, int index, FILE* result, int ga_size, int ga_crossRate, int ga_mutationRate) {    
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
		ga.Selection(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
        cout << "done!" << endl;
		cout << "Iterating...";
		ga.CrossAndMutate(instance.capicity, instance.open_cost, instance.allocate_cost, instance.demand);
		cout << "done!" << endl;

		#else
    	ga.Selection(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		ga.CrossAndMutate(instance.capicity, instance.open_cost, instance.allocate_cost, instance.demand);
		#endif
		
        temp = ga.bestSolution(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
        cost = temp.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		if (min_cost > cost) {
			ans = temp;
			min_cost = cost;
			iteration = 0;
			cout << "GA-Best Solution-" << index << ": " << min_cost << endl;
		} else {
			iteration++;
		}
    } while(iteration < 10000);
    
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

void performHC(Instance &instance, int index, FILE* result) {
	HC hc;
	time_t start = time(nullptr);
	cout << "Start HC..." << endl;
	Solution curNode(instance.facility_num, instance.customer_num);
	while (!curNode.isValid(instance.capicity, instance.demand)) {
            curNode = Solution(instance.facility_num, instance.customer_num);
        }
	Solution nextNode = hc.nextSolution(curNode, instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
	int curCost = curNode.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
	int nextCost = nextNode.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
	while (curCost > nextCost) {
		cout << "HC-Best Solution-" << index << ": " << nextCost << endl; 
		curNode = nextNode;
		nextNode = hc.nextSolution(curNode, instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		curCost = curNode.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
		nextCost = nextNode.getCost(instance.open_cost, instance.allocate_cost, instance.capicity, instance.demand);
	}
	
	string fs = "";
    for (auto &i : curNode.getFacilityState()) {
    	fs += to_string(i) + " ";
	}
	string cs = "";
	for (auto &i : curNode.allocate) {
		cs += to_string(i) + " ";
	}
    fprintf(result, "%d,%f,%d,%s,%s\n", index, (difftime(time(nullptr), start)), curCost, fs.c_str(), cs.c_str());
}
