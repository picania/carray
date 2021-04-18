#include <algorithm>
#include <iostream>
#include <random>
#include <string>

#include "carray.hpp"

template<typename T>
void print(
    const CArray<T>& _array
  )
{
  bool first = true;
  std::cout << '[';
  for (const auto& item : _array)
  {
    if (!first)
    {
      std::cout << "; ";
    }

    std::cout << item;
    
    first = false;
  }
  std::cout << "]\n";
}

void first_test()
{
  auto array = CArray<int>{};

  std::random_device rd{};
  std::mt19937 mt{rd()};
  std::uniform_int_distribution<int> number(0, 100);

  std::cout << "Inserting 20 random numbers:\n";
  for (size_t i = 0; i != 20; ++i)
  {
    array.push_back(number(mt));
  }
  print(array);

  std::cout << "Sorting by ascending:\n";
  std::sort(array.begin(), array.end());
  print(array);

  std::cout << "Deleting in steps of 2:\n";
  size_t index = 0;
  auto it = std::remove_if(array.begin(), array.end(), [index](int num) mutable {
      return index++ % 2 == 1;
    });
  array.erase(it, array.end());
  print(array);

  std::cout << "Inserting 10 random numbers:\n";
  for (size_t i = 0; i != 10; ++i)
  {
    std::uniform_int_distribution<size_t> index(0, array.size() - 1);
    array.insert(index(mt), number(mt));
  }
  print(array);

  std::cout << "Clearing of container:\n";
  array.clear();
  print(array);
}

void second_test()
{
  auto array = CArray<std::string>{};

  std::random_device rd{};
  std::mt19937 mt{rd()};
  std::uniform_int_distribution<int> letter('a', 'z');
  std::uniform_int_distribution<size_t> size(5, 20);

  std::cout << "Inserting 15 random words:\n";
  for (size_t i = 0; i != 15; ++i)
  {
    array.push_back(std::string(size(mt), letter(mt)));
  }
  print(array);

  std::cout << "Sorting by ascending:\n";
  std::sort(array.begin(), array.end());
  print(array);

  std::cout << "Deleting words with 'abcde':\n";
  auto it = std::remove_if(array.begin(), array.end(), [](const std::string& word) {
      return word.find_first_of("abcde") != std::string::npos;
    });
  array.erase(it, array.end());
  print(array);

  std::cout << "Inserting 3 random words:\n";
  for (size_t i = 0; i != 3; ++i)
  {
    std::uniform_int_distribution<size_t> index(0, array.size() - 1);
    array.insert(index(mt), std::string(size(mt), letter(mt)));
  }
  print(array);
}

int main(
    int, char**
  )
{
  first_test();
  second_test();
}
