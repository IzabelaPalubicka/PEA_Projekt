#include "BruteForce.h"
#include <iostream>
#include <conio.h>
#include "Path.h"

BruteForce::BruteForce()
{
}

BruteForce::~BruteForce()
{
}

// Wyliczenie wyniku za pomoc¹ metody Brute force
Path BruteForce::runMethod(DataMatrix data, vector<int> nodes)
{
	// Przypisanie tablicy kosztów przejœæ z miasta do miasta
	costsMatrix = data.matrix;

	// Przypisanie wêz³ów dla pocz¹tkowej œcie¿ki
	minPath = nodes;
	minPath.push_back(0);

	// Przypisanie pocz¹tkowego kosztu œcie¿ki 
	minPathCost = calculatePathCost(minPath);

	// Przeszukiwanie kolejnych œcie¿ek
	checkingNextPaths(nodes, 1, nodes.size());

	Path resultPath = Path(minPath, minPathCost);
	return resultPath;
}

// Sprawdzanie kolejnych rozwi¹zañ
void BruteForce::checkingNextPaths(vector<int> checkedPath, int first, int last)
{
	// Czy mo¿emy zamieniæ wêz³y œcie¿ki
	if (first != last) {

		for (int i = first; i < last; i++) {

			swapNodes(checkedPath[first], checkedPath[i]);

			checkingNextPaths(checkedPath, first + 1, last);

			swapNodes(checkedPath[first], checkedPath[i]);
		}
	}
	else {
		// Zamkniêcie cyklu hamiltona
		checkedPath.push_back(0);

		// Przypisanie kosztu badanej œcie¿ki
		int costCheckedPath = calculatePathCost(checkedPath);

		// Czy koszt siê poprawi³
		if (costCheckedPath < minPathCost) {
			// Przypisanie nowego rozwi¹zania
			minPathCost = costCheckedPath;
			minPath = checkedPath;
		}
	}
}

// Wyliczenie kosztu œcie¿ki
int BruteForce::calculatePathCost(vector<int> nodes)
{
	int cost = 0;
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		// Pobieranie kosztów przejœæ pomiêdzy kolejnymi wêz³ami œcie¿ki
		cost += costsMatrix[nodes[i]][nodes[i + 1]];
	}
	return cost;
}

// Zamiana miejsc wêz³ów
void BruteForce::swapNodes(int& firstNumber, int& secondNumber)
{
	int value = firstNumber;
	firstNumber = secondNumber;
	secondNumber = value;
}