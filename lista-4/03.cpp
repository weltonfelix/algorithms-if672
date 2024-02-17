#include <iostream>
#include <cmath>

class Vertex
{
public:
  int index;
  int d = -1;
  Vertex *parent = NULL;

  Vertex(int index)
  {
    this->index = index;
  }
};

class Edge
{
public:
  Vertex *origin;
  Vertex *destination;
  int weight;

  Edge(Vertex *origin, Vertex *destination, int weight)
  {
    this->origin = origin;
    this->destination = destination;
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

  ~ListObject()
  {
    delete element;
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
};

template <class T>
class MinPriorityQueue : public PriorityQueue<T>
{
private:
  void minHeapify(PriorityQueueObject<T> *element, int i)
  {
    int l = PriorityQueue<T>::left(i);
    int r = PriorityQueue<T>::right(i);

    int smallest = -2;

    if (l < this->size && ((*(this->heap[l]->priority) < *(element->priority) && *(this->heap[l]->priority) != -1) || (*(element->priority) == -1 && *(this->heap[l]->priority) != -1)))
    {
      smallest = l;
    }
    else
    {
      smallest = i;
    }

    if (r < this->size && ((*(this->heap[r]->priority) < *(this->heap[smallest]->priority) && *(this->heap[r]->priority) != -1) || ((*(this->heap[smallest]->priority) == -1 && (*(this->heap[r]->priority) != -1)))))
    {
      smallest = r;
    }

    if (smallest != i)
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

  void decreaseKey(T *el, int *updated_key)
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

    while (
        elIndex > 0 &&
        ((*(this->heap[MinPriorityQueue::parent(elIndex)]->priority) > *updated_key && *updated_key != -1) ||
         *(this->heap[MinPriorityQueue::parent(elIndex)]->priority) == -1))
    {
      PriorityQueueObject<T> *aux = this->heap[elIndex];
      this->heap[elIndex] = this->heap[MinPriorityQueue::parent(elIndex)];
      this->heap[MinPriorityQueue::parent(elIndex)] = aux;

      elIndex = MinPriorityQueue::parent(elIndex);
    }
  }
};

class Graph
{
private:
  int vertexCount;

  void initializeSingleSource(Vertex *source)
  {
    for (int i = 0; i < vertexCount; i++)
    {
      Vertex *v = vertices[i];
      v->d = -1;
      v->parent = NULL;
    }

    source->d = 0;
  }

  bool relax(Edge *edge)
  {
    Vertex *u = edge->origin;
    Vertex *v = edge->destination;

    if (u->d != -1 && (v->d == -1 || v->d > u->d + edge->weight))
    {
      v->d = u->d + edge->weight;
      v->parent = u;
      return true;
    }

    return false;
  }

public:
  Vertex **vertices;
  List<Edge> **adj;

  Graph(int vertexCount)
  {
    this->vertexCount = vertexCount;
    this->adj = new List<Edge> *[vertexCount];
    this->vertices = new Vertex *[vertexCount];

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

    delete[] adj;
    delete[] vertices;
  }

  void addEdge(int originIndex, int destinationIndex, int weight)
  {
    Vertex *origin = this->vertices[originIndex];
    Vertex *destination = this->vertices[destinationIndex];

    if (weight < 0)
    {
      throw std::invalid_argument("Weight must be non-negative");
    }
    Edge *edge = new Edge(origin, destination, weight);
    this->adj[originIndex]->insert(edge);
  }

  void dijkstra(Vertex *source)
  {
    this->initializeSingleSource(source);

    MinPriorityQueue<Vertex> *Q = new MinPriorityQueue<Vertex>(this->vertexCount);

    for (int i = 0; i < this->vertexCount; i++)
    {
      Q->insert(this->vertices[i], &this->vertices[i]->d);
    }
    while (!Q->isEmpty())
    {
      Vertex *u = Q->extractMin();

      ListObject<Edge> *currentAdjToU = this->adj[u->index]->head;
      while (currentAdjToU != NULL)
      {
        if (this->relax(currentAdjToU->element))
        {
          Vertex *v = currentAdjToU->element->destination;
          Q->decreaseKey(v, &v->d);
        }

        currentAdjToU = currentAdjToU->next;
      }
    }

    delete Q;
  }
};

void handleBuildStreet(Graph *G)
{
  int blockA, blockB, timeSpent;
  std::cin >> blockA >> blockB >> timeSpent;

  G->addEdge(blockA, blockB, timeSpent);
}

int main()
{
  int blocksCount, streetsAmount, eventsAmount;

  std::cin >> blocksCount >> streetsAmount >> eventsAmount;

  Graph *G = new Graph(blocksCount);

  for (int i = 0; i < streetsAmount; i++)
  {
    handleBuildStreet(G);
  }

  for (int i = 0; i < eventsAmount; i++)
  {
    int eventType;
    std::cin >> eventType;

    switch (eventType)
    {
    case 1:
      handleBuildStreet(G);
      break;
    case 2:
      int blockA, blockB;
      std::cin >> blockA >> blockB;

      G->dijkstra(G->vertices[blockA]);
      std::cout << G->vertices[blockB]->d << std::endl;
      break;
    default:
      break;
    }
  }

  delete G;

  return 0;
}
