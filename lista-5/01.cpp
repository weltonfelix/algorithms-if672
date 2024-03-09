#include <iostream>

#define WHITE 'W'
#define GRAY 'G'
#define BLACK 'B'

class Vertex
{
public:
  int index;
  char color = WHITE;
  int d = -1;
  int f = -1;
  Vertex *parent;

  Vertex(int index)
  {
    this->index = index;
    this->parent = this;
  }
};

class Edge
{
public:
  Vertex *vertex1;
  Vertex *vertex2;

  Edge(Vertex *vertex1, Vertex *vertex2)
  {
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;
  }
};

template <typename T>
class ListObject
{
public:
  ListObject *next;
  T *element;

  ListObject(T *element, ListObject *next)
  {
    this->element = element;
    this->next = next;
  }
};

template <typename T>
class List
{
public:
  ListObject<T> *head;

  ~List()
  {
    ListObject<T> *current = this->head;
    while (current != NULL)
    {
      ListObject<T> *next = current->next;
      delete current;
      current = next;
    }
  }

  void insert(T *element)
  {
    this->head = new ListObject<T>(element, this->head);
  }
};

class Graph
{
private:
  int vertexCount;
  int time;
  List<Edge> *edges;

  void dfsVisit(Vertex *vertex)
  {
    this->time++;
    vertex->d = this->time;
    vertex->color = GRAY;

    ListObject<Edge> *currentListObject = this->adj[vertex->index]->head;
    while (currentListObject != NULL)
    {
      Edge *edge = currentListObject->element;
      Vertex *adjacentVertex = edge->vertex1->index == vertex->index ? edge->vertex2 : edge->vertex1;

      if (adjacentVertex->color == WHITE)
      {
        adjacentVertex->parent = vertex;
        this->dfsVisit(adjacentVertex);
      }

      currentListObject = currentListObject->next;
    }

    vertex->f = ++time;
    vertex->color = BLACK;
  }

public:
  Vertex **vertices;
  List<Edge> **adj;
  int *treeSizes;

  Graph(int vertexCount)
  {
    this->vertexCount = vertexCount;
    this->adj = new List<Edge> *[vertexCount];
    this->vertices = new Vertex *[vertexCount];
    this->edges = new List<Edge>();
    this->treeSizes = new int[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
      this->adj[i] = new List<Edge>();
      this->vertices[i] = new Vertex(i);
      this->treeSizes[i] = 0;
    }
  }

  ~Graph()
  {
    for (int i = 0; i < vertexCount; i++)
    {
      delete adj[i];
    }

    for (int i = 0; i < vertexCount; i++)
    {
      delete vertices[i];
    }

    ListObject<Edge> *currentListObject = this->edges->head;
    while (currentListObject != NULL)
    {
      ListObject<Edge> *next = currentListObject->next;
      delete currentListObject->element;
      currentListObject = next;
    }

    delete edges;

    delete[] adj;
    delete[] vertices;
    delete[] treeSizes;
  }

  void addEdge(int vertex1Index, int vertex2Index)
  {
    Vertex *vertex1 = this->vertices[vertex1Index];
    Vertex *vertex2 = this->vertices[vertex2Index];

    Edge *edge = new Edge(vertex1, vertex2);
    this->edges->insert(edge);
    this->adj[vertex1Index]->insert(edge);
    this->adj[vertex2Index]->insert(edge);
  }

  void dfs()
  {
    for (int i = 0; i < this->vertexCount; i++)
    {
      this->vertices[i]->color = WHITE;
      this->vertices[i]->parent = this->vertices[i];
    }

    this->time = 0;

    for (int i = 0; i < this->vertexCount; i++)
    {
      if (this->vertices[i]->color == WHITE)
      {
        this->dfsVisit(this->vertices[i]);
      }
    }
  }

  Vertex *findSet(int vertexIndex)
  {
    Vertex *vertex = this->vertices[vertexIndex];

    if (vertex != vertex->parent)
    {
      vertex->parent = findSet(vertex->parent->index);
    }

    return vertex->parent;
  }

  void calculateConnectedComponentsSize()
  {
    for (int i = 0; i < this->vertexCount; i++)
    {
      this->treeSizes[this->findSet(i)->index]++;
    }
  }

  int getTreeSize(int vertexIndex)
  {
    return this->treeSizes[findSet(vertexIndex)->index];
  }
};

int main()
{
  int usersCount, connectionsCount;
  std::cin >> usersCount >> connectionsCount;

  Graph graph(usersCount);

  for (int i = 0; i < connectionsCount; i++)
  {
    int u, v;
    std::cin >> u >> v;
    graph.addEdge(u - 1, v - 1);
  }

  graph.dfs();
  graph.calculateConnectedComponentsSize();

  for (int i = 0; i < usersCount; i++)
  {
    std::cout << graph.getTreeSize(i);
    if (i < usersCount - 1)
    {
      std::cout << " ";
    }
  }
  std::cout << std::endl;

  return 0;
}
