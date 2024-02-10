#include <iostream>

#define WHITE 'W'
#define GRAY 'G'
#define EMPTY_ADJACENCY_LIST "Lista Vazia"

class ListObject
{
public:
  ListObject *next;
  int number;

  ListObject(int number, ListObject *next)
  {
    this->number = number;
    this->next = next;
  }
};

class Graph
{
private:
  int vertexCount;
  ListObject **adjacencyList;

  void dfs_visit(int vertex, char *colors)
  {
    colors[vertex] = GRAY; // Vertex visited. Paint it black
    std::cout << vertex << " ";

    ListObject *adjacentVertex = this->adjacencyList[vertex]; // Get the first adjacent vertex

    // Iterates over all adjacent vertices
    while (adjacentVertex != NULL)
    {
      if (colors[adjacentVertex->number] == WHITE)
      { // Visit the vertex if it has not been visited before
        dfs_visit(adjacentVertex->number, colors);
      }
      adjacentVertex = adjacentVertex->next;
    }
  };

public:
  Graph(int vertexCount)
  {
    this->vertexCount = vertexCount;
    this->adjacencyList = new ListObject *[vertexCount];
  }

  ~Graph()
  {
    for (int i = 0; i < vertexCount; i++)
    {
      ListObject *current = adjacencyList[i];
      while (current != NULL)
      {
        ListObject *next = current->next;
        delete current;
        current = next;
      }
    }
    delete[] adjacencyList;
  }

  void insertEdge(int xVertex, int yVertex)
  {
    adjacencyList[xVertex] = new ListObject(yVertex, adjacencyList[xVertex]);
    adjacencyList[yVertex] = new ListObject(xVertex, adjacencyList[yVertex]);
  }

  void printEdges()
  {
    // iterate over each vertex
    for (int i = 0; i < vertexCount; i++)
    {
      std::cout << i << ": ";
      ListObject *adjacenctVertex = this->adjacencyList[i];

      if (adjacenctVertex == NULL)
      {
        std::cout << EMPTY_ADJACENCY_LIST << std::endl;
        continue;
      }

      // iterate over each adjacent vertex
      while (adjacenctVertex != NULL)
      {
        std::cout << adjacenctVertex->number << " ";
        adjacenctVertex = adjacenctVertex->next;
      }
      std::cout << std::endl;
    }
  }

  void dfs()
  {
    char colors[this->vertexCount];

    for (int i = 0; i < this->vertexCount; i++)
    {
      colors[i] = WHITE; // paint all vertices white
    }

    this->dfs_visit(0, colors); // visit all the vertices reachable from vertex 0
  }
};

int main()
{
  int vertexCount = 0;
  std::cin >> vertexCount;

  Graph graph = Graph(vertexCount);

  int xVertex, yVertex = 0;
  int acceptingConnections = 1;
  while (acceptingConnections)
  {
    std::cin >> xVertex >> yVertex >> acceptingConnections;
    graph.insertEdge(xVertex, yVertex);
  }

  graph.printEdges();
  std::cout << std::endl;
  graph.dfs();

  return 0;
}