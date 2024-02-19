#include <iostream>

#define LEFT "esquerdo"
#define RIGHT "direito"

class QueueObject
{
public:
  QueueObject *next;
  int length_cm;

  QueueObject(int length_cm, QueueObject *next)
  {
    this->length_cm = length_cm;
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
};

int main()
{
  int cases;
  std::cin >> cases;

  Queue left_queue = Queue();
  Queue right_queue = Queue();

  for (int i = 0; i < cases; i++)
  {
    int ferry_length_cm;
    int cars;

    std::cin >> ferry_length_cm;
    ferry_length_cm *= 100;
    std::cin >> cars;

    for (int j = 0; j < cars; j++)
    {
      int car_length_cm;
      std::string river_side;

      std::cin >> car_length_cm;
      std::cin >> river_side;

      QueueObject *new_car = new QueueObject(car_length_cm, NULL);

      if (river_side == LEFT)
      {
        left_queue.enqueue(new_car);
      }
      else
      {
        right_queue.enqueue(new_car);
      }
    }

    int travels = 0;
    int remaining_space_left_cm = ferry_length_cm;
    int remaining_space_right_cm = ferry_length_cm;

    while (!left_queue.isEmpty() || !right_queue.isEmpty())
    {
      while (!left_queue.isEmpty() && remaining_space_left_cm - left_queue.head->length_cm >= 0)
      {
        QueueObject *car = left_queue.dequeue();
        remaining_space_left_cm -= car->length_cm;
        delete car;
      }
      if (remaining_space_left_cm < ferry_length_cm || !right_queue.isEmpty())
      {
        travels++;
      }
      remaining_space_left_cm = ferry_length_cm;

      while (!right_queue.isEmpty() && remaining_space_right_cm - right_queue.head->length_cm >= 0)
      {
        QueueObject *car = right_queue.dequeue();
        remaining_space_right_cm -= car->length_cm;
        delete car;
      }
      if (remaining_space_right_cm < ferry_length_cm || !left_queue.isEmpty())
      {
        travels++;
      }
      remaining_space_right_cm = ferry_length_cm;
    }

    std::cout << "Caso " << i + 1 << ": " << travels << std::endl;
  }
}