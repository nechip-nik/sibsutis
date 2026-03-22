#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <iomanip>
#include <fstream>
#include <functional>

using namespace std;
using namespace chrono;

class Graph {
    int ver;
    vector<list<pair<int, int>>> smez;
    
    public:
    Graph(int ver) {
        this->ver = ver;
        smez.resize(ver);
    }
    
    void addEdge(int u, int v, int weight) {
        smez[u].push_back({v, weight});
        smez[v].push_back({u, weight});
    }
    
    void generateRandomGraph(int maxWeight = 50) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDist(1, maxWeight);
        uniform_real_distribution<> probDist(0.0, 1.0);
        
        double edgeProbability = 0.4;
        
        for (int i = 0; i < ver; i++) {
            for (int j = i + 1; j < ver; j++) {
                if (probDist(gen) < edgeProbability) {
                    int weight = weightDist(gen);
                    addEdge(i, j, weight);
                }
            }
        }
        
        ensureConnectivity();
    }
    
    void ensureConnectivity() {
        vector<bool> visited(ver, false);
        dfs(0, visited);
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDist(1, 50);
        
        for (int i = 0; i < ver; i++) {
            if (!visited[i]) {
                if (i > 0) {
                    uniform_int_distribution<> targetDist(0, i-1);
                    int target = targetDist(gen);
                    int weight = weightDist(gen);
                    addEdge(i, target, weight);
                } else {
                    addEdge(0, 1, weightDist(gen));
                }
                
                fill(visited.begin(), visited.end(), false);
                dfs(0, visited);
            }
        }
    }
    
    void dfs(int v, vector<bool>& visited) {
        visited[v] = true;
        for (auto& edge : smez[v]) {
            if (!visited[edge.first]) {
                dfs(edge.first, visited);
            }
        }
    }
    
    // Поиск в глубину (DFS) с возвратом
    pair<vector<int>, int> findMinPathDFS(int start, int end) {
        vector<int> bestPath;
        int minWeight = numeric_limits<int>::max();
        vector<bool> visited(ver, false);
        vector<int> currentPath;
        
        // Рекурсивная функция DFS
        function<void(int, int)> dfs = [&](int v, int currentWeight) {
            if (v == end) {
                if (currentWeight < minWeight) {
                    minWeight = currentWeight;
                    bestPath = currentPath;
                }
                return;
            }
            
            for (auto& edge : smez[v]) {
                int u = edge.first;
                int w = edge.second;
                
                if (!visited[u]) {
                    visited[u] = true;
                    currentPath.push_back(u);
                    dfs(u, currentWeight + w);
                    currentPath.pop_back();
                    visited[u] = false;
                }
            }
        };
        
        visited[start] = true;
        currentPath.push_back(start);
        dfs(start, 0);
        
        if (minWeight == numeric_limits<int>::max()) {
            return {{}, -1};
        }
        
        return {bestPath, minWeight};
    }
    
    int calculatePathWeight(const vector<int>& path) {
        int totalWeight = 0;
        
        for (size_t i = 0; i < path.size() - 1; i++) {
            int u = path[i];
            int v = path[i + 1];
            
            bool edgeFound = false;
            for (auto& edge : smez[u]) {
                if (edge.first == v) {
                    totalWeight += edge.second;
                    edgeFound = true;
                    break;
                }
            }
            
            if (!edgeFound) {
                return -1;
            }
        }
        
        return totalWeight;
    }
};

long long factorial(int n) {
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "Программа измерения времени выполнения DFS\n";
    cout << "===========================================\n\n";
    
    // Открываем файл для записи
    ofstream dataFile("dfs_time_data.txt");
    
    if (!dataFile.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }
    
    // Записываем заголовок
    dataFile << "# Экспоненциальный рост времени выполнения DFS\n";
    dataFile << "# Количество вершин | Время (секунды) | Вес пути\n";
    dataFile << "#------------------------------------------------\n";
    
    vector<int> sizes = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    cout << "Измерение времени для графов:\n";
    cout << "-----------------------------\n";
    
    for (int size : sizes) {
        cout << "Граф с " << size << " вершинами... ";
        cout.flush();
        
        Graph g(size);
        g.generateRandomGraph(50);
        
        int start = 0;
        int end = size - 1;
        
        auto startTime = high_resolution_clock::now();
        auto result = g.findMinPathDFS(start, end);
        auto endTime = high_resolution_clock::now();
        
        duration<double> elapsed = endTime - startTime;
        
        // Запись в файл
        dataFile << size << " " 
                 << scientific << setprecision(8) << elapsed.count() << " "
                 << result.second << "\n";
        
        cout << "готово (" << fixed << setprecision(3) << elapsed.count() << " сек)\n";
        
        dataFile.flush();
    }
    
    dataFile.close();
    
    cout << "\n✅ Данные сохранены в файл: dfs_time_data.txt\n";
    cout << "\nФормат файла:\n";
    cout << "  Колонка 1: количество вершин\n";
    cout << "  Колонка 2: время выполнения (секунды)\n";
    cout << "  Колонка 3: вес найденного пути\n";
    
    return 0;
}