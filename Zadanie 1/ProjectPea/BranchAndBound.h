#pragma once

#include "Method.h"
#include <queue>
#include "Timer.h"

// W�z�y drzewa przestrrzeni stan�w
struct Node
{
	// Przechowuje kraw�dzie drzewa przestrzeni stan�w
	vector<pair<int, int>> path;

	// Przechowuje zredukowan� macierz
	int** reducedMatrix;

	// Przechowuje dolne ograniczenie
	int cost;

	// numer w�z�a
	int vertex;

	// Przechowuje ilo�� odwiedzonych w�z��w
	int level;
};

// Struktura porz�dkuj�ca kolejk� priorytetow�
struct comparator
{
	bool operator()(const Node* a, const Node* b) const {
		return a->cost > b->cost;
	}
};

class BranchAndBound
{
private:
	// Licznik do przerwa�
	Timer timer;

	// Metoda oblicza dolne ograniczenie �cie�ki dla wybranego w�z�a
	int calculateReduction(int** reducedMatrix);

	// Metoda tworzy nowy w�ze�
	Node* addNode(int** parentMatrix, vector<pair<int, int>> const& path, int level, int parent, int actualNode);

	// Metody do redukowania wierzy i kolumn
	void rowReduction(int** reducedMatrix, int* row);
	void columnReduction(int** reducedMatrix, int* column);

	// Metoda kopiuj�ca zawarto�� jednej tablicy do drugiej
	int** copyTable(int** parentTable, int** childTable);

	// Metoda wype�mia tablie warto�ci� INT_MAX
	void fillWithInfinities(int* table);

public:
	string name = "Branch and Bound";

	// Tablica koszt�w przej��
	int** costsMatrix;

	// Ilo�� w�z��w
	int numberOfNodes;

	BranchAndBound();
	~BranchAndBound();

	// Metoda g��wna rozwi�zuj�ca problem komiwoja�era za pomoc� B&B
	Path runMethod(DataMatrix data, vector<int> nodes);
};