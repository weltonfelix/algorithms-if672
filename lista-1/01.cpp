#include <iostream>

class QueueObject
{
public:
  QueueObject *prev;
  int number;

  QueueObject(int number, QueueObject *prev)
  {
    this->number = number;
    this->prev = prev;
  }
};

class Stack
{
public:
  int size = 0;
  QueueObject *top;

  bool stack_empty()
  {
    return this->size == 0;
  }

  void push(QueueObject *obj)
  {
    obj->prev = this->top;
    this->top = obj;
    this->size++;
  }

  QueueObject *pop()
  {
    if (this->stack_empty())
    {
      return NULL;
    }
    else
    {
      this->size--;
      QueueObject *old_top = this->top;
      this->top = this->top->prev;

      return old_top;
    }
  }
};

int main()
{
  int stacks_count;
  std::cin >> stacks_count;

  Stack *stacks[stacks_count];

  for (int i = 0; i < stacks_count; i++)
  {
    int packets_in_stack;

    QueueObject *prev_box = NULL;
    stacks[i] = new Stack();

    std::cin;
    do
    {
      std::cin >> packets_in_stack;
      if (packets_in_stack != 0)
      {
        if (stacks[i]->top && stacks[i]->top->number % 2 == packets_in_stack % 2)
        {
          int new_number = std::abs(packets_in_stack - stacks[i]->top->number);
          delete stacks[i]->pop();
          QueueObject *box = new QueueObject(new_number, prev_box);
          stacks[i]->push(box);
        }
        else
        {
          QueueObject *box = new QueueObject(packets_in_stack, prev_box);
          stacks[i]->push(box);
        }
      }
    } while (packets_in_stack != 0);
  }

  for (int i = 0; i < stacks_count; i++)
  {
    std::cout << "Pilha " << i + 1 << ": " << stacks[i]->size << " " << stacks[i]->top->number << std::endl;
    
    while (!stacks[i]->stack_empty())
    {
      delete stacks[i]->pop();
    }
    
    delete stacks[i];
  }
}
