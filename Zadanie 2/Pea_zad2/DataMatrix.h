#pragma once
#include <string>
using namespace std;

class DataMatrix
{
public:
	int size; //iloœæ wierzcho³ków
	int** matrix; //tabela kosztów
	float coolingTemp; //wspó³czynnik sch³adzania
	int stopTime; //kryterium stopu

	DataMatrix();
	~DataMatrix();
	// Wczytywanie danych z pliku 
	void readFromFile(string fileName);
	void printData();
};
