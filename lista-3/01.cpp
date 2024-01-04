#include <iostream>

class ListObject
{
public:
  ListObject *next;
  std::string name;

  ListObject(std::string name, ListObject *next)
  {
    this->name = name;
    this->next = next;
  }
};

class HashTable
{
private:
  ListObject **table;
  int size;

  int hash(std::string value)
  {
    int sum = 0;
    for (int i = 0; i < value.length(); i++)
    {
      sum += value[i] * (i + 1);
    }

    return ((sum * 17) % size);
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

  void POST(std::string value)
  {
    int target = hash(value);
    table[target] = new ListObject(value, table[target]);
  };

  void GET(std::string value)
  {
    int i = hash(value);
    ListObject *current = table[i];

    int j = -1;
    int k = 1;
    while (current != NULL)
    {
      if (current->name == value)
      {
        j = k;
      }
      current = current->next;
      k++;
    }

    if (j == -1)
    {
      std::cout << "404 - NOT FOUND" << std::endl;
    }
    else
    {
      std::cout << i << " " << j << std::endl;
    }
  };

  void DELETE(std::string value)
  {
    int target = hash(value);
    ListObject *current = this->table[target];
    ListObject *previous = NULL;

    while (current != NULL && current->name != value)
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

      current->name = "";
      delete current;

      std::cout << "DELETADO" << std::endl;
    }
  };
};

int main()
{
  int tableSize;
  std::cin >> tableSize;
  HashTable *table = new HashTable(tableSize);

  int users;
  std::cin >> users;

  for (int i = 0; i < users; i++)
  {
    std::string command;
    std::cin >> command;

    std::string name;
    std::cin >> name;
    table->POST(name);
  }

  int queries;
  std::cin >> queries;

  for (int i = 0; i < queries; i++)
  {
    std::string command;
    std::cin >> command;

    std::string name;
    std::cin >> name;

    if (command == "GET")
    {
      table->GET(name);
    }
    else if (command == "POST")
    {
      table->POST(name);
    }
    else if (command == "DELETE")
    {
      table->DELETE(name);
    }
  }

  delete table;
  return 0;
}