#include "SW.h"

// generowanie liczb z przedziału od 0.0 do 1.0
float random()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Path SW::runAlgorithm(DataMatrix data, int eraFactor, int initialTempIterator, int minTempExponent)
{
    srand(time(NULL));

    Timer timer;
    if(!data.coolingTemp) data.coolingTemp = 0.99;
    if (!data.stopTime) data.stopTime = 300;
    float Tmin = 1/pow(10, minTempExponent);
    int era = data.size * eraFactor; //Epoka = N * a
    int bestCost = 0;
    float time = 0;
    vector<int>  bestPermutation;
    bestPermutation.resize(data.size + 1);
    bestPermutation = generateInitialSolution(data);
    bestCost = calculatePathCost(bestPermutation, data.matrix);
    vector<int> currentPermutation;
    currentPermutation.resize(data.size + 1);

    // Obliczenie temperatury początkowej
    int currentTemp = calculateInitialTemperature(data, bestPermutation, initialTempIterator);

    timer.startTimer();
    do {
        timer.stopTimer();
        int counter = 0;

        while ((timer.timeCounter() < data.stopTime) && (counter < era))
        {
            // wykonanie ruchu swap na losowych wierzchołkach
            currentPermutation = bestPermutation;
            int i = (rand() % (data.size - 1)) + 1;
            int j = (rand() % (data.size - 1)) + 1;
            if (i == j) continue;

            int tmp = currentPermutation[j];
            currentPermutation[j] = currentPermutation[i];
            currentPermutation[i] = tmp;

            int cost = calculatePathCost(currentPermutation, data.matrix);

            // Przyjęcie rozwiązania jako najlepsze
            if (cost < bestCost)
            {
                timer.stopTimer();
                bestCost = cost;
                bestPermutation = currentPermutation;
                time = timer.timeCounter();
            }
            // Przyjęcie gorszego rozwiązania z pewnym prawdopodobieństwem
            else if (random() < exp(-(static_cast<float>((cost - bestCost) / currentTemp))))
            {
                timer.stopTimer();
                bestCost = cost;
                bestPermutation = currentPermutation;
                time = timer.timeCounter();
            }
            counter++;
        }
        currentTemp = currentTemp * data.coolingTemp;
        timer.stopTimer();
        // Warunek kończący algorytm
        // czas wykonywania < limit czasu && temperatura > temperatura minimalna
    } while (timer.timeCounter() < data.stopTime && currentTemp > Tmin);
    Path bestSolution = Path(bestPermutation, bestCost, time);
    return bestSolution;
}

// Obliczenie temperatury początkowej
int SW::calculateInitialTemperature(DataMatrix data, vector<int> bestPermutation, int initialTempIterator)
{
    auto tempStart = bestPermutation;
    
    int initialTemp = INT32_MIN;
    for (int k = 0; k < data.size * initialTempIterator; k++) {

        int cost1 = calculatePathCost(tempStart, data.matrix);

        int i = (rand() % (data.size - 1)) + 1;
        int j = (rand() % (data.size - 1)) + 1;
        if (i == j) continue;

        int tmp = tempStart[j];
        tempStart[j] = tempStart[i];
        tempStart[i] = tmp;

        int cost2 = calculatePathCost(tempStart, data.matrix);

        int diff = abs(cost1 - cost2);

        // Ustalenie temperatury początkowej jako maksymalna różnica pomiędzy dowolnymi sąsiadami
        if (diff > initialTemp) initialTemp = diff;

        random_shuffle(tempStart.begin() + 1, tempStart.end() - 1);
    }
    return initialTemp;
}
// Generowanie początkowego rozwiązania za pomocą algorytmu zachłannego
vector<int> SW::generateInitialSolution(DataMatrix data)
{
    vector<int> currentSolution;
    vector<int> bestSolution;
    currentSolution.resize(data.size + 1);
    bestSolution.resize(data.size + 1);
    bool locallyOpitmal = false;

    // Przyjęcie rozwiązania początkowego
    for (int i = 0; i < data.size; i++) currentSolution[i] = i;
    currentSolution[data.size] = 0;
    // Przetasowanie rozwiązania
    random_shuffle(currentSolution.begin() + 1, currentSolution.end() - 1);
    int bestCost = calculatePathCost(currentSolution, data.matrix);
    bestSolution = currentSolution;
    do{
        locallyOpitmal = false;
        currentSolution = bestSolution;
        // Sprawdzanie sąsiedztwa bieżącego rozwiązania
        for (int i = 1; i < data.size -1; i++) {
            for (int j = i + 1; j < data.size; j++) {
                swap(currentSolution[i], currentSolution[j]);
                int currentCost = calculatePathCost(currentSolution, data.matrix);
                // Jeżeli rozwiązanie sąsiadujące ma lepsze rozwiązanie to zamiana
                if (currentCost < bestCost) {
                    locallyOpitmal = true;
                    bestCost = currentCost;
                    bestSolution = currentSolution;
                }
                swap(currentSolution[i], currentSolution[j]);
            }
        }
    } while (locallyOpitmal);
    return bestSolution;
}

// Obliczenie kosztu drogi
int SW::calculatePathCost(vector<int> permutation, int** costsMatrix)
{
    int cost = 0;

    for (int i = 0; i < permutation.size() - 1; i++)
    {
        cost += costsMatrix[permutation[i]][permutation[i + 1]];
    }
    return cost;
}
