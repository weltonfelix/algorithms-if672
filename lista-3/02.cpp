#include <iostream>
#include <cmath>

class Person
{
public:
  std::string name;
  int priority;
  int id;
  int seatRow = -1;

  Person(std::string name, int priority, int id, int seatRow)
  {
    this->name = name;
    this->priority = priority;
    this->id = id;
    this->seatRow = seatRow;
  }
};

class ListObject
{
public:
  ListObject *next;
  Person *data;

  ListObject(Person *data, ListObject *next)
  {
    this->data = data;
    this->next = next;
  }
};

class HashTable
{
private:
  ListObject **table;
  int size;

  int hash(std::string value, int id)
  {
    int sum = 0;
    for (int i = 0; i < value.length(); i++)
    {
      sum += value[i] * (i + 1);
    }

    return ((sum * (id + 1)) % size);
  };

public:
  HashTable(int size)
  {
    this->size = size;
    table = new ListObject *[size];
    for (int i = 0; i < size; i++)
    {
      table[i] = NULL;
    }
  };

  ~HashTable()
  {
    for (int i = 0; i < size; i++)
    {
      ListObject *current = table[i];
      while (current != NULL)
      {
        ListObject *next = current->next;
        delete current;
        current = next;
      }
    }
    delete[] table;
  };

  void insert(Person *person)
  {
    int target = hash(person->name, person->id);
    table[target] = new ListObject(person, table[target]);
  };

  Person *find(std::string value, int id)
  {
    int i = hash(value, id);
    ListObject *current = table[i];

    while (current != NULL)
    {
      if (current->data->id == id && current->data->name == value)
      {
        break;
      }
      current = current->next;
    }

    if (current == NULL)
    {
      return NULL;
    }
    else
    {
      return current->data;
    }
  };

  Person *remove(std::string value, int id)
  {
    int target = hash(value, id);
    ListObject *current = this->table[target];
    ListObject *previous = NULL;

    while (current != NULL && current->data->name != value)
    {
      previous = current;
      current = current->next;
    }

    if (current != NULL)
    {
      if (previous != NULL)
      {
        previous->next = current->next;
      }
      else
      {
        table[target] = current->next;
      }

      return current->data;
    }

    return NULL;
  };
};

class PriorityQueue
{
protected:
  Person **heap;
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
    this->heap = new Person *[capacity];
  };

  ~PriorityQueue()
  {
    delete[] heap;
  };

  bool is_full()
  {
    return this->size == this->capacity;
  };
};

class MinPriorityQueue : public PriorityQueue
{
private:
  void min_heapify(Person *p, int i)
  {
    int l = left(i);
    int r = right(i);

    int smallest = -1;

    if (l < this->size && this->heap[l]->priority < p->priority)
    {
      smallest = l;
    }
    else if (l < this->size && this->heap[l]->priority == p->priority && this->heap[l]->id < p->id)
    {
      smallest = l;
    }
    else
    {
      smallest = i;
    }

    if (r < this->size && this->heap[r]->priority < this->heap[smallest]->priority)
    {
      smallest = r;
    }
    else if (r < this->size && this->heap[r]->priority == this->heap[smallest]->priority && this->heap[r]->id < this->heap[smallest]->id)
    {
      smallest = r;
    }

    if (smallest != i)
    {
      Person *aux = this->heap[i];
      this->heap[i] = this->heap[smallest];
      this->heap[smallest] = aux;

      min_heapify(this->heap[smallest], smallest);
    }
  }

public:
  using PriorityQueue::PriorityQueue;

  Person *heap_minimum()
  {
    return this->heap[0];
  };

  Person *heap_extract_min()
  {
    if (this->size < 1)
    {
      return NULL;
    }

    Person *min = this->heap[0];
    this->heap[0] = this->heap[this->size - 1];
    this->size--;
    min_heapify(this->heap[0], 0);

    return min;
  };

