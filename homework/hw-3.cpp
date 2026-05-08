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

void printGraph() {
    for (int i = 0; i < N; i++) {
        cout << i << ": ";
        for (int j = 0; j < Gr[i].size(); j++) {
            cout << Gr[i][j] << " ";
        }
        cout << endl;
    }
}

// функция подсчёта степеней всех вершин
void printDegrees() {
    for (int i = 0; i < N; i++) {
        cout << "степень вершины " << i << " = " << Gr[i].size() << endl;
    }
}

int main() {
    int M;
    cout << "введите количество вершин и рёбер: ";
    cin >> N >> M;

    buildGraph(M);

    cout << "\nсписок смежности:" << endl;
    printGraph();

    cout << "\nстепени вершин:" << endl;
    printDegrees();

}