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
	srand (time(NULL));
	
//	FILE* result = fopen("result.csv", "w");
//	fprintf(result, "%s", "Instance Time Cost FacilityState CustomerState");
	
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
    	#if DEBUG
    	cout << "Iteration Times: " << iteration++ << endl;
    	cout << "Selection...";
    	#endif
        ga.Selection(instance.open_cost, instance.allocate_cost);
        #if DEBUG
		cout << "done!" << endl;
		
		cout << "Iterating...";
		#endif
		ga.CrossAndMutate(instance.capicity, instance.open_cost, instance.allocate_cost);
		#if DEBUG
		cout << "done!" << endl;
		#endif
		
        cout << "Best Solution: ";
		cout << ga.bestSolution(instance.open_cost, instance.allocate_cost).getCost(instance.open_cost, instance.allocate_cost); 
		cout << endl;
		iteration++;
    } while(iteration < 100000);
}
