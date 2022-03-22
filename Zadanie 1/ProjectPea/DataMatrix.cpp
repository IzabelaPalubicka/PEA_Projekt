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
	// Je�li tablica nie by�a pusta, czyszczenie jej
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

	// Je�li plik zosta� poprawnie otwarty
	if (file.good()) {

		//wczytanie ilo�ci w�z��w
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

		// Wpisywanie kolejnych odczytywanych kraw�dzi z pliku do macierzy
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				// Zmienna przechowuj�ce koszt przej�cia z miasta do miasta
				int value;
				// Wczytywanie z pliku
				file >> value;
				// Dodanie do macierzy wagi kraw�dzi
				matrix[i][j] = value;
			}
		}
		file.close();
	}
}

// Wy�wietlenie zawarto�ci macierzy
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

// Generowanie koszt�w przej�� dla macierzy o podanej ilo�ci miast
void DataMatrix::generateRandomData(int generatedSize)
{
	// Zwi�kszenie losowo�ci generacji
	srand(time(0));
	// Je�eli tablica nie by�a pusta
	if (matrix != nullptr)
	{
		for (int i = 0; i < size; i++)
			delete[] matrix[i]; //uwolnienie pamieci
		delete[] matrix; //uwolnienie pamieci
		matrix = nullptr;
		size = 0;
	}
	// Ustwienie nowej ilo�ci w�z��w
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
		// Wpisywanie wygenerowanych koszt�w przej�� mi�dzy miastami
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
