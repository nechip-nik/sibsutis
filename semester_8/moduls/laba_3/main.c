#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Структура для представления ребра дерева
typedef struct Edge
{
  int parent;
  int child;
} Edge;

// Структура для хранения координат вершины
typedef struct Vertex
{
  double x;
  double y;
} Vertex;

// Функция для генерации случайного дерева с ограничениями
int
generateRandomTreeWithConstraints (int n, int k, int maxDepth, Edge *edges,
                                   Vertex *vertices, int *depth)
{
  srand (time (NULL));

  int *degree = (int *)calloc (n, sizeof (int));
  if (depth == NULL || degree == NULL)
    {
      printf ("Ошибка выделения памяти.\n");
      free (degree);
      free (depth);
      return 0;
    }

  int *availableParents = (int *)malloc (n * sizeof (int));
  if (availableParents == NULL)
    {
      printf ("Ошибка выделения памяти.\n");
      free (degree);
      free (depth);
      return 0;
    }

  int numAvailable = 1;
  availableParents[0] = 0;
  depth[0] = 0;
  int numEdges = 0;

  double *probabilities = (double *)malloc (n * sizeof (double));
  if (probabilities == NULL)
    {
      printf ("Ошибка выделения памяти.\n");
      free (degree);
      free (depth);
      free (availableParents);
      return 0;
    }

  for (int i = 1; i < n; i++)
    {
      if (numAvailable == 0)
        {
          printf ("Невозможно построить дерево: нет доступных родителей.\n");
          free (degree);
          free (depth);
          free (availableParents);
          free (probabilities);
          return 0;
        }

      double totalProb = 0.0;
      for (int j = 0; j < numAvailable; j++)
        {
          int parent = availableParents[j];
          probabilities[j] = 1.0 / (degree[parent] + 1);
          totalProb += probabilities[j];
        }

      for (int j = 0; j < numAvailable; j++)
        {
          probabilities[j] /= totalProb;
        }

      double r = (double)rand () / RAND_MAX;
      double cumulativeProb = 0.0;
      int chosenParent = -1;
      for (int j = 0; j < numAvailable; j++)
        {
          cumulativeProb += probabilities[j];
          if (r <= cumulativeProb)
            {
              chosenParent = availableParents[j];
              break;
            }
        }
      if (chosenParent == -1)
        chosenParent = availableParents[numAvailable - 1];

      int newDepth = depth[chosenParent] + 1;
      if (newDepth > maxDepth)
        {
          availableParents[0] = availableParents[numAvailable - 1];
          numAvailable--;
          i--;
          continue;
        }

      edges[numEdges].parent = chosenParent;
      edges[numEdges].child = i;
      numEdges++;

      depth[i] = newDepth;

      degree[chosenParent]++;
      if (degree[chosenParent] == k)
        {
          for (int j = 0; j < numAvailable; j++)
            {
              if (availableParents[j] == chosenParent)
                {
                  availableParents[j] = availableParents[numAvailable - 1];
                  numAvailable--;
                  break;
                }
            }
        }

      availableParents[numAvailable] = i;
      numAvailable++;
    }

  free (degree);
  free (availableParents);
  free (probabilities);
  return 1;
}

// Функция для вычисления координат вершин
void
computeCoordinates (int n, Edge *edges, Vertex *vertices, int *depth)
{
  // Создаём список детей для каждой вершины
  int *numChildren = (int *)calloc (n, sizeof (int));
  int **children = (int **)malloc (n * sizeof (int *));
  for (int i = 0; i < n; i++)
    {
      children[i] = (int *)malloc (n * sizeof (int)); // Максимум n детей
      numChildren[i] = 0;
    }

  // Заполняем список детей
  for (int i = 0; i < n - 1; i++)
    {
      int parent = edges[i].parent;
      int child = edges[i].child;
      children[parent][numChildren[parent]++] = child;
    }

  // Устанавливаем координаты корня
  vertices[0].x = 0.0;
  vertices[0].y = 0.0;

  // Рекурсивно вычисляем координаты для каждой вершины
  double *levelWidth = (double *)calloc (depth[0] + 1, sizeof (double));
  for (int v = 0; v < n; v++)
    {
      int d = depth[v];
      double spacing = 1.0; // Базовый шаг для детей
      if (numChildren[v] > 0)
        {
          // Вычисляем координаты детей
          double parentX = vertices[v].x;
          double offset
              = -(numChildren[v] - 1) * spacing / 2.0; // Центрируем детей
          for (int i = 0; i < numChildren[v]; i++)
            {
              int child = children[v][i];
              vertices[child].x = parentX + offset + i * spacing;
              vertices[child].y = -depth[child];
            }
        }
    }

  // Освобождаем память
  for (int i = 0; i < n; i++)
    {
      free (children[i]);
    }
  free (children);
  free (numChildren);
  free (levelWidth);
}

// Функция для сохранения данных для GNUplot
void
saveForGnuplot (int n, Edge *edges, Vertex *vertices)
{
  FILE *verticesFile = fopen ("vertices.dat", "w");
  if (verticesFile == NULL)
    {
      printf ("Ошибка открытия файла vertices.dat\n");
      return;
    }
  for (int i = 0; i < n; i++)
    {
      fprintf (verticesFile, "%d %f %f\n", i, vertices[i].x, vertices[i].y);
    }
  fclose (verticesFile);

  FILE *edgesFile = fopen ("edges.dat", "w");
  if (edgesFile == NULL)
    {
      printf ("Ошибка открытия файла edges.dat\n");
      return;
    }
  for (int i = 0; i < n - 1; i++)
    {
      int parent = edges[i].parent;
      int child = edges[i].child;
      fprintf (edgesFile, "%f %f\n", vertices[parent].x, vertices[parent].y);
      fprintf (edgesFile, "%f %f\n", vertices[child].x, vertices[child].y);
      fprintf (edgesFile, "\n");
    }
  fclose (edgesFile);
}

int main ()
{
  int n, k, maxDepth;
  printf ("Введите количество вершин: ");
  scanf ("%d", &n);
  printf ("Введите максимальную степень вершины (k): ");
  scanf ("%d", &k);
  printf ("Введите максимальную глубину дерева: ");
  scanf ("%d", &maxDepth);

  if (n < 1 || k < 1 || maxDepth < 1)
    {
      printf ("Количество вершин, k и максимальная глубина должны быть больше "
              "0.\n");
      return 1;
    }

  Edge *edges = (Edge *)malloc ((n - 1) * sizeof (Edge));
  Vertex *vertices = (Vertex *)malloc (n * sizeof (Vertex));
  int *depth = (int *)calloc (n, sizeof (int));
  if (edges == NULL || vertices == NULL || depth == NULL)
    {
      printf ("Ошибка выделения памяти.\n");
      free (edges);
      free (vertices);
      free (depth);
      return 1;
    }

  if (generateRandomTreeWithConstraints (n, k, maxDepth, edges, vertices,
                                         depth))
    {
      // Вычисляем координаты
      computeCoordinates (n, edges, vertices, depth);
      printf ("Дерево сгенерировано. Сохраняем данные для GNUplot...\n");
      saveForGnuplot (n, edges, vertices);
      printf ("Данные сохранены в vertices.dat и edges.dat\n");
    }

  free (edges);
  free (vertices);
  free (depth);
  return 0;
}