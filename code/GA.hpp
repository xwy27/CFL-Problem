#ifndef GENEALGORITHM
#define GENEALGORITHM

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <ctime>
#include "Solution.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

// Initialize an instance of Genetic Algorithm which could be performed on an instance
// Selection is that randomly choosing half of the current popultion without repeat.
// However, to make it faster converges, put the 'best' one into the selection first.
// Cross over uses the two point segment exchanging methods and based on its rate.
// Mutation(randomly choose a new facility) can be operated on each gene point based on the rate.
// Example:
//      GeneAlgorithm ga(ga_size, ga_crossRate, ga_mutationRate, facilityNum, customerNum, capicity, demand);
//      Solution ans, temp;
//      int iteration = 0, min_cost = INT_MAX, cost = 0;
//      do {
//      	ga.Selection(openCost, allocateCost, capicity, demand);
// 	        ga.CrossAndMutate(capicity, openCost, allocateCost, demand);
//          temp = ga.bestSolution(openCost, allocateCost, capicity, demand);
//          cost = temp.getCost(openCost, allocateCost, capicity, demand);
//       	if (min_cost > cost) {
//       		ans = temp;
//       		min_cost = cost;
//       		iteration = 0;
//       	} else {
//       		iteration++;
//       	}
//       } while(iteration < 10000);
//      // Some other code for 'best' solution: ans
class GeneAlgorithm {
public:
    int crossRate;  // CrossOver rate
    int mutationRate;   // Mutation Rate
    vector<Solution> population;    // Population
    vector<Solution> selection; // Selection of population
    vector<Solution> generation;    // Generation of Selection

    // Constructor
    GeneAlgorithm(int size, int crossRate, int mutationRate, int &facilityNum, int &customerNum, vector<int> &capicity, vector<int> &demand);
    // Select from population
    void Selection(vector<int> &open, vector<vector<int> > &allocate, vector<int> &capicity, vector<int> &demand);
    // Generate new population with CrossOver and mutation within selection
    void CrossAndMutate(vector<int> &capicity, vector<int> &open, vector<vector<int> > &allocate, vector<int> &demand);
    // Mutate within generation
    vector<int>& Mutation(vector<int> &g);
    // Get the best Solution in population
    Solution bestSolution(vector<int> &open, vector<vector<int> > &allocate, vector<int> &capicity, vector<int> &demand);
};

// Costructor of Genetic Algorithm and initialize a popultion with populationSize
// <Param>
//      int size: the population size
//      int crossRate: the cross over rate
//      int mutationRate: the mutation rate
//      int facilityNum: the number of facilities
//      int customerNum: the number of customers
//      vector<int> &capicity: the capicity of facilities
//      vector<int> &demand: the demand of customers
// </Param>
GeneAlgorithm::GeneAlgorithm(int size, int crossRate, int mutationRate,
    int &facilityNum, int &customerNum, vector<int> &capicity, vector<int> &demand) {
    this->crossRate = crossRate;
    this->mutationRate = mutationRate;

    for (int i = 0; i < size; ++i) {
        Solution temp(facilityNum, customerNum);
        while(!temp.isValid(capicity, demand) || std::count(this->population.begin(), this->population.end(), temp) != 0) {
            temp = Solution(facilityNum, customerNum);
        }
        this->population.push_back(temp);
    }

    this->selection.resize(size/2);
    this->generation.resize(size/2);
}

// Perform selection on the current population.
// <Param>
//      vector<int> &open: the open cost of facilities
//      vector<vector<int> > &allocate: the allocation cost of customers to facilities
//      vector<int> &capicity: the capicity of facilities
//      vector<int> &demand: the demand of customers
// </Param>
// <Return>
//	    Void
// </Return>
void GeneAlgorithm::Selection(vector<int> &open, vector<vector<int> > &allocate, vector<int> &capicity, vector<int> &demand) {
    int size = this->selection.size();
    int population_size = this->population.size();
    vector<bool> visited(population_size, 0);
    int l = 0, r = 0;
    this->selection[0] = this->bestSolution(open, allocate, capicity, demand);
    for (int i = 1; i < size; ++i) {
        l = rand() % population_size;
        while (visited[l])	l = rand() % population_size;
		r = rand() % population_size;
        while (r == l || visited[r])	r = rand() % population_size;
        this->selection[i] = (this->population[l].getCost(open, allocate, capicity, demand) > this->population[r].getCost(open, allocate, capicity, demand) ?
            this->population[r] : this->population[l]);
    }
}

