#pragma once

#include "Method.h"

class BruteForce : public Method
{
private:
	// Metoda modyfikuje wêz³y œcie¿ki tworz¹c nowe œcie¿ki
	void checkingNextPaths(vector<int> checkedPath, int first, int last);
	// Metoda oblicza koszt œcie¿ki
	int calculatePathCost(vector<int> nodes);
	// Metoda zamienia miejscami wêz³y œcie¿ki
	void swapNodes(int& firstNumber, int& secondNumber);

public:
	string name = "Brute Force";
	// Tablica kosztów przejœæ
	int** costsMatrix;
	// Minimalna œcie¿ka
	vector<int> minPath;
	// Koszt minimalnej œcie¿ki
	int minPathCost;

	BruteForce();
	~BruteForce();

	// Metoda wyliczaj¹ca rozwi¹zanie za pomoc¹ przeszukiwania zupe³nego
	Path runMethod(DataMatrix data, vector<int> nodes);
};