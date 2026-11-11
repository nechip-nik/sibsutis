#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX
#define MAX_V 1000

// Поиск вершины с минимальным расстоянием среди непосещённых
int minDistance(int dist[], bool visited[], int V) {
    int min = INF, min_index = -1;
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Вывод результата
void printSolution(int dist[], int V) {
    printf("Вершина \t Расстояние от источника\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF)
            printf("%d \t\t недостижима\n", i);
        else
            printf("%d \t\t %d\n", i, dist[i]);
    }
}

// Алгоритм Дейкстры (матрица смежности)
void dijkstra(int graph[MAX_V][MAX_V], int src, int V) {
    int dist[MAX_V];      // кратчайшие расстояния
    bool visited[MAX_V];  // посещённые вершины

    // Инициализация
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[src] = 0;

    // Основной цикл
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited, V);
        if (u == -1) break; // оставшиеся вершины недостижимы
        visited[u] = true;

        // Обновление расстояний соседей
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printSolution(dist, V);
}

int main() {
    int V = 9;
    int graph[MAX_V][MAX_V] = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
    };

    dijkstra(graph, 0, V);
    return 0;
}
дейкстра на куче 

форд-беллман(взешаный граф) от основный вершины 

заливка bfs обход в ширину релаксация 