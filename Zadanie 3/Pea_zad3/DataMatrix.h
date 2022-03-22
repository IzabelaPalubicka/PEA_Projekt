#pragma once
#include <string>
using namespace std;

class DataMatrix
{
public:
	int size; //ilo�� wierzcho�k�w
	int** matrix; //tabela koszt�w
	int stopTime = 120; //kryterium stopu
	int initialPopulationSize = 400; //populacja pocz�tkowa
	float mutationRate = 0.01; //wsp�czynnik mutacji
	float crossingRate = 0.8; //wsp�czynnik krzy�owania
	int firstMutation = false;

	DataMatrix();
	~DataMatrix();

	// Wczytywanie danych z pliku 
	void readFromFile(string fileName);
	void printData();
};
