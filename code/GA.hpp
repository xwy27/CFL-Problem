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
    // CrossOver within selection
    void CrossAndMutate(vector<int> &capicity, vector<int> &open, vector<vector<int> > &allocate, vector<int> &demand);
    // Mutate within generation
    vector<int>& Mutation(vector<int> &g);
    // Get the best Solution in population
    Solution bestSolution(vector<int> &open, vector<vector<int> > &allocate, vector<int> &capicity, vector<int> &demand);
};

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

void GeneAlgorithm::CrossAndMutate(vector<int> &capicity, vector<int> &open, vector<vector<int> > &allocate, vector<int> &demand) {
    int f = 0, m = 0, p = 0, temp = 0, l = 0, r = 0;
    int size = this->selection.size();
    int i = 0, tryCnt = 0;
    while (i < size) {
		f = i;
		m = i + 1;
    	p = rand() % 100;
        if (p <= this->crossRate) {
        	l = rand() % (this->selection[f].allocate.size());
        	r = rand() % (this->selection[f].allocate.size());
			if (l > r)	std::swap(l, r);
			vector<int> g1;
			vector<int> g2;
            for (int j = 0; j < this->selection[f].allocate.size(); ++j) {
                if (l <= j && j <= r) {
					g1.push_back(this->selection[m].allocate[j]);
					g2.push_back(this->selection[f].allocate[j]);
                } else {
                    g1.push_back(this->selection[f].allocate[j]);
                    g2.push_back(this->selection[m].allocate[j]);
                }
            }
            g1 = this->Mutation(g1);
            g2 = this->Mutation(g2);
            Solution s1(g1, (int)(this->selection[f].serving.size()));
            Solution s2(g2, (int)(this->selection[f].serving.size()));
            if (!s1.isValid(capicity, demand) && !s2.isValid(capicity, demand)) {
                tryCnt += 1;
                if (tryCnt < 1000)  continue;
            }
            this->generation[i] = s1;
            this->generation[i+1] = s1;
			i += 2;
		}
    }

    int index = 0;
    for (int i = 0; i < this->selection.size(); ++i) {
        this->population[index++] = this->selection[i];
    }

    for (int i = 0; i < this->generation.size(); ++i) {
        this->population[index++] = this->generation[i];
    }
    
    std::random_shuffle(this->population.begin(), this->population.end());
    #if DEBUG
	for (int i = 0; i < this->population.size(); ++i) {
    	cout << "population-" << i << ":" << this->population[i].getCost(open, allocate, capicity, demand) << endl;
    }
	#endif
}

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
