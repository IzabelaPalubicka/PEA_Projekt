#pragma once
#include "Result.h"
#include "DataMatrix.h"
#include "Timer.h"
#include <chrono>
#include <algorithm>
using namespace std;

struct Specimen
{
	vector<int> chromosomes; // permutacja miast
	int adaptationValue; // koszt permutacji
};



class GeneticAlgorithm
{
private: 
	int populationSize;
	int parentsSize;
	int childrenSize;
	int eliteSize;

	vector<Specimen> population;
	vector<Specimen> temporaryPopulation;
	vector<Specimen> parents;
	vector<int> firstChildChromosomes;
	vector<int> secondChildChromosomes;

	DataMatrix data;

	int crossingPoints;
	int maxMutationsAmount;
	int localOptimalization = true;
	int firstMutation = true;

	void generateInitialPopulation();
	void generateNewPopulation();
	int adaptationAssessment(vector<int> chromosomes);
	void selection();
	void crossing();
	void mutating();
	float generateProbability();


	
public:
	GeneticAlgorithm();
	~GeneticAlgorithm();
	Result runAlgorithm(DataMatrix dataMatrix);
};

