#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <sstream>
#include "Timer.h"
#include "DataMatrix.h"
#include "GeneticAlgorithm.h"
#include "Result.h"
using namespace std;

Timer myTimer;
DataMatrix dataMatrix;
GeneticAlgorithm* ga = new GeneticAlgorithm();

vector<string> fileNames;
vector<int> iterations;
vector<int> bestSolution;
vector<int> timeBreak;
vector<int> beginPopulation;
vector<float> mutationRate;
vector<float> crossingRate;
vector<int> firstMutation;
vector<int> solution;
string outFile;

vector<string> iniRead(string name) {
	vector<string> vec;
	ifstream input(name);
	string line;

	int kek = 0;

	while (getline(input, line))
	{
		kek++;
	}
	input.clear();
	input.seekg(0);
	for (int i = 0; i < kek; i++) {

		getline(input, line);
		stringstream ss;
		ss << line;
		string found;
		string temp;

		while (!ss.eof()) {
			ss >> temp;

			if (stringstream(temp) >> found) {
				if (i == kek - 1) {
					outFile = found;
					break;
				}
				fileNames.push_back(found);
				temp = "";
				ss >> temp;
				iterations.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				bestSolution.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				timeBreak.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				beginPopulation.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				mutationRate.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				crossingRate.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				firstMutation.push_back(stoi(temp));	
			}
			temp = "";
		}
	}
	input.close();
	return vec;
}

void test() {
	iniRead("myini.txt");
	ofstream file(outFile);
	file << "Nazwa pliku;Czas[s];Populacja;Blad wzgledny;Poprawny wynik;Koszt;Droga" << endl;

	for (int i = 0; i < fileNames.size(); i++)
	{
		dataMatrix.readFromFile(fileNames[i]);
		if (iterations[i] == 0)
			continue;

		cout << "Nazwa pliku : " << fileNames[i] << endl;

		dataMatrix.initialPopulationSize = beginPopulation[i];
		dataMatrix.stopTime = timeBreak[i];
		dataMatrix.mutationRate = mutationRate[i];
		dataMatrix.crossingRate = crossingRate[i];
		if (firstMutation[i] == 1)
			dataMatrix.firstMutation = true;
		else
			dataMatrix.firstMutation = false;

		vector<Result> results;
		for (int k = 0; k < iterations[i]; k++) {
			Result result = ga->runAlgorithm(dataMatrix);
			results.push_back(result);
		}

		cout << "Poprawny wynik: " << bestSolution[i] << endl;
		cout << "Czas: \tPopulacja: \tBlad wzgledny: \tKoszt: \tDroga:" << endl;
		for (int k = 0; k < results.size(); k++) {
			cout << timeBreak[i] << " \t" << beginPopulation[i] << " \t" << (abs(bestSolution[i] / 1.0 - results[k].pathCost / 1.0) / bestSolution[i]) * 100 << "%" <<  " \t" << results[k].pathCost << " \t";
			for (int j = 0; j < results[k].path.size() - 1; j++)
			{
				cout << results[k].path[j] << "->";
			}
			cout << results[k].path[results[k].path.size() - 1] << endl;
		}

		for (int k = 0; k < results.size(); k++) {
			file << fileNames[i] << ";";
			file << timeBreak[i] << ";" << beginPopulation[i] << ";" << (abs(bestSolution[i] / 1.0 - results[k].pathCost / 1.0) / bestSolution[i]) * 100 << ";" << bestSolution[i] << ";" << results[k].pathCost << ";";
			for (int j = 0; j < results[k].path.size() - 1; j++)
			{
				file << results[k].path[j] << "->";
			}
			file << results[k].path[results[k].path.size() - 1] << ";" << endl;
		}
		cout << endl;
	}
	file.close();

	system("pause");
}

void drawMenu() {
	string fileName;
	char option;
	string name;
	vector<int> nodes;
	Result result = Result(nodes, 0);
	do {
		cout << "--- ALGORYTM GENETYCZNY ---" << endl;
		cout << "1. Wczytanie danych z pliku." << endl;
		cout << "2. Wprowadzenie kryterium stopu." << endl;
		cout << "3. Ustawienie wielkosci populacji poczatkowej." << endl;
		cout << "4. Ustawienie wspolczynnika mutacji" << endl;
		cout << "5. Ustawienie wspolczynnika krzyzowania." << endl;
		cout << "6. Wybor metody mutacji." << endl;
		cout << "7. Uruchom algorytm." << endl;
		cout << "8. Testy." << endl;
		cout << "0. Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = _getche();
		cout << endl;
		int method = 0;
		float value2 = 0;
		int stop = 0;
		int population = 0;
		float mutation = 0;
		float cross = 0;

		while (option < 48 && option > 56)
		{
			option = _getche();
			cout << endl;
		}
		switch (option) {
		case '1':
			system("cls");
			cout << "Podaj nazwe wczytywanego pliku:\n";
			getline(cin, fileName);
			system("cls");
			dataMatrix.readFromFile(fileName);
			dataMatrix.printData();
			break;
		case '2':
			system("cls");
				cout << "Podaj kryterium stopu w sekundach:\n";
				cin >> stop;
			dataMatrix.stopTime = stop;
			break;
		case '3':
			system("cls");
				cout << "Podaj wielkosc populacji poczatkowej:\n";
				cin >> population;
			dataMatrix.initialPopulationSize = population;
			break;
		case '4':
			system("cls");
				cout << "Podaj wielkosc wspolczynnika mutacji:\n";
				cin >> mutation;
			dataMatrix.mutationRate = mutation;
			break;
		case '5':
			system("cls");
				cout << "Podaj wielkosc wspolczynnika krzyzowania:\n";
				cin >> cross;
			dataMatrix.crossingRate = cross;
			break;
		case '6':
			system("cls");
			cout << "Wybierz metode mutacji:\n";
			cout << "1. Transposition\n";
			cout << "2. Insertion\n";
			cout << "Podaj numer:\n";
			cin >> method;
			if (method == 1) dataMatrix.firstMutation = true;
			else if (method == 2) dataMatrix.firstMutation = false;
			break;
		case '7':
			system("cls");
			myTimer.startTimer();
			result = ga->runAlgorithm(dataMatrix);
			myTimer.stopTimer();

			cout << "Rozwiazanie dla pliku: " << fileName << endl;
			cout << "Calkowity czas[s]: \t Koszt:" << endl;
			cout << myTimer.timeCounter() << " \t " << result.pathCost << endl;
			cout << "Droga:" << endl;
			for (int j = 0; j < result.path.size() - 1; j++)
			{
				cout << result.path[j] << "->";
			}
			cout << result.path[result.path.size() - 1] << ";" << endl;
			system("pause");
			break;
		case '8':
			system("cls");
			test();
			break;
		}
	} while (option != '0');
}

int main()
{
	//menu g³ówne
	drawMenu();
	return 0;
}