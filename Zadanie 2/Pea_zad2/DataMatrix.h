#pragma once
#include <string>
using namespace std;

class DataMatrix
{
public:
	int size; //ilo�� wierzcho�k�w
	int** matrix; //tabela koszt�w
	float coolingTemp; //wsp�czynnik sch�adzania
	int stopTime; //kryterium stopu

	DataMatrix();
	~DataMatrix();
	// Wczytywanie danych z pliku 
	void readFromFile(string fileName);
	void printData();
};
