#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

typedef struct {
    int u, v, weight;
} Edge;

typedef struct {
    int vertices;
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES];
} Graph;

/* Comparator for qsort – sort edges by weight ascending */
int compareEdges(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;
    return ea->weight - eb->weight;
}

int findRoot(int *parent, int v) {
    if (parent[v] != v)
        parent[v] = findRoot(parent, parent[v]);
    return parent[v];
}

/* Union two sets */
void unionSets(int *parent, int a, int b) {
    int rootA = findRoot(parent, a);
    int rootB = findRoot(parent, b);
    if (rootA != rootB)
        parent[rootB] = rootA;
}

int kruskal(Graph *g, Edge *spanningTree) {
    int vertices = g->vertices;
    Edge edges[MAX_VERTICES * MAX_VERTICES];
    int edgeCount = 0;

    /* Collect all edges (upper triangle of symmetric matrix) */
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (g->adjacencyMatrix[i][j] != 0) {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                edges[edgeCount].weight = g->adjacencyMatrix[i][j];
                edgeCount++;
            }
        }
    }

    /* Sort edges by weight */
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);

    printf("вершины:\n");
    for (int i = 0; i < edgeCount; i++) {
        printf("%d -- %d: %d\n", edges[i].u, edges[i].v, edges[i].weight);
    }
    printf("\n");

    /* Union-Find */
    int parent[MAX_VERTICES];
    for (int i = 0; i < vertices; i++)
        parent[i] = i;

    int treeCount = 0;
    printf("\n");

    for (int i = 0; i < edgeCount; i++) {
        int a = edges[i].u;
        int b = edges[i].v;
        int weight = edges[i].weight;

        int rootA = findRoot(parent, a);
        int rootB = findRoot(parent, b);

        if (rootA != rootB) {
            spanningTree[treeCount++] = edges[i];
            printf("%d -- %d: %d - подходит\n", a, b, weight);
            unionSets(parent, a, b);

            if (treeCount == vertices - 1)
                break;
        } else {
            printf("%d -- %d: %d - не подходит\n", a, b, weight);
        }
    }

    return treeCount;
}

int main(void) {
    Graph graph;
    graph.vertices = 8;

    int matrix[8][8] = {
        {  0,  9,  0, 20,  0,  0,  0,  0 },
        {  9,  0,  7,  0,  0,  0,  0,  0 },
        {  0,  7,  0,  8,  3,  0,  2, 12 },
        { 20,  0,  8,  0,  0,  0,  0,  0 },
        {  0,  0,  3,  0,  0,  6,  5,  0 },
        {  0,  0,  0,  0,  6,  0,  4,  0 },
        {  0,  0,  2,  0,  5,  4,  0, 22 },
        {  0,  0, 12,  0,  0,  0, 22,  0 }
    };

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            graph.adjacencyMatrix[i][j] = matrix[i][j];

    Edge spanningTree[MAX_VERTICES];
    int treeCount = kruskal(&graph, spanningTree);

    int totalWeight = 0;
    printf("ребра минимального остовного дерева:\n");
    for (int i = 0; i < treeCount; i++) {
        printf("%d -- %d: %d\n", spanningTree[i].u, spanningTree[i].v, spanningTree[i].weight);
        totalWeight += spanningTree[i].weight;
    }
    printf("вес: %d\n", totalWeight);

    return 0;
}