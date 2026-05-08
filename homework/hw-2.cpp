#include <iostream>
#include <vector>
using namespace std;

int N;
vector<vector<int>> Gr;

void buildGraph(int M) {
    Gr.resize(N);
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y;
        Gr[x].push_back(y);
        Gr[y].push_back(x);
    }
}

// функция которая выводит весь граф на экран
void printGraph() {
    for (int i = 0; i < N; i++) {
        cout << i << ": ";
        for (int j = 0; j < Gr[i].size(); j++) {
            cout << Gr[i][j] << " ";
        }
        cout << endl;
    }
}

// вставка ребра
void insertEdge(int a, int b) {
    // проверяем что такого ребра ещё нет
    for (int i = 0; i < Gr[a].size(); i++) {
        if (Gr[a][i] == b) {
            cout << "такое ребро уже существует!" << endl;
            return;
        }
    }
    // добавляем ребро в обе стороны
    Gr[a].push_back(b);
    Gr[b].push_back(a);
}

int main() {
    int M;
    cout << "введите количество вершин и рёбер: ";
    cin >> N >> M;

    buildGraph(M);

    cout << "\nграф ДО вставки:" << endl;
    printGraph();

    int a, b;
    cout << "\nвведите вершины A и B для нового ребра: ";
    cin >> a >> b;

    insertEdge(a, b);

    cout << "\nграф ПОСЛЕ вставки:" << endl;
    printGraph();

}