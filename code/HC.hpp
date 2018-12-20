#ifndef HILLCLIMBINGALGORITHM
#define HILLCLIMBINGALGORITHM

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <ctime>
#include "Solution.hpp"

#define DEBUG false

using std::cout;
using std::endl;
using std::vector;
using std::string;

class HC {
public:
    Solution twoOpt(Solution &s);
    Solution threeOpt(Solution &s);
    Solution randomOpt(Solution &s);
    Solution reverseOpt(Solution &s);
    vector<Solution> neighborHood(Solution &s);
    Solution nextSolution(Solution &s, vector<int> &open, vector<vector<int> > &allocation,
        vector<int> &capicity, vector<int> &demand);
};

Solution HC::twoOpt(Solution &s) {
    int size = s.allocate.size();
    if (size == 0)  return Solution();
    int l = rand() % (size-1);
	int r = rand() % (size-1);
    while(r == l)   r = rand() % size;
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::swap(temp[l], temp[r]);
    Solution ans(temp, s.serving.size());
    return ans;
}

Solution HC::threeOpt(Solution &s) {
    int size = s.allocate.size();
    if (size == 0)  return Solution();
    int l = rand() % (size - 3);
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::swap(temp[l], temp[l+1]);
    std::swap(temp[l+1], temp[l+2]);
    Solution ans(temp, s.serving.size());
    return ans;
}

Solution HC::randomOpt(Solution &s) {
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::random_shuffle(temp.begin(), temp.end());
    Solution ans(temp, s.serving.size());
    return ans;
}

Solution HC::reverseOpt(Solution &s) {
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::reverse(temp.begin(), temp.end());
    Solution ans(temp, s.serving.size());
    return ans;
}

vector<Solution> HC::neighborHood(Solution &s) {
	#if DEBUG
	cout << "Generate Neighbor" << endl;
	#endif
    vector<Solution> neighbor;
    neighbor.push_back(this->reverseOpt(s));
    for (int i = 0; i < 5000; ++i) {
        neighbor.push_back(this->twoOpt(s));
        neighbor.push_back(this->threeOpt(s));
        neighbor.push_back(this->randomOpt(s));
    }
    return neighbor;
}

Solution HC::nextSolution(Solution &s, vector<int> &open, vector<vector<int> > &allocation,
    vector<int> &capicity, vector<int> &demand) {
    #if DEBUG
    cout << "next solution" << endl;
    #endif
    int cost = s.getCost(open, allocation, capicity, demand);
    int temp = 0;
    Solution ans = s;
    vector<Solution> t = this->neighborHood(s);
    int i = 0;
    for (i = 0; i < t.size(); ++i) {
      if (t[i].allocate.size() == 0) continue;
      temp = t[i].getCost(open, allocation, capicity, demand);
      if (temp < cost && temp != 0) {
        ans = t[i];
        cost = temp;
      }
    }
    return ans;
}

#endif
