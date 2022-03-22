#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm()
{
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}

bool sortByAdaptation(Specimen a, Specimen b)
{
	return (a.adaptationValue < b.adaptationValue);
}

// Metoda odpowiadaj¹ca za dzia³anie algorytmu
Result GeneticAlgorithm::runAlgorithm(DataMatrix dataMatrix)
{
	srand(time(0));
	Timer timer = Timer();
	data = dataMatrix;
	//firstMutation = data.firstMutation;

	// przypisanie rozmiaru populacji
	populationSize = data.initialPopulationSize;

	population.clear();
	population.reserve(populationSize);

	// losowe wygenerowanie przyk³adowej populacji
	generateInitialPopulation();

	// elitarnych osobników jest 20% starych osobników
	eliteSize = populationSize * 0.1;
	// bêdzie wybierane zawsze 50% populacji
	parentsSize = data.initialPopulationSize * 0.5;
	// iloœæ dzieci to populacja - elita
	if ((populationSize - eliteSize) % 2 == 1)
		childrenSize = (populationSize - eliteSize) + 1;
	else
		childrenSize = (populationSize - eliteSize);

	timer.startTimer();
	do {
		temporaryPopulation.clear();
		temporaryPopulation.reserve(eliteSize+childrenSize);

		// wybór metod¹ turniejow¹ najlepszych rodziców
		selection();

		// tworzenie nowej populacji 
		for (int i = 0; i < childrenSize/2; i++)
		{
			// przygotowanie wektorów dzieci
			firstChildChromosomes.clear();
			firstChildChromosomes.reserve(data.size + 1);

			secondChildChromosomes.clear();
			secondChildChromosomes.reserve(data.size + 1);

			// krzy¿owanie
			crossing();

			// mutowanie
			mutating();

			//dodanie pierwszego dziecka do populacji tymczasowej
			int adaptation = adaptationAssessment(firstChildChromosomes);

			Specimen newSpecimen;
			newSpecimen.adaptationValue = adaptation;
			newSpecimen.chromosomes = firstChildChromosomes;

			temporaryPopulation.push_back(newSpecimen);

			//dodanie drugiego dziecka do populacji tymczasowej
			adaptation = adaptationAssessment(secondChildChromosomes);

			newSpecimen.adaptationValue = adaptation;
			newSpecimen.chromosomes = secondChildChromosomes;

			temporaryPopulation.push_back(newSpecimen);
		}

		// dodanie osobników elitarnych do dodanych dzieci
		generateNewPopulation();

		population.clear();
		population.reserve(populationSize);
		population = temporaryPopulation;

		timer.stopTimer();
	} while (timer.timeCounter() < data.stopTime);

	// szukanie najlepszego rozwi¹zania
	sort(population.begin(), population.end(), sortByAdaptation);

	Result result = Result();

	result.path = population[0].chromosomes;
	result.pathCost = population[0].adaptationValue;

	return result;
}



// Generowanie populacji pocz¹tkowej
void GeneticAlgorithm::generateInitialPopulation()
{
	for (int i = 0; i < populationSize; i++)
	{
		// Utworzenie osobnika z podstawow¹ œcie¿k¹
		vector<int> path;
		path.reserve(data.size + 1);

		for (int i = 0; i < data.size; i++)
			path.push_back(i);

		path.push_back(0);

		// Przetasowanie miast osobnika
		random_shuffle(path.begin() + 1, path.end() - 1);

		Specimen newSpecimen;
		newSpecimen.adaptationValue = adaptationAssessment(path);
		newSpecimen.chromosomes = path;

		// Dodanie osobnika do populacji
		population.push_back(newSpecimen);
	}
}

// Oszacowanie przystosowania osobników
int GeneticAlgorithm::adaptationAssessment(vector<int> chromosomes)
{
	int cost = 0;

	for (int i = 0; i < chromosomes.size() - 1; i++)
		cost += data.matrix[chromosomes[i]][chromosomes[i + 1]];

	return cost;
}

// Wybór najlepszych osobników jako potencjalnych rodziców na podstawie wartoœæ przystosowania
void GeneticAlgorithm::selection()
{
	// wyczyszczenie wektora rodziców
	parents.clear();
	parents.reserve(parentsSize);

	for (int i = 0; i < parentsSize; i++) {
		vector<Specimen> tournament;
		// Wybór 10% populacji
		for (int j = 0; j < populationSize * 0.1; j++) {
			int random = rand() % populationSize;
			Specimen randomSpecimen = population[random];
			tournament.push_back(randomSpecimen);
		}
		// szukanie najlepszego osobnika z wylosowanej populacji i dodanie go do wektora rodziców
		sort(tournament.begin(), tournament.end(), sortByAdaptation);
		parents.push_back(tournament[0]);
	}
}

