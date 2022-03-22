#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <sstream>
#include "Timer.h"
#include "DataMatrix.h"
#include "SW.h"
using namespace std;

Timer myTimer;
DataMatrix dataMatrix;
SW* sw = new SW();

vector<string> fileNames;
vector<int> graphSize;
vector<int> solution;
vector<int> iterations;
vector<int> bestSolution;
vector<int> timeBreak;
vector<int> era;
vector<int> beginSolution;
vector<float> coolingTempFactor;
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
				era.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				beginSolution.push_back(stoi(temp));
				temp = "";
				ss >> temp;
				coolingTempFactor.push_back(stoi(temp));
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
	file << "Nazwa pliku;Czas[s];Blad wzgledny;Poprawny wynik;Czas rozwi¹zania[s];Koszt;Droga" << endl;

	for (int i = 0; i < fileNames.size(); i++)
	{
		dataMatrix.readFromFile(fileNames[i]);
		if (iterations[i] == 0)
			continue;

		cout << "Nazwa pliku : " << fileNames[i] << endl;

		dataMatrix.coolingTemp = coolingTempFactor[i];
		dataMatrix.stopTime = timeBreak[i];

		vector<Path> results;
		vector<float> times;
		for (int k = 0; k < iterations[i]; k++) {
			myTimer.startTimer();
			Path result = sw->runAlgorithm(dataMatrix, era[i], beginSolution[i], 15);
			myTimer.stopTimer();
			results.push_back(result);
			times.push_back(myTimer.timeCounter());
		}

		cout << "Poprawny wynik: " << bestSolution[i] << endl;
		cout << "Czas algorytmu[s]: \tBlad wzgledny: \t Czas rozwiazania: \tKoszt: \tDroga:" << endl;
		for (int k = 0; k < times.size(); k++) {
			cout << times[k] << " \t" << (abs(bestSolution[i] / 1.0 - results[k].pathCost / 1.0) / bestSolution[i]) * 100 << "%" << " \t" << results[k].time << " \t" << results[k].pathCost << " \t";
			for (int j = 0; j < results[k].path.size()-1; j++)
			{
				cout << results[k].path[j] << "->";
			}
			cout << results[k].path[results[k].path.size() - 1] << endl;
		}

		for (int k = 0; k < times.size(); k++) {
			file << fileNames[i] << ";";
			file << times[k] <<";" << (abs(bestSolution[i] / 1.0 - results[k].pathCost / 1.0) / bestSolution[i]) * 100 << ";" << bestSolution[i] << ";" << results[k].time << ";" << results[k].pathCost << ";";
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
	//int correctSolution = 2755;
	vector<int> nodes;
	//nodes.push_back(0);
	Path result = Path(nodes, 0, 0);
	do {
		cout << "1. Wczytanie danych z pliku." << endl;
		cout << "2. Wprowadzenie kryterium stopu." << endl;
		cout << "3. Ustawienie wspolczynnika zmiany temp." << endl;
		cout << "4. Uruchomianie algorytmu SW" << endl;
		cout << "5. Testy" << endl;
		cout << "0. Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = _getche();
		cout << endl;
		int value = 0; 
		float value2 = 0;
		//float time = 0;
		while (option < 48 && option > 52)
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
			do {
				cout << "Podaj kryterium stopu w sekundach:\n";
				cin >> value;
			} while (value <= 0);
			dataMatrix.stopTime = value;
			break;
		case '3':
			system("cls");
			do {
				cout << "Podaj wspolczynnik zmiany temperatury z przedzialu <0.85, 1):\n";
				cin >> value2;
			} while (value2 < 0.85 || value2 >= 1);
			dataMatrix.coolingTemp = value2;
			break;
		case '4':
			system("cls");
			myTimer.startTimer();
			result = sw->runAlgorithm(dataMatrix, 300, 300, 15);
			myTimer.stopTimer();

			cout << "Rozwiazanie dla pliku: " << fileName << endl;
			cout << "Calkowity czas[s]: \t Czas znalezienia wyniku[s]: \t Koszt:" << endl;
			cout << myTimer.timeCounter()<<" \t" <<result.time << " \t " << result.pathCost << endl;
			cout << "Droga:" << endl;
			for (int j = 0; j < result.path.size() - 1; j++)
			{
				cout << result.path[j] << "->";
			}
			cout << result.path[result.path.size() - 1] << ";" << endl;
			system("pause");
			break;
		case '5':
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