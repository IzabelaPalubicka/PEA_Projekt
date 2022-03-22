#pragma once
#include <string>
using namespace std;

class DataMatrix
{
public:
	int size; //iloœæ wierzcho³ków
	int** matrix; //tabela kosztów
	int stopTime = 120; //kryterium stopu
	int initialPopulationSize = 400; //populacja pocz¹tkowa
	float mutationRate = 0.01; //wspó³czynnik mutacji
	float crossingRate = 0.8; //wspó³czynnik krzy¿owania
	int firstMutation = false;

	DataMatrix();
	~DataMatrix();

	// Wczytywanie danych z pliku 
	void readFromFile(string fileName);
	void printData();
};
