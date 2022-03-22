#include "BranchAndBound.h"

BranchAndBound::BranchAndBound()
{
}

BranchAndBound::~BranchAndBound()
{
}

Path BranchAndBound::runMethod(DataMatrix data, vector<int> nodes)
{
    // Odliczanie czasu od wywo³ania funkcji
    timer.startTimer();

    // Przypisanie iloœci wêz³ów
    numberOfNodes = nodes.size();

    // Przypisanie tablicy kosztów przejœæ z miasta do miasta
    costsMatrix = data.matrix;

    // Ustawienie wartoœci na przek¹tnej jako INT_MAX
    for (int i = 0; i < numberOfNodes; i++) {
        costsMatrix[i][i] = INT_MAX;
    }

    // Utworzenie pustego wektora œcie¿ki dla korzenia
    vector<pair<int, int>> v;

    // Uworzenie korzenia drzewa i wyliczenie jego kosztu
    Node* root = addNode(costsMatrix, v, 0, -1, 0);

    // Wyliczenie dolnej granicy œcie¿ki zaczynaj¹cej siê w korzeniu
    root->cost = calculateReduction(root->reducedMatrix);

    // Utworzenie kolejki priorytetowej do przechowywania wêz³ów obiecuj¹cych
    priority_queue<Node*, vector<Node*>, comparator> pq;

    // Dodanie korzenia do listy wêz³ów obiecuj¹cych
    pq.push(root);

    // Znalezienie wêz³a obiecuj¹cego, który ma najni¿szy koszt, dodanie jego dzieci do listy
    while (!pq.empty())
    {
        // Znalezienie wêz³a o najmniejszym koszcie
        Node* minimalNode = pq.top();

        // Usuwanie wêz³a z listy
        pq.pop();

        // Czy wszytskie miasta zosta³y znalezione
        if (minimalNode->level == numberOfNodes - 1)
        {
            // Zamkniêcie cyklu Hamiltona, powrót do wierzcho³ka startowego
            minimalNode->path.push_back(make_pair(minimalNode->vertex, 0));

            // Stworzenie wektora ze œcie¿k¹, która jest rozwi¹zaniem
            vector<int> solutionPath;
            for (int i = 0; i < minimalNode->path.size(); i++) {
                solutionPath.push_back(minimalNode->path[i].first);
            }
            solutionPath.push_back(0);

            // Stworzenie rozwi¹zania (drogi z kosztem)
            Path resultPath = Path(solutionPath, minimalNode->cost);

            // Uwolnienie pamiêci
            delete minimalNode;
            return resultPath;
        }

        // Stworzenie potomków minimalnego wêz³a i dodanie ich do drzewa
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (minimalNode->reducedMatrix[minimalNode->vertex][i] != INT_MAX)
            {
                // Stworzenie dziecka wêz³a, obliczenie jego kosztu i dodanie do kolejki wêz³ów obiecuj¹cych
                Node* child = addNode(minimalNode->reducedMatrix, minimalNode->path, minimalNode->level + 1, minimalNode->vertex, i);

                // Koszt = koszt ojca + koszt przejœci z rodzica do dziecka + redukcja
                child->cost = minimalNode->cost + minimalNode->reducedMatrix[minimalNode->vertex][i]
                    + calculateReduction(child->reducedMatrix);

                pq.push(child);
            }

            // Sprawdzenie czy algorytm nie wykonuje siê zbyt d³ugo
            timer.stopTimer();
            if (timer.timeCounter() > 120000)
            {
                vector<int> nullVector;
                nullVector.push_back(0);
                Path nullPath = Path(nullVector, 0);
                return nullPath;
            }
        }
        // Usuniêcie wêz³a, wszystkie obiecuj¹ce wêz³y maj¹ wystarczaj¹co informacji
        // i przechowywane s¹ w kolejce
        delete minimalNode;
    }
}

// Przydzielenie miasta jako nowego wêz³a drzewa 
Node* BranchAndBound::addNode(int** parentMatrix, vector<pair<int, int>> const& path, int level, int parent, int actualNode)
{
    Node* node = new Node;

    // Przechowuje krawêdzie przodków drzewa przestrzeni stanów
    node->path = path;

    // Pomiñ wêze³ g³ówny
    if (level != 0)
    {
        // Dodanie biê¿¹cej krawêdzi do œcie¿ki
        node->path.push_back(make_pair(parent, actualNode));
    }

    // Tworzenie dynamicznej tablicy
    // Tablica na wskazniki
    node->reducedMatrix = new int* [numberOfNodes];
    // Generowanie poszczegolnych wymiarów
    for (int i = 0; i < numberOfNodes; i++)
        node->reducedMatrix[i] = new int[numberOfNodes];

    // Skopiuj dane o kosztach z wêz³a ojca do wêz³a bie¿¹cego
    copyTable(parentMatrix, node->reducedMatrix);

    // Zmieñ wszytkie wpisy wiersza rodzica i kolumny aktulanego wierzcho³ka 
    // na nieskoñczonoœæ i pomiñ wêze³ g³ówny
    for (int k = 0; level != 0 && k < numberOfNodes; k++)
    {
        // Ustawienie krawêdzi wychodz¹cych dla rodzica
        node->reducedMatrix[parent][k] = INT_MAX;

        // Ustawienie przychodz¹cych krawêdzi dla aktulanego wêz³a
        node->reducedMatrix[k][actualNode] = INT_MAX;
    }

    // Ustawienie krawêdzi powrotu do wierzcho³ka startowego jako INT_MAX
    node->reducedMatrix[actualNode][0] = INT_MAX;

    // Ustawiæ liczbê odwiedzonych dotychczas miast
    node->level = level;

    // Przypisanie aktualnego numeru miasta
    node->vertex = actualNode;

    return node;
}


// Redukowanie wiersza tak ¿eby w ka¿dym wierszu by³o przynajmniej jedno zero lub same nieskoñczonoœci
void BranchAndBound::rowReduction(int** reducedMatrix, int* row)
{
    // Wyszukanie najmnieszjszych wartoœci z danych wierszy
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] < row[i]) 
                row[i] = reducedMatrix[i][j];
        }
    }
    // Zmniejszenie ka¿dego wiersza o minmaln¹ wartoœæ
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

// Redukowanie kolumny tak ¿eby w ka¿dej kolumnie by³o przynajmniej jedno zero lub same nieskoñczonoœci
void BranchAndBound::columnReduction(int** reducedMatrix, int* column)
{
    // Wyszukanie najmnieszjszych wartoœci z danych kolumn
    for (int i = 0; i < numberOfNodes; i++)
    {
        for (int j = 0; j < numberOfNodes; j++)
        {
            if (reducedMatrix[i][j] < column[j]) {
                column[j] = reducedMatrix[i][j];
            }
        }
    }
    // Zmniejszenie ka¿dej kolumny o minmaln¹ wartoœæ
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

// Kopiowanie zawartoœci jednej tablicy do drugiej
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

    // Inicjalizowanie wszystkich wartoœci tablic jako INT_MAX
    fillWithInfinities(row);
    fillWithInfinities(column);

    // Redukowanie macierzy kosztów
    rowReduction(reducedMatrix, row);
    columnReduction(reducedMatrix, column);

    // Wartoœæ redukcji jest sum¹ wszystkich redukcji
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