// Wybór rodziców, sprawdzenie prawdopodobieñstwa krzy¿owania, krzy¿owanie OX - Order Crossover
void GeneticAlgorithm::crossing()
{
	// wybór dwóch losowych rodziców
	int i = rand() % parentsSize;
	vector<int> firstParent = parents[i].chromosomes;

	int j = rand() % parentsSize;
	while (j == i)
		j = rand() % parentsSize;
	vector<int> secondParent = parents[j].chromosomes;

	if (generateProbability() < data.crossingRate) {

		// Punkty wyznaczaj¹ce przedzia³ krzy¿owania
		int firstCrossPoint = 0, secondCrossPoint = 0;

		firstCrossPoint = rand() % (data.size - 1) + 1;

		do {
			secondCrossPoint = rand() % (data.size - 1) + 1;
		} while (secondCrossPoint == firstCrossPoint);

		if (secondCrossPoint < firstCrossPoint) {
			int buf = firstCrossPoint;
			firstCrossPoint = secondCrossPoint;
			secondCrossPoint = buf;
		}

		// Dodanie punktów startowych
		firstChildChromosomes.push_back(0);
		secondChildChromosomes.push_back(0);

		// Skopiowanie przedzia³u od rodziców
		for (int i = firstCrossPoint; i <= secondCrossPoint; i++)
		{
			firstChildChromosomes.push_back(firstParent[i]);
			secondChildChromosomes.push_back(secondParent[i]);
		}

		int genIndex = 0; 
		int firstParentGen = 0; 
		int secondParentGen = 0;

		for (int i = 0; i < data.size; i++) {

			genIndex = (secondCrossPoint + i) % (data.size);
			firstParentGen = firstParent[genIndex];
			secondParentGen = secondParent[genIndex];

			bool isInFirst = false;

			for (int j = 0; j < firstChildChromosomes.size(); j++)
			{
				if (firstChildChromosomes[j] == secondParentGen)
				{
					isInFirst = true;
				}
			}
			if (!isInFirst)
			{
				firstChildChromosomes.push_back(secondParentGen);
			}

			bool isInSecond = false;

			for (int j = 0; j < secondChildChromosomes.size(); j++)
			{
				if (secondChildChromosomes[j] == firstParentGen)
				{
					isInSecond = true;
				}
			}
			if (!isInSecond)
			{
				secondChildChromosomes.push_back(firstParentGen);
			}
		}
		firstChildChromosomes.push_back(0);
		secondChildChromosomes.push_back(0);

		rotate(firstChildChromosomes.begin() + 1, firstChildChromosomes.end() - 1 - (firstCrossPoint), firstChildChromosomes.end() - 1);
		rotate(secondChildChromosomes.begin() + 1, secondChildChromosomes.end() - 1 - (firstCrossPoint), secondChildChromosomes.end()- 1);
	}
	else {
		firstChildChromosomes = firstParent;
		secondChildChromosomes = secondParent;
	}
}

// Mutowanie miast w œcie¿ce
void GeneticAlgorithm::mutating()
{
	// Mutacja potomków
	if (firstMutation)
	{
		// Metoda TRANSPOSITION - zamienia dwa losowo wybrane miasta
		if (generateProbability() < data.mutationRate)
		{
			// losowanie punktów które zostan¹ zamienione miejscami
			int x1 = 0; int x2 = 0;

			x1 = (rand() % (data.size - 2)) + 1;

			do
			{
				x2 = (rand() % (data.size - 2)) + 1;
			} while (x1 == x2);

			// zamiana punktów miejscami
			int tmp = firstChildChromosomes[x1];
			firstChildChromosomes[x1] = firstChildChromosomes[x2];
			firstChildChromosomes[x2] = tmp;
		}

		if (generateProbability() < data.mutationRate)
		{
			// losowanie punktów które zostan¹ zamienione miejscami
			int x1 = 0; int x2 = 0;

			x1 = (rand() % (data.size - 2)) + 1;

			do
			{
				x2 = (rand() % (data.size - 2)) + 1;
			} while (x1 == x2);

			int tmp = secondChildChromosomes[x1];
			secondChildChromosomes[x1] = secondChildChromosomes[x2];
			secondChildChromosomes[x2] = tmp;
		}
	}
	else
	{
		// Metoda INSERTION - przestawia miasto na losowo wybran¹ pozycjê, rozsuwaj¹c pozosta³e miasta
		if (generateProbability() < data.mutationRate)
		{
			int x1 = 0; int x2 = 0;
			do
			{
				x1 = (rand() % (data.size - 2)) + 1;
				x2 = (rand() % (data.size - 2)) + 1;
			} while (x1 == x2 - 1 || x1 == x2 || x1 == x2 + 1);

			firstChildChromosomes.insert(firstChildChromosomes.begin() + x2, firstChildChromosomes[x1]);

			if (x2 > x1)
			{
				firstChildChromosomes.erase(firstChildChromosomes.begin() + x1);
			}
			else
			{
				firstChildChromosomes.erase(firstChildChromosomes.begin() + x1 + 1);
			}
		}
		if (generateProbability() < data.mutationRate)
		{
			int x1 = 0; int x2 = 0;

			do
			{
				x1 = (rand() % (data.size - 2)) + 1;
				x2 = (rand() % (data.size - 2)) + 1;
			} while (x1 == x2 - 1 || x1 == x2 || x1 == x2 + 1);

			secondChildChromosomes.insert(secondChildChromosomes.begin() + x2, secondChildChromosomes[x1]);

			if (x2 > x1)
			{
				secondChildChromosomes.erase(secondChildChromosomes.begin() + x1);
			}
			else
			{
				secondChildChromosomes.erase(secondChildChromosomes.begin() + x1 + 1);
			}
		}
	}
}

// generowanie liczb z przedzia³u od 0.0 do 1.0
float GeneticAlgorithm::generateProbability()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// Generowanie nowej populacji
void GeneticAlgorithm::generateNewPopulation()
{
	sort(population.begin(), population.end(), sortByAdaptation);

	for (int i = 0; i < eliteSize; i++)
	{
		temporaryPopulation.push_back(population[i]);
	}
}

