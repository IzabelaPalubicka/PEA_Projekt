#include "dataMatrix.h"
#include <fstream>
#include <iostream>

DataMatrix::DataMatrix()
{
}

DataMatrix::~DataMatrix()
{
}

// Wczytywanie danych z pliku
void DataMatrix::readFromFile(string fileName)
{
	// Jeœli tablica nie by³a pusta, czyszczenie jej
	if (matrix != nullptr)
	{
		for (int i = 0; i < size; i++)
			delete[] matrix[i]; //uwolnienie pamieci
		delete[] matrix; //uwolnienie pamieci
		matrix = nullptr;
		size = 0;
	}
	// Otwieranie pliku
	ifstream file;
	file.open(fileName, ios::in);

	// Jeœli plik zosta³ poprawnie otwarty
	if (file.good()) {

		//wczytanie iloœci wêz³ów
		file >> size;

		// Alokacja pamieci
		matrix = new int* [size];
		for (int i = 0; i < size; i++)
		{
			// Alokacja pamieci
			matrix[i] = new int[size];
			//wpisanie wartosci do tablicy jako INT_MAX
			for (int j = 0; j < size; j++)
				matrix[i][j] = INT_MAX;
		}

		// Wpisywanie kolejnych odczytywanych krawêdzi z pliku do macierzy
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				// Zmienna przechowuj¹ce koszt przejœcia z miasta do miasta
				int value;
				// Wczytywanie z pliku
				file >> value;
				// Dodanie do macierzy wagi krawêdzi
				matrix[i][j] = value;
			}
		}
		file.close();
	}
}

// Wyœwietlenie zawartoœci macierzy
void DataMatrix::printData()
{
	cout << "-------------------------------" << endl;
	cout << "Ilosc miast: " << size << endl;
	cout << "-------------------------------" << endl << endl;
	if (matrix != nullptr) {
		cout << "    ";

		for (int i = 0; i < size; i++)
		{
			if (i < 10) cout << "  " << i << " ";
			if (i >= 10 && i < 100) cout << " " << i << " ";
			if (i >= 100) cout << i << " ";
		}
		cout << endl << "    ";

		for (int i = 0; i < size; i++)
		{
			cout << "____";
		}
		cout << endl;

		for (int i = 0; i < size; i++)
		{
			if (i < 10) cout << "  " << i << " |";
			if (i >= 10 && i < 100) cout << " " << i << " |";
			if (i >= 100) cout << i << " |";

			for (int j = 0; j < size; j++)
			{
				if (matrix[i][j] != INT_MAX)
				{
					if (matrix[i][j] < 10 && matrix[i][j] >= 0) cout << "  " << matrix[i][j] << " ";
					else if ((matrix[i][j] >= 10 && matrix[i][j] < 100) || matrix[i][j] < 0) cout << " " << matrix[i][j] << " ";
					else if (matrix[i][j] >= 100) cout << matrix[i][j] << " ";
				}
				else if (matrix[i][j] == INT_MAX) cout << " N ";
			}
			cout << endl;
		}
	}
	else cout << "Brak zawartosci do wyswietlenia";
	cout << endl;
}

// Generowanie kosztów przejœæ dla macierzy o podanej iloœci miast
void DataMatrix::generateRandomData(int generatedSize)
{
	// Zwiêkszenie losowoœci generacji
	srand(time(0));
	// Je¿eli tablica nie by³a pusta
	if (matrix != nullptr)
	{
		for (int i = 0; i < size; i++)
			delete[] matrix[i]; //uwolnienie pamieci
		delete[] matrix; //uwolnienie pamieci
		matrix = nullptr;
		size = 0;
	}
	// Ustwienie nowej iloœci wêz³ów
	size = generatedSize;
	if (size > 2) {
		// Alokacja pamieci
		matrix = new int* [size];
		for (int i = 0; i < size; i++)
		{   // Alokacja pamieci
			matrix[i] = new int[size];
			// Wpisanie wartosci do tablicy jako -1
			for (int j = 0; j < size; j++)
				matrix[i][j] = -1;
		}
		// Wpisywanie wygenerowanych kosztów przejœæ miêdzy miastami
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i != j) matrix[i][j] = 1 + rand() % 100;
			}
		}
	}
	else cout << endl << "Podano za maly rozmiar!" << endl;
}
