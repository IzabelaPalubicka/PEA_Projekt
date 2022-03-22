#pragma once

#include "Method.h"

class BruteForce : public Method
{
private:
	// Metoda modyfikuje w�z�y �cie�ki tworz�c nowe �cie�ki
	void checkingNextPaths(vector<int> checkedPath, int first, int last);
	// Metoda oblicza koszt �cie�ki
	int calculatePathCost(vector<int> nodes);
	// Metoda zamienia miejscami w�z�y �cie�ki
	void swapNodes(int& firstNumber, int& secondNumber);

public:
	string name = "Brute Force";
	// Tablica koszt�w przej��
	int** costsMatrix;
	// Minimalna �cie�ka
	vector<int> minPath;
	// Koszt minimalnej �cie�ki
	int minPathCost;

	BruteForce();
	~BruteForce();

	// Metoda wyliczaj�ca rozwi�zanie za pomoc� przeszukiwania zupe�nego
	Path runMethod(DataMatrix data, vector<int> nodes);
};