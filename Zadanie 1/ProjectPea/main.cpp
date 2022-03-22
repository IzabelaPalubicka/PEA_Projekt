#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include "Method.h"
#include "BruteForce.h"
#include "BranchAndBound.h"
#include "DataMatrix.h"
#include "Path.h"
#include "Timer.h"
using namespace std;

Timer myTimer;
DataMatrix dataMatrix;
BranchAndBound BnB;
BruteForce BF;

// Funkcja wypisuj¹ca w konsoli czas dzia³ania algorytmu
void printPath(vector<int> nodes)
{
	cout << "Sciezka: " << endl;
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		cout << nodes[i] << " - ";
	}
	cout << nodes[nodes.size() - 1];
	cout << endl << endl;
}

vector<int> getVector(int size)
{
	vector<int> result;
	for (int i = 0; i < size; i++)
		result.push_back(i);
	return result;
}

void tests() {

	cout << "Czesc testowa" << endl;
	cout << "Brute Force" << endl;

	ofstream file;
	int number;
	file.open("pomiary.csv", ios::app);

	file << "Brute Force" << ", " << endl;
	file << "Instancja" << ", " << "Czas [ms]" << ", " << endl;
	for (int i = 6; i < 13; i++)
	{
		float bfTime = 0;
		cout << i << "..." << endl;

		for (int j = 0; j < 100; j++)
		{
			dataMatrix.generateRandomData(i);
			vector<int> nodes = getVector(dataMatrix.size);
			myTimer.startTimer();
			BF.runMethod(dataMatrix, nodes);
			myTimer.stopTimer();
			bfTime += myTimer.timeCounter();
		}

		file << i << ", "  << bfTime / 100<< ", " << endl;
	}

	cout << "Branch and Bound" << endl;
	file << endl;
	file << "Branch and Bound" << ", " << endl;
	file << "Instancja" << ", " << "Czas [ms]" << ", " << "Przerwania [%]" << ", " << endl;
	for (int i = 6; i < 19; i+=2)
	{
		float bnbTime = 0;
		cout << i << "..." << endl;
		int counter = 0;
		for (int j = 0; j < 100; j++)
		{
			dataMatrix.generateRandomData(i);
			vector<int> nodes = getVector(dataMatrix.size);
			myTimer.startTimer();
			Path solutionPath = BnB.runMethod(dataMatrix, nodes);
			myTimer.stopTimer();
			if (solutionPath.pathCost == 0) counter++;
			bnbTime += myTimer.timeCounter();
		}
		file << i << ", " << bnbTime / 100 << ", " << counter << endl;
	}
}

void drawMenu() {
	string fileName;
	char option;
	string name;
	vector<int> nodes1;
	nodes1.push_back(0);
	Path resultPath = Path(nodes1, 0);

	// Zmienne przechowuj¹ce czasy rozpoczêcia i zakoñczenia pomiaru
	clock_t start, finish;
	do {
		cout << "1. Wczytaj dane z pliku." << endl;
		cout << "2. Wygeneruj losowe dane." << endl;
		cout << "3. Wyswietl dane." << endl;
		cout << "4. Uruchom BF." << endl;
		cout << "5. Uruchom B&B." << endl;
		cout << "6. Testowanie" << endl;
		cout << "0. Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = _getche();
		cout << endl;
		while (option < 48 && option > 54)
		{
			option = _getche();
			cout << endl;
		}
		switch (option) {
		case '1':
			cout << "Podaj nazwe wczytywanego pliku:\n";
			getline(cin, fileName);
			system("cls");
			dataMatrix.readFromFile(fileName);
			dataMatrix.printData();
			break;
		case '2':
			system("cls");
			int generatedSize;
			cout << "Podaj rozmiar wygenerowanej tabeli:\n";
			cin >> generatedSize;
			dataMatrix.generateRandomData(generatedSize);
			break;
		case '3':
			system("cls");
			dataMatrix.printData();
			break;
		case '4':
			system("cls");
			cout << "========" << BF.name << "==============" << endl;
			myTimer.startTimer();
				resultPath = BF.runMethod(dataMatrix, getVector(dataMatrix.size));
				myTimer.stopTimer();
				cout << "Rozwiazanie dla encji: " << dataMatrix.size << "\n\n";
				cout << "Czas: " << myTimer.timeCounter() << " ms\n";
				cout << "Koszt: " << resultPath.pathCost << "\n";
				printPath(resultPath.path);
				break;
		case '5':
			system("cls");
			cout << "========" << BnB.name << "==============" << endl;
			myTimer.startTimer();
			resultPath = BnB.runMethod(dataMatrix, getVector(dataMatrix.size));
			myTimer.stopTimer();
			cout << "Rozwiazanie dla encji: " << dataMatrix.size << "\n\n";
				cout << "Czas: " << myTimer.timeCounter() <<" s\n";
				cout << "Koszt: " << resultPath.pathCost << "\n";
				printPath(resultPath.path);
				break;
		case '6':
			system("cls");
			tests();
			break;
		}
	} while (option != '0');
}


int main()
{
	//manu g³ówne
	drawMenu();
	return 0;
}
