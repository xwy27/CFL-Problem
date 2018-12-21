#ifndef SOLUTION
#define SOLUTION

#include <string>
#include <ctime>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

// Initialize an instance of solution which contains the allocation of customers
// Example:
//		Solution s1;
//      Solution s2(facilityNum, customerNum);
//      Solution s3(v, facilityNum);
//
//      vector<int> facilityState = s2.getFacilityState();
//      int cost = s3.getCost(openCost, allocationCost, capicity, demand);
//      
//      if (s1.isValid(capicity, demand)) {
//          // do something
//      }
class Solution {
public:
    vector<int> allocate; // Allocation of all customers
    vector<int> serving; // Serving customer count for each facility
    
    Solution();
    // Default Constructor
    Solution(int &facilityNum, int &customerNum);
    // Constructor
    Solution(vector<int> &v, int facilityNum);
    // If solution match the constraint: not over capicity
    bool isValid(vector<int> &capicity, vector<int> &demand);
    // Get Facility state: 0-close, 1-open
    vector<int> getFacilityState();
    // Get total cost of this solution
    int getCost(vector<int> &open, vector<vector<int> > &allocation, vector<int> &capicity, vector<int> &demand);
    
    bool operator == (const Solution &p2) const;
};

// Construct an instance from an instance file.
Solution::Solution() {}

// Construct an instance from an instance file.
// <Param>
//		int &facilityNum: number of facilities
//      int &customerNum: number of customers
// </Param>
Solution::Solution(int &facilityNum, int &customerNum) {
    this->allocate.resize(customerNum);
    for (int i = 0; i < customerNum; ++i) {
        this->allocate[i] = 1 + rand() % facilityNum;
    }
    this->serving.resize(facilityNum);
}

// Construct an instance from an instance file.
// <Param>
//      vector<int> &v: array of allocations
//		int &facilityNum: number of facilities
// </Param>
Solution::Solution(vector<int> &v, int facilityNum) {
    int size = v.size();
    this->allocate.resize(size);
    for (int i = 0; i < size; ++i) {
        this->allocate[i] = v[i];
    }
    this->serving.resize(facilityNum);
}

// Determine if the solution is valid, which is 
// the allocated demand of a facility is no more than its capicity
// <Param>
//      vector<int> &capicity: capicity of facilities
//      vector<int> &demand: demand of customers
// </Param>
// <Return>
//		bool: true for valid, otherwise false
// </Return>
bool Solution::isValid(vector<int> &capicity, vector<int> &demand) {
    int size = capicity.size();
    for (int i = 0; i < size; ++i) {
    	this->serving[i] = 0;
	}

    size = this->allocate.size();
    for (int i = 0; i < size; ++i) {
        this->serving[this->allocate[i]-1] += demand[i];
    }

	size = capicity.size();
    for (int i = 0; i < size; ++i) {
        if (this->serving[i] > capicity[i]) {
            return false;
        }
    }

    return true;
}

// Get the facility state(open or close)
// <Return>
//		vector<int>: v[i] = 1 for open, otherwise close
// </Return>
vector<int> Solution::getFacilityState() {
    vector<int> ans;
    int size = this->serving.size();
    for (int i = 0; i < size; ++i) {
        ans.push_back(0);
    }

    for (auto &i : this->allocate) {
        ans[i-1] = 1;
    }

    return ans;
}

// Get the cost of the solution
// <Param>
//      vector<int> &open: open cost of the facilities
//       vector<vector<int> > &allocation: allocation cost
//       vector<int> &capicity: capicity of facilities
//       vector<int> &demand: demand of customers
// </Param>
// <Return>
//		int: cost of the solution
// </Return>
int Solution::getCost(vector<int> &open, vector<vector<int> > &allocation, vector<int> &capicity, vector<int> &demand) {
	if (!this->isValid(capicity, demand))	return INT_MAX;
    vector<int> temp = this->getFacilityState();
    int cost = 0;

    // Open Cost
    int size = temp.size();
    for (int i = 0; i < size; ++i) {
        if (temp[i])    cost += open[i];
    }

    // Allocation cost
    size = this->allocate.size();
    for (int i = 0; i < size; ++i) {
        cost += allocation[this->allocate[i]-1][i];
    }

    return cost;
}

bool Solution::operator == (const Solution &p2) const {
	int size = this->allocate.size();
	for (int i = 0; i < size; ++i) {
		if (this->allocate[i] != p2.allocate[i])	return false;
	}
	return true;
}

#endif
