#pragma once
#include <string>
using namespace std;

class DataMatrix
{
public:
	int size;
	int** matrix;
	int distanceResult;

	DataMatrix();
	~DataMatrix();
	void readFromFile(string fileName);
	void printData();
	void generateRandomData(int generatedSize);
};

