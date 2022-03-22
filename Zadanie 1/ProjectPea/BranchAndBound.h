#pragma once

#include "Method.h"
#include <queue>
#include "Timer.h"

// Wêz³y drzewa przestrrzeni stanów
struct Node
{
	// Przechowuje krawêdzie drzewa przestrzeni stanów
	vector<pair<int, int>> path;

	// Przechowuje zredukowan¹ macierz
	int** reducedMatrix;

	// Przechowuje dolne ograniczenie
	int cost;

	// numer wêz³a
	int vertex;

	// Przechowuje iloœæ odwiedzonych wêz³ów
	int level;
};

// Struktura porz¹dkuj¹ca kolejkê priorytetow¹
struct comparator
{
	bool operator()(const Node* a, const Node* b) const {
		return a->cost > b->cost;
	}
};

class BranchAndBound
{
private:
	// Licznik do przerwañ
	Timer timer;

	// Metoda oblicza dolne ograniczenie œcie¿ki dla wybranego wêz³a
	int calculateReduction(int** reducedMatrix);

	// Metoda tworzy nowy wêze³
	Node* addNode(int** parentMatrix, vector<pair<int, int>> const& path, int level, int parent, int actualNode);

	// Metody do redukowania wierzy i kolumn
	void rowReduction(int** reducedMatrix, int* row);
	void columnReduction(int** reducedMatrix, int* column);

	// Metoda kopiuj¹ca zawartoœæ jednej tablicy do drugiej
	int** copyTable(int** parentTable, int** childTable);

	// Metoda wype³mia tablie wartoœci¹ INT_MAX
	void fillWithInfinities(int* table);

public:
	string name = "Branch and Bound";

	// Tablica kosztów przejœæ
	int** costsMatrix;

	// Iloœæ wêz³ów
	int numberOfNodes;

	BranchAndBound();
	~BranchAndBound();

	// Metoda g³ówna rozwi¹zuj¹ca problem komiwoja¿era za pomoc¹ B&B
	Path runMethod(DataMatrix data, vector<int> nodes);
};