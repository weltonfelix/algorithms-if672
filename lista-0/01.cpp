#include <iostream>

int main()
{
  int opens = 0;
  bool malformed = false;

  std::string line;
  std::getline(std::cin, line);

  for (int i = 0; i < line.length(); ++i)
  {
    if (line[i] == '}')
    {
      if (opens <= 0)
      {
        malformed = true;
        break;
      }
      else
      {
        --opens;
      }
    }
    else if (line[i] == '{')
    {
      ++opens;
    }
  }

  malformed = malformed || opens > 0;

  std::cout << (malformed ? 'N' : 'S') << std::endl;
}