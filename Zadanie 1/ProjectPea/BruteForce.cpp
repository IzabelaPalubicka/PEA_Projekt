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

// Wyliczenie wyniku za pomoc� metody Brute force
Path BruteForce::runMethod(DataMatrix data, vector<int> nodes)
{
	// Przypisanie tablicy koszt�w przej�� z miasta do miasta
	costsMatrix = data.matrix;

	// Przypisanie w�z��w dla pocz�tkowej �cie�ki
	minPath = nodes;
	minPath.push_back(0);

	// Przypisanie pocz�tkowego kosztu �cie�ki 
	minPathCost = calculatePathCost(minPath);

	// Przeszukiwanie kolejnych �cie�ek
	checkingNextPaths(nodes, 1, nodes.size());

	Path resultPath = Path(minPath, minPathCost);
	return resultPath;
}

// Sprawdzanie kolejnych rozwi�za�
void BruteForce::checkingNextPaths(vector<int> checkedPath, int first, int last)
{
	// Czy mo�emy zamieni� w�z�y �cie�ki
	if (first != last) {

		for (int i = first; i < last; i++) {

			swapNodes(checkedPath[first], checkedPath[i]);

			checkingNextPaths(checkedPath, first + 1, last);

			swapNodes(checkedPath[first], checkedPath[i]);
		}
	}
	else {
		// Zamkni�cie cyklu hamiltona
		checkedPath.push_back(0);

		// Przypisanie kosztu badanej �cie�ki
		int costCheckedPath = calculatePathCost(checkedPath);

		// Czy koszt si� poprawi�
		if (costCheckedPath < minPathCost) {
			// Przypisanie nowego rozwi�zania
			minPathCost = costCheckedPath;
			minPath = checkedPath;
		}
	}
}

// Wyliczenie kosztu �cie�ki
int BruteForce::calculatePathCost(vector<int> nodes)
{
	int cost = 0;
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		// Pobieranie koszt�w przej�� pomi�dzy kolejnymi w�z�ami �cie�ki
		cost += costsMatrix[nodes[i]][nodes[i + 1]];
	}
	return cost;
}

// Zamiana miejsc w�z��w
void BruteForce::swapNodes(int& firstNumber, int& secondNumber)
{
	int value = firstNumber;
	firstNumber = secondNumber;
	secondNumber = value;
}