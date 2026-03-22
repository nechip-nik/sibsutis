#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cmath>

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
    
    pair<vector<int>, int> findMinPathBruteForce(int start, int end) {
        vector<int> vertices;
        for (int i = 0; i < ver; i++) {
            if (i != start && i != end) {
                vertices.push_back(i);
            }
        }
        
        int minPathWeight = numeric_limits<int>::max();
        vector<int> bestPath;
        
        do {
            vector<int> currentPath;
            currentPath.push_back(start);
            
            for (int v : vertices) {
                currentPath.push_back(v);
            }
            currentPath.push_back(end);
            
            int pathWeight = calculatePathWeight(currentPath);
            
            if (pathWeight != -1 && pathWeight < minPathWeight) {
                minPathWeight = pathWeight;
                bestPath = currentPath;
            }
            
        } while (next_permutation(vertices.begin(), vertices.end()));
        
        vector<int> directPath = {start, end};
        int directWeight = calculatePathWeight(directPath);
        if (directWeight != -1 && directWeight < minPathWeight) {
            minPathWeight = directWeight;
            bestPath = directPath;
        }
        
        return {bestPath, minPathWeight};
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
    
    cout << "Программа записи данных времени выполнения\n";
    cout << "==========================================\n\n";
    
    // Открываем файл для записи
    ofstream dataFile("time_data.txt");
    
    if (!dataFile.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }
    
    // Записываем заголовок
    dataFile << "# Экспоненциальный рост времени выполнения полного перебора\n";
    dataFile << "# Количество вершин | Время (секунды) | Количество перестановок\n";
    dataFile << "#------------------------------------------------------------\n";
    
    vector<int> sizes = {3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    cout << "Измерение времени для графов:\n";
    cout << "-----------------------------\n";
    
    for (int size : sizes) {
        cout << "Граф с " << size << " вершинами... ";
        cout.flush();
        
        Graph g(size);
        g.generateRandomGraph(50);
        
        int start = 0;
        int end = size - 1;
        
        long long permutations = (size > 2) ? factorial(size - 2) : 1;
        
        auto startTime = high_resolution_clock::now();
        auto result = g.findMinPathBruteForce(start, end);
        auto endTime = high_resolution_clock::now();
        
        duration<double> elapsed = endTime - startTime;
        
        // Запись в файл
        dataFile << size << " " 
                 << scientific << setprecision(8) << elapsed.count() << " "
                 << permutations << "\n";
        
        cout << "готово (" << fixed << setprecision(3) << elapsed.count() << " сек)\n";
        
        dataFile.flush();
    }
    
    dataFile.close();
    
    cout << "\n✅ Данные сохранены в файл: time_data.txt\n";
    cout << "\nФормат файла:\n";
    cout << "  Колонка 1: количество вершин\n";
    cout << "  Колонка 2: время выполнения (секунды)\n";
    cout << "  Колонка 3: количество перестановок\n";
    
    return 0;
}