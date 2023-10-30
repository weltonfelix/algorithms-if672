#include <iostream>

int main()
{
  std::string input;
  std::string largestArray;
  int largestArrayLength = 0;

  do
  {
    std::getline(std::cin, input);
    if (input.length() == 0)
      continue;

    int arrayLength = 0;

    for (int i = 0; i < input.length(); ++i)
    {
      if (input[i] == ',')
      {
        ++arrayLength;
      }
    }

    if (arrayLength == 0)
    {
      for (int i = 0; i < input.length(); ++i)
      {
        switch (input[i])
        {
        case '[':
        case ']':
        case ' ':
        case '\n':
          break;
        default:
          arrayLength = 1;
          break;
        }
      }
    }
    else
    {
      ++arrayLength;
    }

    if (largestArrayLength == 0 || arrayLength > largestArrayLength)
    {
      largestArray = input;
      largestArrayLength = arrayLength;
    }
  } while (input.length() > 0);

  std::cout << largestArray << std::endl;
}