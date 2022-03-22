#include "BranchAndBound.h"

BranchAndBound::BranchAndBound()
{
}

BranchAndBound::~BranchAndBound()
{
}

Path BranchAndBound::runMethod(DataMatrix data, vector<int> nodes)
{
    // Odliczanie czasu od wywo�ania funkcji
    timer.startTimer();

    // Przypisanie ilo�ci w�z��w
    numberOfNodes = nodes.size();

    // Przypisanie tablicy koszt�w przej�� z miasta do miasta
    costsMatrix = data.matrix;

    // Ustawienie warto�ci na przek�tnej jako INT_MAX
    for (int i = 0; i < numberOfNodes; i++) {
        costsMatrix[i][i] = INT_MAX;
    }

    // Utworzenie pustego wektora �cie�ki dla korzenia
    vector<pair<int, int>> v;

    // Uworzenie korzenia drzewa i wyliczenie jego kosztu
    Node* root = addNode(costsMatrix, v, 0, -1, 0);

    // Wyliczenie dolnej granicy �cie�ki zaczynaj�cej si� w korzeniu
    root->cost = calculateReduction(root->reducedMatrix);

    // Utworzenie kolejki priorytetowej do przechowywania w�z��w obiecuj�cych
    priority_queue<Node*, vector<Node*>, comparator> pq;

    // Dodanie korzenia do listy w�z��w obiecuj�cych
    pq.push(root);

    // Znalezienie w�z�a obiecuj�cego, kt�ry ma najni�szy koszt, dodanie jego dzieci do listy
    while (!pq.empty())
    {
        // Znalezienie w�z�a o najmniejszym koszcie
        Node* minimalNode = pq.top();

        // Usuwanie w�z�a z listy
        pq.pop();

        // Czy wszytskie miasta zosta�y znalezione
        if (minimalNode->level == numberOfNodes - 1)
        {
            // Zamkni�cie cyklu Hamiltona, powr�t do wierzcho�ka startowego
            minimalNode->path.push_back(make_pair(minimalNode->vertex, 0));

            // Stworzenie wektora ze �cie�k�, kt�ra jest rozwi�zaniem
            vector<int> solutionPath;
            for (int i = 0; i < minimalNode->path.size(); i++) {
                solutionPath.push_back(minimalNode->path[i].first);
            }
            solutionPath.push_back(0);

            // Stworzenie rozwi�zania (drogi z kosztem)
            Path resultPath = Path(solutionPath, minimalNode->cost);

            // Uwolnienie pami�ci
            delete minimalNode;
            return resultPath;
        }

        // Stworzenie potomk�w minimalnego w�z�a i dodanie ich do drzewa
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (minimalNode->reducedMatrix[minimalNode->vertex][i] != INT_MAX)
            {
                // Stworzenie dziecka w�z�a, obliczenie jego kosztu i dodanie do kolejki w�z��w obiecuj�cych
                Node* child = addNode(minimalNode->reducedMatrix, minimalNode->path, minimalNode->level + 1, minimalNode->vertex, i);

                // Koszt = koszt ojca + koszt przej�ci z rodzica do dziecka + redukcja
                child->cost = minimalNode->cost + minimalNode->reducedMatrix[minimalNode->vertex][i]
                    + calculateReduction(child->reducedMatrix);

                pq.push(child);
            }

            // Sprawdzenie czy algorytm nie wykonuje si� zbyt d�ugo
            timer.stopTimer();
            if (timer.timeCounter() > 120000)
            {
                vector<int> nullVector;
                nullVector.push_back(0);
                Path nullPath = Path(nullVector, 0);
                return nullPath;
            }
        }
        // Usuni�cie w�z�a, wszystkie obiecuj�ce w�z�y maj� wystarczaj�co informacji
        // i przechowywane s� w kolejce
        delete minimalNode;
    }
}

