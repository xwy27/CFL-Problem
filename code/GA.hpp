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
    GeneAlgorithm(int size, int crossRate, int mutationRate, int &facilityNum, int &customerNum, vector<int> &capicity);
    // Select from population
    void Selection(vector<int> &open, vector<vector<int> > &allocate);
    // CrossOver within selection
    void CrossOver(vector<int> &capicity);
    // Mutate within generation
    void Mutation(vector<int> &capicity);
    // Get the best Solution in population
    Solution bestSolution(vector<int> &open, vector<vector<int> > &allocate);
};

GeneAlgorithm::GeneAlgorithm(int size, int crossRate, int mutationRate,
    int &facilityNum, int &customerNum, vector<int> &capicity) {
    this->crossRate = crossRate;
    this->mutationRate = mutationRate;

    for (int i = 0; i < size; ++i) {
        Solution temp(facilityNum, customerNum);
        while(!temp.isValid(capicity) || std::count(this->population.begin(), this->population.end(), temp) != 0) {
            temp = Solution(facilityNum, customerNum);
        }
        this->population.push_back(temp);
    }

    this->selection.resize(size/2);
    this->generation.resize(size/2);
}

void GeneAlgorithm::Selection(vector<int> &open, vector<vector<int> > &allocate) {
	//srand((unsigned)time(NULL));
    int size = this->selection.size();
    int l = 0, r = 0;
    for (int i = 0; i < size; ++i) {
        l = rand() % size;
        r = rand() % size;
        while (r == l) {
            r = rand() % size;
        }
        this->selection[i] = (this->population[l].getCost(open, allocate) > this->population[r].getCost(open, allocate) ?
            this->population[r] : this->population[l]);
    }
}

void GeneAlgorithm::CrossOver(vector<int> &capicity) {
	//srand((unsigned)time(NULL));
    int f = 0, m = 0, p = 0, temp = 0, l = 0, r = 0;
    int size = this->selection.size();
    int i = 0;
    while (i < size) {
        f = rand() % size;
        m = rand() % size;
        while(m == f) {
            m = rand() % size;
        }
        p = rand() % 100;
        if (p <= this->crossRate) {
            l = rand() % this->selection[f].allocate.size();
            r = l + rand() % (this->selection[f].allocate.size() - l + 1);
			//l = this->selection[f].allocate.size() / 2;
            vector<int> g1;
            vector<int> g2;
            for (int j = 0; j < this->selection[f].allocate.size(); ++j) {
                if (j < l) {
                    g1.push_back(this->selection[f].allocate[j]);
                    g2.push_back(this->selection[m].allocate[j]);
                } else {
                    g1.push_back(this->selection[m].allocate[j]);
                    g2.push_back(this->selection[f].allocate[j]);
                }
            }
            Solution s1(g1, (int)(this->selection[f].serving.size()));
            Solution s2(g2, (int)(this->selection[f].serving.size()));
            if (!s1.isValid(capicity) || !s2.isValid(capicity)) {
                continue;
            }
            this->generation[i] = s1;
            this->generation[i+1] = s2;
            i += 2;
        }
    }
}

void GeneAlgorithm::Mutation(vector<int> &capicity) {
	//srand((unsigned)time(NULL));
    int p = 0, l = 0, r = 0;
    int size = this->generation.size();
    for (int i = 0; i < size; ++i) {
        p = rand() % 100;
        if (p <= this->mutationRate) {
            vector<int> temp = this->generation[i].allocate;
            l = rand() % (this->generation[i].allocate.size());
            r = l + rand() % (this->generation[i].allocate.size() - l + 1);
            for (int j = l; j < r; ++j) {
                this->generation[i].allocate[j] = 1 + rand() % this->generation[i].serving.size();
            }
            if (!this->generation[i].isValid(capicity)) {
                this->generation[i].allocate = temp;
            }
        }
    }

    int index = 0, max_size = this->population.size();
    for (int i = 0; i < this->selection.size(); ++i) {
        this->population[index++] = this->selection[i];
    }

    for (int i = 0; this->generation.size(); ++i) {
    	if (index >= max_size)	break;
        this->population[index++] = this->generation[i];
    }
}

Solution GeneAlgorithm::bestSolution(vector<int> &open, vector<vector<int> > &allocate) {
    int cost = INT_MAX, index = 0, temp = 0;
    int size = this->population.size();
    for (int i = 0; i < size; ++i) {
        temp = this->population[i].getCost(open, allocate);
        if (cost > temp) {
            cost = temp;
            index = i;
        }
    }
    return this->population[index];
}

#endif
