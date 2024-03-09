#include <iostream>
#include <cmath>

class Vertex
{
public:
  int index;
  int key;
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
  int weight;

  Edge(Vertex *vertex1, Vertex *vertex2, int weight)
  {
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;
    this->weight = weight;
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

template <typename T>
class PriorityQueueObject
{
public:
  int *priority;
  T *element;

  PriorityQueueObject(int *priority, T *element)
  {
    this->priority = priority;
    this->element = element;
  }
};

template <class T>
class PriorityQueue
{
protected:
  PriorityQueueObject<T> **heap;
  int size;
  int capacity;

  int parent(int index)
  {
    return std::floor((index - 1) / 2);
  }

  int left(int index)
  {
    return 2 * index + 1;
  }

  int right(int index)
  {
    return 2 * index + 2;
  }

public:
  PriorityQueue(int capacity)
  {
    this->capacity = capacity;
    this->size = 0;
    this->heap = new PriorityQueueObject<T> *[capacity];
  };

  ~PriorityQueue()
  {
    for (int i = 0; i < this->size; i++)
    {
      delete this->heap[i];
    }

    delete[] heap;
  };

  bool isEmpty()
  {
    return this->size == 0;
  }

  bool has(T *el)
  {
    for (int i = 0; i < this->size; i++)
    {
      if (this->heap[i]->element == el)
      {
        return true;
      }
    }

    return false;
  }
};

template <class T>
class MinPriorityQueue : public PriorityQueue<T>
{
private:
  void minHeapify(PriorityQueueObject<T> *element, int i)
  {
    int l = this->left(i);
    int r = this->right(i);

    int smallest;

    // if left element has lower priority than current element (-1 is +infinity priority)
    bool isLeftSmallerThanEl = l < this->size &&                   // if left element exists, and
                               *(this->heap[l]->priority) != -1 && // left priority is not +infinity
                               (*(element->priority) == -1 || *(this->heap[l]->priority) < *(element->priority));

    smallest = isLeftSmallerThanEl ? l : i;

    // if right element has lower priority than current smallest element (-1 is +infinity priority)
    bool isRightSmallerThanSmallest = r < this->size && // if right element exists
                                      *(this->heap[r]->priority) != -1 &&
                                      (*(this->heap[smallest]->priority) == -1 ||
                                       *(this->heap[r]->priority) < *(this->heap[smallest]->priority));

    smallest = isRightSmallerThanSmallest ? r : smallest;

    if (smallest != i) // if smallest element is not the current element, swap them and call minHeapify again
    {
      PriorityQueueObject<T> *aux = this->heap[i];
      this->heap[i] = this->heap[smallest];
      this->heap[smallest] = aux;

      minHeapify(this->heap[smallest], smallest);
    }
  }

public:
  using PriorityQueue<T>::PriorityQueue;
  T *extractMin()
  {
    if (this->size < 1)
    {
      return NULL;
    }

    T *min = this->heap[0]->element;
    delete this->heap[0];

    this->heap[0] = this->heap[this->size - 1];
    this->size--;
    minHeapify(this->heap[0], 0);

    return min;
  };

  void insert(T *el, int *priority)
  {
    PriorityQueueObject<T> *element = new PriorityQueueObject<T>(priority, el);

    this->size++;
    int i = this->size - 1;
    this->heap[i] = element;
    this->decreaseKey(el, priority);
  };

  void decreaseKey(T *el, int *newPriority)
  {
    int elIndex = -1;

    // find index i of element el
    for (int i = 0; i < this->size; i++)
    {
      if (this->heap[i]->element == el)
      {
        elIndex = i;
        break;
      }
    }

    if (elIndex == -1)
    {
      throw std::invalid_argument("Element not found");
    }

    // lambda function to check if element has lower priority than its parent
    auto elementHasLowerPriority = [&]() -> bool
    {
      int parentPriority = *(this->heap[this->parent(elIndex)]->priority);

      return parentPriority == -1 || (parentPriority > *newPriority && *newPriority != -1);
    };

    // while element has lower priority than its parent, swap them
    while (elIndex > 0 && elementHasLowerPriority())
    {
      PriorityQueueObject<T> *aux = this->heap[elIndex];
      this->heap[elIndex] = this->heap[this->parent(elIndex)];
      this->heap[this->parent(elIndex)] = aux;

      elIndex = this->parent(elIndex);
    }
  }
};

class Graph
{
private:
  int vertexCount;
  List<Edge> *edges;

public:
  Vertex **vertices;
  List<Edge> **adj;

  Graph(int vertexCount)
  {
    this->vertexCount = vertexCount;
    this->adj = new List<Edge> *[vertexCount];
    this->vertices = new Vertex *[vertexCount];
    this->edges = new List<Edge>();

    for (int i = 0; i < vertexCount; i++)
    {
      this->adj[i] = new List<Edge>();
      this->vertices[i] = new Vertex(i);
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
  }

  void addEdge(int vertex1Index, int vertex2Index, int weight)
  {
    Vertex *vertex1 = this->vertices[vertex1Index];
    Vertex *vertex2 = this->vertices[vertex2Index];

    Edge *edge = new Edge(vertex1, vertex2, weight);
    this->edges->insert(edge);
    this->adj[vertex1Index]->insert(edge);
    this->adj[vertex2Index]->insert(edge);
  }

  int mstPrim(int rootVertexIndex)
  {
    int totalWeight = 0;
    Vertex *rootVertex = this->vertices[rootVertexIndex];

    for (int i = 0; i < this->vertexCount; i++)
    {
      this->vertices[i]->key = -1;
      this->vertices[i]->parent = NULL;
    }

    rootVertex->key = 0;
    MinPriorityQueue<Vertex> *queue = new MinPriorityQueue<Vertex>(this->vertexCount);

    for (int i = 0; i < this->vertexCount; i++)
    {
      queue->insert(this->vertices[i], &this->vertices[i]->key);
    }

    while (!queue->isEmpty())
    {
      Vertex *u = queue->extractMin();

      ListObject<Edge> *currentAdjToU = this->adj[u->index]->head;
      while (currentAdjToU != NULL)
      {
        Edge *edge = currentAdjToU->element;
        Vertex *v = edge->vertex1 == u ? edge->vertex2 : edge->vertex1;

        if (queue->has(v) && (v->key == -1 || edge->weight < v->key))
        {
          // update total weight
          totalWeight += edge->weight;
          if (v->key != -1) {
            totalWeight -= v->key;
          }

          v->parent = u;
          v->key = edge->weight;
          queue->decreaseKey(v, &edge->weight);
        }

        currentAdjToU = currentAdjToU->next;
      }
    }

    delete queue;
    return totalWeight;
  }
};

int main()
{
  int airports, flights;

  std::cin >> airports >> flights;

  Graph graph(airports);

  for (int i = 0; i < flights; i++)
  {
    int airport1, airport2, cost;
    std::cin >> airport1 >> airport2 >> cost;
    graph.addEdge(airport1, airport2, cost);
  }

  std::cout << graph.mstPrim(0) << std::endl;

  return 0;
}