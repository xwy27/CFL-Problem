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

int main() {
	//srand (time(NULL));
    int ga_size, ga_crossRate, ga_mutationRate;
    string instancePath;

    cout << "Path of the instance file: ";
    cin >> instancePath;

    cout << "GA: Population Size, CrossOver Rate, Mutation Rate: " << endl;
    cin >> ga_size >> ga_crossRate >> ga_mutationRate;

	cout << "START Read File..." << endl;
    Instance instance(instancePath);
    
	cout << "START GA..." << endl;
    GeneAlgorithm ga(ga_size, ga_crossRate, ga_mutationRate, instance.facility_num,
        instance.customer_num, instance.capicity);

    char c;
    int iteration = 0;
    do {
    	cout << "Iteration Times: " << iteration++ << endl;
    	cout << "Selection...";
        ga.Selection(instance.open_cost, instance.allocate_cost);
        cout << "done!" << endl;
		
		cout << "Cross Over...";
		ga.CrossOver(instance.capicity);
		cout << "done!" << endl;
		
		cout << "Mutation...";
        ga.Mutation(instance.capicity);
        cout << "done!" << endl;
        
        cout << "Best Solution: ";
		cout << ga.bestSolution(instance.open_cost, instance.allocate_cost).getCost(instance.open_cost, instance.allocate_cost); 
		cout << endl;
    } while(cin.get() == '\n');
}
