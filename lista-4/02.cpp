#include <iostream>

#define WHITE 'W'
#define GRAY 'G'
#define BLACK 'B'

#define UNREACHABLE_VERTEX "Labirinto Impossivel"

class Vertex
{
public:
  int index;
  char color = ' ';
  int distance = -1;
  Vertex *parent = NULL;

  Vertex(int index)
  {
    this->index = index;
  }
};

class ListObject
{
public:
  ListObject *next;
  Vertex *vertex;

  ListObject(Vertex *vertex, ListObject *next)
  {
    this->vertex = vertex;
    this->next = next;
  }
};

class QueueObject
{
public:
  QueueObject *next;
  Vertex *vertex;

  QueueObject(Vertex *vertex, QueueObject *next = NULL)
  {
    this->vertex = vertex;
    this->next = next;
  }
};

class Queue
{
public:
  int size = 0;
  QueueObject *head;
  QueueObject *tail;

  bool isEmpty()
  {
    return this->size == 0;
  }

  void enqueue(QueueObject *obj)
  {
    if (this->isEmpty())
    {
      this->head = obj;
      this->tail = obj;
    }
    else
    {
      this->tail->next = obj;
      this->tail = obj;
    }

    this->size++;
  }

  void empty()
  {
    while (!this->isEmpty())
    {
      delete this->dequeue();
    }
  }

  QueueObject *dequeue()
  {
    if (this->isEmpty())
    {
      return NULL;
    }
    else
    {
      this->size--;

      QueueObject *old_head = this->head;
      this->head = this->head->next;

      return old_head;
    }
  }

  ~Queue()
  {
    this->empty();
  }
};

class Graph
{
private:
  int vertexCount;

public:
  Vertex **vertices;
  ListObject **adj;

  Graph(int vertexCount)
  {
    this->vertexCount = vertexCount;
    this->adj = new ListObject *[vertexCount];
    this->vertices = new Vertex *[vertexCount];

    for (int i = 0; i < vertexCount; i++)
    {
      this->adj[i] = NULL;
      this->vertices[i] = NULL;
    }
  }

  ~Graph()
  {
    for (int i = 0; i < vertexCount; i++)
    {
      ListObject *current = adj[i];
      while (current != NULL)
      {
        ListObject *next = current->next;
        delete current;
        current = next;
      }
    }

    for (int i = 0; i < vertexCount; i++)
    {
      delete vertices[i];
    }

    delete[] adj;
    delete[] vertices;
  }

  void addEdge(int vertex1_index, int vertex2_index)
  {
    Vertex *vertex1 = this->vertices[vertex1_index];
    Vertex *vertex2 = this->vertices[vertex2_index];

    if (vertex1 == NULL || vertex2 == NULL)
    {
      throw std::invalid_argument("Vertex not found");
    }

    adj[vertex1_index] = new ListObject(vertex2, adj[vertex1_index]);
    adj[vertex2_index] = new ListObject(vertex1, adj[vertex2_index]);
  }

  void addVertex(Vertex *vertex)
  {
    this->vertices[vertex->index] = vertex;
  }

  void bfs(int source_index, Queue *Q)
  {
    for (int i = 0; i < this->vertexCount; i++)
    {
      Vertex *u = this->vertices[i];
      if (u->index == source_index)
      {
        u->color = GRAY;
        u->distance = 0;
        u->parent = NULL;
        continue;
      }

      u->color = WHITE;
      u->distance = -1;
      u->parent = NULL;
    }

    Q->empty();
    Q->enqueue(new QueueObject(this->vertices[source_index]));
    while (!Q->isEmpty())
    {
      QueueObject *u_queue_obj = Q->dequeue();
      Vertex *u = u_queue_obj->vertex;
      delete u_queue_obj;

      // search the neighbors of u
      ListObject *current_neighbor = this->adj[u->index];
      while (current_neighbor != NULL)
      {
        Vertex *v = current_neighbor->vertex;

        if (v->color == WHITE)
        { // is v being discovered now?
          v->color = GRAY;
          v->distance = u->distance + 1;
          v->parent = u;

          Q->enqueue(new QueueObject(v)); // v is now on the frontier
        }

        current_neighbor = current_neighbor->next;
      }

      u->color = BLACK; // u is now behind the frontier
    }
  }

  int path_length(Vertex *s, Vertex *v)
  {
    if (v != s)
    {
      if (v->parent == NULL)
      {
        return -1;
      }

      int length = path_length(s, v->parent);
      if (length == -1)
      {
        return -1;
      }
      return 1 + length;
    }
    return 0;
  }
};

int main()
{
  int lines, columns;

  std::cin >> lines >> columns;

  int maze_matrix[lines][columns];

  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      int value;
      std::cin >> value;

      maze_matrix[i][j] = value;
    }
  }

  int vertexCount = lines * columns;

  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      if (maze_matrix[i][j] == 1)
      {
        vertexCount--;
      }
    }
  }

  Graph *G = new Graph(vertexCount);

  int index_counter = 0;
  int start, end;

  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      switch (maze_matrix[i][j])
      {
      case 1:
        maze_matrix[i][j] = -1;
        break;
      case 0:
        maze_matrix[i][j] = index_counter;
        break;
      case 2:
        maze_matrix[i][j] = index_counter;
        start = index_counter;
        break;
      case 3:
        maze_matrix[i][j] = index_counter;
        end = index_counter;
        break;
      default:
        break;
      }

      if (maze_matrix[i][j] != -1)
      {
        Vertex *new_vertex = new Vertex(index_counter);
        G->addVertex(new_vertex);

        // upper line
        if (i > 0)
        {
          // test upper
          if (maze_matrix[i - 1][j] != -1)
          {
            G->addEdge(index_counter, maze_matrix[i - 1][j]);
          }
        }
        if (j > 0)
        {
          // check left
          if (maze_matrix[i][j - 1] != -1)
          {
            G->addEdge(index_counter, maze_matrix[i][j - 1]);
          }
        }
        index_counter++;
      }
    }
  }

  Queue *Q = new Queue();

  G->bfs(start, Q);
  delete Q;

  int distance = G->path_length(G->vertices[start], G->vertices[end]);
  if (distance == -1)
  {
    std::cout << UNREACHABLE_VERTEX << std::endl;
  }
  else
  {
    std::cout << distance << std::endl;
  }

  delete G;

  return 0;
}