// Przydzielenie miasta jako nowego w�z�a drzewa 
Node* BranchAndBound::addNode(int** parentMatrix, vector<pair<int, int>> const& path, int level, int parent, int actualNode)
{
    Node* node = new Node;

    // Przechowuje kraw�dzie przodk�w drzewa przestrzeni stan�w
    node->path = path;

    // Pomi� w�ze� g��wny
    if (level != 0)
    {
        // Dodanie bi꿹cej kraw�dzi do �cie�ki
        node->path.push_back(make_pair(parent, actualNode));
    }

    // Tworzenie dynamicznej tablicy
    // Tablica na wskazniki
    node->reducedMatrix = new int* [numberOfNodes];
    // Generowanie poszczegolnych wymiar�w
    for (int i = 0; i < numberOfNodes; i++)
        node->reducedMatrix[i] = new int[numberOfNodes];

    // Skopiuj dane o kosztach z w�z�a ojca do w�z�a bie��cego
    copyTable(parentMatrix, node->reducedMatrix);

    // Zmie� wszytkie wpisy wiersza rodzica i kolumny aktulanego wierzcho�ka 
    // na niesko�czono�� i pomi� w�ze� g��wny
    for (int k = 0; level != 0 && k < numberOfNodes; k++)
    {
        // Ustawienie kraw�dzi wychodz�cych dla rodzica
        node->reducedMatrix[parent][k] = INT_MAX;

        // Ustawienie przychodz�cych kraw�dzi dla aktulanego w�z�a
        node->reducedMatrix[k][actualNode] = INT_MAX;
    }

    // Ustawienie kraw�dzi powrotu do wierzcho�ka startowego jako INT_MAX
    node->reducedMatrix[actualNode][0] = INT_MAX;

    // Ustawi� liczb� odwiedzonych dotychczas miast
    node->level = level;

    // Przypisanie aktualnego numeru miasta
    node->vertex = actualNode;

    return node;
}


// Redukowanie wiersza tak �eby w ka�dym wierszu by�o przynajmniej jedno zero lub same niesko�czono�ci
void BranchAndBound::rowReduction(int** reducedMatrix, int* row)
{
    // Wyszukanie najmnieszjszych warto�ci z danych wierszy
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] < row[i]) 
                row[i] = reducedMatrix[i][j];
        }
    }
    // Zmniejszenie ka�dego wiersza o minmaln� warto��
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] != INT_MAX && row[i] != INT_MAX) {
                reducedMatrix[i][j] -= row[i];
            }
        }
    }
}

// Redukowanie kolumny tak �eby w ka�dej kolumnie by�o przynajmniej jedno zero lub same niesko�czono�ci
void BranchAndBound::columnReduction(int** reducedMatrix, int* column)
{
    // Wyszukanie najmnieszjszych warto�ci z danych kolumn
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] < column[j]) {
                column[j] = reducedMatrix[i][j];
            }
        }
    }
    // Zmniejszenie ka�dej kolumny o minmaln� warto��
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] != INT_MAX && column[j] != INT_MAX) {
                reducedMatrix[i][j] -= column[j];
            }
        }
    }
}

// Kopiowanie zawarto�ci jednej tablicy do drugiej
int** BranchAndBound::copyTable(int** parentTable, int** childTable)
{
    for (int i = 0; i < numberOfNodes; i++)
        for (int j = 0; j < numberOfNodes; j++)
            childTable[i][j] = parentTable[i][j];
    return childTable;
}

int BranchAndBound::calculateReduction(int** reducedMatrix)
{
    int cost = 0;

    int* row = new int[numberOfNodes];
    int* column = new int[numberOfNodes];

    // Inicjalizowanie wszystkich warto�ci tablic jako INT_MAX
    fillWithInfinities(row);
    fillWithInfinities(column);

    // Redukowanie macierzy koszt�w
    rowReduction(reducedMatrix, row);
    columnReduction(reducedMatrix, column);

    // Warto�� redukcji jest sum� wszystkich redukcji
    for (int i = 0; i < numberOfNodes; i++)
    {
        if (row[i] != INT_MAX)
            cost += row[i];
        if (column[i] != INT_MAX)
            cost += column[i];
    }
    delete[] column;
    delete[] row;
    return cost;
}

void BranchAndBound::fillWithInfinities(int* table)
{
    for (int i = 0; i < numberOfNodes; i++)
        table[i] = INT_MAX;
}