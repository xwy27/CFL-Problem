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

// Initialize an instance of Hill Climbing Algorithm which could be performed on an instance
// The neighborhood is calculated from four operations:
//      1. Swap two allocations between two random chosen customer
//      2. Reorder the allocations among three continuous customers(e.g: a, b, c-> b, c, a)
//      3. Randomly rearrange the allocations with the current allocations
//      4. Reverse the allocations
// Example:
//      HC hc;
//      Solution nextNode = nextSolution(solution, openCost, allocationCost, capicity, demand);
class HC {
public:
    Solution twoOpt(Solution &s);   // Swap two allocations between two random chosen customer
    Solution threeOpt(Solution &s); // Reorder the allocations among three continuous customers(e.g: a, b, c-> b, c, a)
    Solution randomOpt(Solution &s);    // Randomly rearrange the allocations with the current allocations
    Solution reverseOpt(Solution &s);   // Reverse the allocations
    vector<Solution> neighborHood(Solution &s); // Generate the neighborhood
    Solution nextSolution(Solution &s, vector<int> &open, vector<vector<int> > &allocation,
        vector<int> &capicity, vector<int> &demand);    // Find the best solution in neighborhood
};

// Performs swaping two allocations on a solution and
// returns the generated solution.
// <Param>
// 		Solution &s: the solution to be performed on
// </Param>
// <Return>
//		Solution: the generated solution
// </Return>
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

// Performs reordering three allocations on a solution and
// returns the generated solution.
// <Param>
// 		Solution &s: the solution to be performed on
// </Param>
// <Return>
//		Solution: the generated solution
// </Return>
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

// Performs randomly arranging on a solution and
// returns the generated solution.
// <Param>
// 		Solution &s: the solution to be performed on
// </Param>
// <Return>
//		Solution: the generated solution
// </Return>
Solution HC::randomOpt(Solution &s) {
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::random_shuffle(temp.begin(), temp.end());
    Solution ans(temp, s.serving.size());
    return ans;
}

// Performs reversing allocations on a solution and
// returns the generated solution.
// <Param>
// 		Solution &s: the solution to be performed on
// </Param>
// <Return>
//		Solution: the generated solution
// </Return>
Solution HC::reverseOpt(Solution &s) {
    vector<int> temp(s.allocate.begin(), s.allocate.end());
    std::reverse(temp.begin(), temp.end());
    Solution ans(temp, s.serving.size());
    return ans;
}

// Performs generating neighborhood on a solution and
// returns the neighborhood.
// <Param>
// 		Solution &s: the solution to be performed on
// </Param>
// <Return>
//		vector<Solution>: the generated neighborhood
// </Return>
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

// Performs finding the 'best' solution in the neighborhood on a solution and
// returns the generated solution.
// <Param>
// 		Solution &s: the solution to be performed on
//      vector<int> &open: the open cost of facilities
//      vector<vector<int> > &allocation: the allocation cost of customers to facilities
//      vector<int> &capicity: the capicity of facilities
//      vector<int> &demand: the demand of customers
// </Param>
// <Return>
//		Solution: the generated solution
// </Return>
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