  void min_heap_insert(Person *p)
  {
    this->size++;
    int i = this->size - 1;
    this->heap[i] = p;

    while (i > 0 && this->heap[parent(i)]->priority > this->heap[i]->priority)
    {
      Person *aux = this->heap[i];
      this->heap[i] = this->heap[parent(i)];
      this->heap[parent(i)] = aux;

      i = parent(i);
    }
  };

  void remove(std::string name, int id)
  {
    int index_to_remove = -1;
    for (int i = 0; i < this->size; i++)
    {
      if (this->heap[i]->id == id && this->heap[i]->name == name)
      {
        index_to_remove = i;
        break;
      }
    }

    if (index_to_remove != -1)
    {
      this->heap[index_to_remove] = this->heap[this->size - 1];
      this->heap[this->size - 1] = NULL;
      this->size--;

      min_heapify(this->heap[index_to_remove], index_to_remove);
    }
  };
};

class MaxPriorityQueue : public PriorityQueue
{
private:
  void max_heapify(Person *p, int i)
  {
    int l = left(i);
    int r = right(i);

    int largest = -1;

    if (l<this->size &&this->heap[l]->priority> p->priority)
    {
      largest = l;
    }
    else if (l < this->size && this->heap[l]->priority == p->priority && this->heap[l]->id < p->id)
    {
      largest = l;
    }
    else
    {
      largest = i;
    }

    if (r<this->size &&this->heap[r]->priority> this->heap[largest]->priority)
    {
      largest = r;
    }
    else if (r < this->size && this->heap[r]->priority == this->heap[largest]->priority && this->heap[r]->id < this->heap[largest]->id)
    {
      largest = r;
    }

    if (largest != i)
    {
      Person *aux = this->heap[i];
      this->heap[i] = this->heap[largest];
      this->heap[largest] = aux;

      max_heapify(this->heap[largest], largest);
    }
  }

public:
  using PriorityQueue::PriorityQueue;

  Person *heap_maximum()
  {
    return this->heap[0];
  };

  Person *heap_extract_max()
  {
    if (this->size < 1)
    {
      return NULL;
    }

    Person *max = this->heap[0];
    this->heap[0] = this->heap[this->size - 1];
    this->size--;
    max_heapify(this->heap[0], 0);

    return max;
  };

  void max_heap_insert(Person *p)
  {
    this->size++;
    int i = this->size - 1;
    this->heap[i] = p;

    while (i > 0 && this->heap[parent(i)]->priority < this->heap[i]->priority)
    {
      Person *aux = this->heap[i];
      this->heap[i] = this->heap[parent(i)];
      this->heap[parent(i)] = aux;

      i = parent(i);
    }
  };

  void remove(std::string name, int id)
  {
    int index_to_remove = -1;
    for (int i = 0; i < this->size; i++)
    {
      if (this->heap[i]->id == id && this->heap[i]->name == name)
      {
        index_to_remove = i;
      }
    }

    if (index_to_remove != -1)
    {
      this->heap[index_to_remove] = this->heap[this->size - 1];
      this->heap[this->size - 1] = NULL;
      this->size--;

      max_heapify(this->heap[index_to_remove], index_to_remove);
    }
  };
};

class SeatsManager
{
private:
  HashTable *peopleHashTable;
  MinPriorityQueue *seatedPeoplePriorityQueue;
  int rowsAmount;
  int seatsPerRow;

  MinPriorityQueue **rows;
  MaxPriorityQueue *standingPeople;

  int peopleAmount = 0;

public:
  SeatsManager(int rowsAmount, int seatsPerRow)
  {
    this->rowsAmount = rowsAmount;
    this->seatsPerRow = seatsPerRow;

    this->peopleHashTable = new HashTable(2 * (rowsAmount * seatsPerRow));

    this->rows = new MinPriorityQueue *[rowsAmount];
    for (int i = 0; i < rowsAmount; i++)
    {
      this->rows[i] = new MinPriorityQueue(seatsPerRow);
    }

    this->standingPeople = new MaxPriorityQueue(rowsAmount * seatsPerRow);
    this->seatedPeoplePriorityQueue = new MinPriorityQueue(rowsAmount * seatsPerRow);
  };

