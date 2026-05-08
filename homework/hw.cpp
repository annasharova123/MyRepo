#include <iostream>
#include <vector>
using namespace std;

int N; // количество вершин
vector<vector<int>> Gr; // список смежности

// заполняем граф рёбрами
void buildGraph(int M) {
    Gr.resize(N); // создаём  пустые списки
    for (int i = 0; i < M; i++) {
        int x, y;
        cin >> x >> y; // вводим ребро x-y
        // граф неориентированный поэтому добавляем в обе стороны
        Gr[x].push_back(y);
        Gr[y].push_back(x);
    }
}

int main() {
    int M; // количество рёбер
    cout << "введите количество вершин и рёбер: ";
    cin >> N >> M;

    buildGraph(M);

    int v;
    cout << "введите вершину: ";
    cin >> v;

    // количество смежных вершин это размер списка этой вершины
    cout << "количество смежных вершин: " << Gr[v].size() << endl;

    // выведем и сами смежные вершины
    cout << "смежные вершины: ";
    for (int i = 0; i < Gr[v].size(); i++) {
        cout << Gr[v][i] << " ";
    }
    cout << endl;
}