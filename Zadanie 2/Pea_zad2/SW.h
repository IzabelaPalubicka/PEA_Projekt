#pragma once

#include <vector>
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iomanip> 
#include <chrono> 
#include "DataMatrix.h"
#include "Timer.h"
#include "Path.h"
using namespace std;
using namespace chrono;

class SW
{
public:
	Path runAlgorithm(DataMatrix dataMatrix, int eraFactor, int initialTempIterator, int minTempExponent);
	int calculateInitialTemperature(DataMatrix data, vector<int> bestPermutation, int initialTempPrecision);
	vector<int> generateInitialSolution(DataMatrix );
	int calculatePathCost(vector<int> permutacja, int** costsMatrix);
};