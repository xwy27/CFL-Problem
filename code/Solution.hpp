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
    bool isValid(vector<int> &capicity);
    // Get Facility state: 0-close, 1-open
    vector<int> getFacilityState();
    // Get total cost of this solution
    int getCost(vector<int> &open, vector<vector<int> > &allocation);
    
    bool operator == (const Solution &p2) const;
};

Solution::Solution() {}

Solution::Solution(int &facilityNum, int &customerNum) {
    this->allocate.resize(customerNum);
    for (int i = 0; i < customerNum; ++i) {
        this->allocate[i] = 1 + rand() % facilityNum;
    }
    this->serving.resize(facilityNum);
}

Solution::Solution(vector<int> &v, int facilityNum) {
    int size = v.size();
    this->allocate.resize(size);
    for (int i = 0; i < size; ++i) {
        this->allocate[i] = v[i];
    }
    this->serving.resize(facilityNum);
}

bool Solution::isValid(vector<int> &capicity) {
    int size = capicity.size();
    for (int i = 0; i < size; ++i) {
        this->serving[i] = 0;
    }
    
    size = this->allocate.size();
    for (int i = 0; i < size; ++i) {
        this->serving[this->allocate[i]-1] += 1;
    }

	size = capicity.size();
    for (int i = 0; i < size; ++i) {
        if (this->serving[i] > capicity[i]) {
            return false;
        }
    }

    return true;
}

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

int Solution::getCost(vector<int> &open, vector<vector<int> > &allocation) {
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
