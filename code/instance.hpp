#ifndef INSTANCE
#define INSTANCE

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

// Initialize an instance from instance file and 
// contains the info inside the instance file
// Example:
//		Instance instance(filePath);
//		int facilityNum = instance.facilityNum;
//		int customerNum = instance.customerNum;
//		vector<int> capicity = instance.capicity;
//		vector<int> open_cost = instance.open_cost;
//		vector<int> deamnd = instance.deamnd;
//		vector<vector<int> > allocate_cost = instance.allocate_cost;
class Instance {
public:
    int facility_num;   // facility number
    int customer_num;   // customer number
    vector<int> capicity; // capicity of facilities
    vector<int> open_cost;   // open cost of facilities
    vector<int> demand; // demand of customers
    vector<vector<int> > allocate_cost; // allocate cost of facilities

    Instance(string filePath);	// Constructor
};

// Construct an instance from an instance file.
// <Param>
//		string filePath: the path of the instance file
// </Param>
Instance::Instance(string filePath) {
    FILE* fp = fopen(filePath.c_str(), "r");
    if (fp == nullptr) {
    	cout << "Failed to open file: " << filePath << endl;
	} else {
	    int temp = 0;
	    cout << "read facility num...";
	    fscanf(fp, "%d", &(temp));
	    this->facility_num = temp;
	    cout << "done!" << endl;
	    
	    cout << "read customer num...";
	    fscanf(fp, "%d", &(temp));
	    this->customer_num = temp;
	    cout << "done!" << endl;
	    
	    cout << "read facility capicity and open cost...";
	    for (int i = 0; i < this->facility_num; ++i) {
	        // read facility capicity
	        fscanf(fp, "%d", &temp);
	        this->capicity.push_back(temp);
	        // read facility open cost
	        fscanf(fp, "%d", &temp);
	        this->open_cost.push_back(temp);
	    }
		cout << "done!" << endl;
	
		cout << "read customer demand...";
	    for (int i = 0; i < this->customer_num; ++i) {
	        // read customer demand
	        fscanf(fp, "%d", &temp);
	        this->demand.push_back(temp);
	    }
	    cout << "done!" << endl;
	
		cout << "read allocation cost...";
	    this->allocate_cost.resize(this->facility_num);
	    for (int i = 0; i < this->facility_num; ++i) {
	        this->allocate_cost[i].resize(this->customer_num);
	        for (int j = 0; j < this->customer_num; ++j) {
	            // read allocating cost
	            fscanf(fp, "%d", &temp);
	            this->allocate_cost[i][j] = temp;
	        }
	    }
	    cout << "done!" << endl;
	
	    fclose(fp);
	}
}

#endif
