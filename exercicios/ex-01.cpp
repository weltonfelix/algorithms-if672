#include <iostream>

class Object
{
public:
  Object *prev;
  char op;

  Object(char op, Object *prev)
  {
    this->op = op;
    this->prev = prev;
  }
};

class Stack
{
public:
  int size = 0;
  Object *top;

  bool stack_empty()
  {
    return this->size == 0;
  }

  void push(Object *obj)
  {
    obj->prev = this->top;
    this->top = obj;
    this->size++;
  }

  Object *pop()
  {
    if (this->stack_empty())
    {
      return NULL;
    }
    else
    {
      this->size--;
      Object *old_top = this->top;
      this->top = this->top->prev;

      return old_top;
    }
  }
};

typedef struct Node
{
  std::string value;
  struct Node *next;
  struct Node *prev;
} Node;

Node *create(std::string value)
{
  Node *newNode = new Node;
  newNode->value = value;
  newNode->next = nullptr;
  newNode->prev = nullptr;

  return newNode;
}

void insert(std::string value, Node *&head)
{
  Node *newNode = create(value);

  if (head == nullptr)
  {
    head = newNode;
  }
  else
  {
    Node *current = head;
    while (current->next != nullptr)
    {
      current = current->next;
    }
    current->next = newNode;
    newNode->prev = current;
  }
}

int main()
{
  std::string conventional_notation;
  std::cin >> conventional_notation;

  Node *linkedList = nullptr;

  Stack operators_stack = Stack();
  std::string number = "";

  if (conventional_notation[0] != '(')
  {
    conventional_notation = "(" + conventional_notation + ")";
  }

  for (int i = 0; i < conventional_notation.length(); i++)
  {
    switch (conventional_notation[i])
    {
    case ')':
      if (!number.empty())
      {
        insert(number, linkedList);
        number = "";
      }

      if (!operators_stack.stack_empty())
      {
        insert(std::string(1, operators_stack.pop()->op), linkedList);
      }
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
      if (!number.empty())
      {
        insert(number, linkedList);
        number = "";
      }
      operators_stack.push(new Object(conventional_notation[i], operators_stack.top));
      break;
    case '(':
      break;
    default:
      number += conventional_notation[i];
      break;
    }
  }

  if (!number.empty())
  {
    insert(number, linkedList);
    number = "";
  }
  
  
  Node *current = linkedList;
  while (current != nullptr)
  {
    if (current->value == "+") {
      float result = (float) std::stof(current->prev->prev->value) + std::stof(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    } else if (current->value == "-") {
      float result = (float) std::stof(current->prev->prev->value) - std::stof(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    } else if (current->value == "*") {
      float result = (float) std::stof(current->prev->prev->value) * std::stof(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    } else if (current->value == "/") {
      float result = (float) std::stof(current->prev->prev->value) / std::stof(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    } else if (current->value == "^") {
      float result = std::stoi(current->prev->prev->value) ^ std::stoi(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    } else if (current->value == "%") {
      float result = std::stoi(current->prev->prev->value) % std::stoi(current->prev->value);
      current->prev->prev->value = std::to_string(result);
      if (current->next != nullptr) {
        current->prev->prev->next = current->next;
      } else {
        current->prev->prev->next = nullptr;
        current->prev->prev->prev = nullptr;
      }
      if (current->next != nullptr) {
        current->next->prev = current->prev->prev;
      }
    }
    current = current->next;
  }

  std::cout << linkedList->value << std::endl;
}