// Perform crossing over and mutating on the selection and
// generate the new population.
// <Param>
//      vector<int> &capicity: the capicity of facilities
//      vector<int> &open: the open cost of facilities
//      vector<vector<int> > &allocate: the allocation cost of customers to facilities
//      vector<int> &demand: the demand of customers
// </Param>
// <Return>
//	    Void
// </Return>
void GeneAlgorithm::CrossAndMutate(vector<int> &capicity, vector<int> &open, vector<vector<int> > &allocate, vector<int> &demand) {
    int f = 0, m = 0, p = 0, l = 0, r = 0;
    int size = this->selection.size();
    int i = 0, tryCnt = 0;
    while (i < size) {
		f = i;  // father
		m = i + 1;  // mother
    	p = rand() % 100;   // probability
        if (p <= this->crossRate) {
        	l = rand() % (this->selection[f].allocate.size());  // 'gene segment' start point
        	r = rand() % (this->selection[f].allocate.size());  // 'gene segment' end point
			if (l > r)	std::swap(l, r);
			vector<int> g1; // generation-1 gene
			vector<int> g2; // generation-2 gene
            for (int j = 0; j < this->selection[f].allocate.size(); ++j) {
                if (l <= j && j <= r) {
					g1.push_back(this->selection[m].allocate[j]);
					g2.push_back(this->selection[f].allocate[j]);
                } else {
                    g1.push_back(this->selection[f].allocate[j]);
                    g2.push_back(this->selection[m].allocate[j]);
                }
            }
            // mutation
            g1 = this->Mutation(g1);
            g2 = this->Mutation(g2);
            
            // new generation solution
            Solution s1(g1, (int)(this->selection[f].serving.size()));
            Solution s2(g2, (int)(this->selection[f].serving.size()));
            if (!s1.isValid(capicity, demand) && !s2.isValid(capicity, demand)) {
                tryCnt += 1;
                // if not valid solution, try another 1000 times
                // if still not valid after 1000 times, put into the generation
                if (tryCnt < 1000)  continue;
            }
            this->generation[i] = s1;
            this->generation[i+1] = s1;
			i += 2;
		}
    }

    // Generate new population
    int index = 0;
    for (int i = 0; i < this->selection.size(); ++i) {
        this->population[index++] = this->selection[i];
    }

    for (int i = 0; i < this->generation.size(); ++i) {
        this->population[index++] = this->generation[i];
    }
    
    // Randomly reorder the population to make fairness
    std::random_shuffle(this->population.begin(), this->population.end());
    #if DEBUG   // debug mode: print the population
	for (int i = 0; i < this->population.size(); ++i) {
    	cout << "population-" << i << ":" << this->population[i].getCost(open, allocate, capicity, demand) << endl;
    }
	#endif
}

// Performs mutation on a 'gene' based on the mutation rate.
// <Param>
//      vector<int> &g: the gene to be performed on
// </Param>
// <Return>
//		the mutated gene
// </Return>
vector<int>& GeneAlgorithm::Mutation(vector<int> &g) {
    int p = 0, l = 0, temp = 0;
    int size = g.size();
    for (int i = 0; i < size; ++i) {
        p = rand() % 100;
        if (p < this->mutationRate) {
            l = rand() % (g.size());
            temp = g[l];
            g[l] = 1 + rand() % (this->selection[0].serving.size());
		}
    }
    return g;
}

// Find the 'best' solution in the population
// <Param>
//      vector<int> &open: the open cost of facilities
//      vector<vector<int> > &allocate: the allocation cost of customers to facilities
//      vector<int> &capicity: the capicity of facilities
//      vector<int> &demand: the demand of customers
// </Param>
// <Return>
//		Solution: the 'best' solution
// </Return>
Solution GeneAlgorithm::bestSolution(vector<int> &open, vector<vector<int> > &allocate, vector<int> &capicity, vector<int> &demand) {
    int cost = INT_MAX, index = 0, temp = 0;
    int size = this->population.size();
    for (int i = 0; i < size; ++i) {
        temp = this->population[i].getCost(open, allocate, capicity, demand);
        if (cost > temp) {
            cost = temp;
            index = i;
        }
    }
    return this->population[index];
}

#endif