  void insert(std::string name, int priority)
  {
    Person *person = new Person(name, priority, peopleAmount, -1);
    for (int i = 0; i < rowsAmount; i++)
    {
      if (!this->rows[i]->is_full())
      {

        person->seatRow = i;
        this->rows[i]->min_heap_insert(person);
        this->peopleHashTable->insert(person);
        this->seatedPeoplePriorityQueue->min_heap_insert(person);
        this->peopleAmount++;

        std::cout << person->name << " (" << person->id + 1 << ") foi alocado(a) na fileira " << person->seatRow + 1 << std::endl;

        return;
      }
    }

    if (this->seatedPeoplePriorityQueue->heap_minimum()->priority < priority)
    {
      Person *seatedPerson = this->seatedPeoplePriorityQueue->heap_extract_min();
      this->rows[seatedPerson->seatRow]->heap_extract_min();
      this->rows[seatedPerson->seatRow]->min_heap_insert(person);
      this->seatedPeoplePriorityQueue->min_heap_insert(person);
      this->standingPeople->max_heap_insert(seatedPerson);
      person->seatRow = seatedPerson->seatRow;
      seatedPerson->seatRow = -1;
      this->peopleHashTable->insert(person);
      this->peopleAmount++;

      std::cout << person->name << " (" << person->id + 1 << ") foi alocado(a) na fileira " << person->seatRow + 1 << std::endl;

      return;
    }

    this->peopleHashTable->insert(person);
    this->standingPeople->max_heap_insert(person);
    this->peopleAmount++;
    std::cout << person->name << " (" << person->id + 1 << ") nao foi alocado(a) em nenhuma fileira" << std::endl;
  };

  void remove(std::string name, int id)
  {
    Person *person = this->peopleHashTable->remove(name, id - 1);

    if (person == NULL)
    {
      std::cout << "Inexistente" << std::endl;
      return;
    }

    if (person->seatRow != -1)
    {
      this->rows[person->seatRow]->remove(name, id - 1);
      this->seatedPeoplePriorityQueue->remove(name, id - 1);

      Person *standingPerson = this->standingPeople->heap_extract_max();
      if (standingPerson != NULL)
      {
        standingPerson->seatRow = person->seatRow;
        this->rows[person->seatRow]->min_heap_insert(standingPerson);
        this->seatedPeoplePriorityQueue->min_heap_insert(standingPerson);
      }
    }
    else
    {
      this->standingPeople->remove(name, id - 1);
    }

    std::cout << "Removido(a)" << std::endl;
  };

  void find(std::string name, int id)
  {
    Person *person = this->peopleHashTable->find(name, id - 1);

    if (person == NULL)
    {
      std::cout << "Inexistente" << std::endl;
    }
    else if (person->seatRow == -1)
    {
      std::cout << "Sem assento" << std::endl;
    }
    else
    {
      std::cout << "Sentado(a) na fileira " << person->seatRow + 1 << std::endl;
    }
  };
};

int main()
{
  int rows;
  int seatsPerRow;
  std::cin >> rows;
  std::cin >> seatsPerRow;

  SeatsManager *manager = new SeatsManager(rows, seatsPerRow);

  int commands;
  std::cin >> commands;
  for (int i = 0; i < commands; i++)
  {
    std::string command;
    std::cin >> command;

    std::string name;
    std::cin >> name;

    if (command == "CAD")
    {
      int priority;
      std::cin >> priority;

      manager->insert(name, priority);
    }
    else if (command == "REM")
    {
      int id;
      std::cin >> id;
      manager->remove(name, id);
    }
    else if (command == "VER")
    {
      int id;
      std::cin >> id;
      manager->find(name, id);
    }
  }

  return 0;
}