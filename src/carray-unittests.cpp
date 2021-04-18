#include <algorithm>

#include "catch.hpp"
#include "carray.hpp"

TEST_CASE("check default constructor", "[CArray]")
{
  const auto array = CArray<int>{};

  REQUIRE(array.size() == 0);
  REQUIRE(array.capacity() == 0);
  REQUIRE(array.begin() == array.end());
}

TEST_CASE("check copy constructor", "[CArray]")
{
  auto array = CArray<int>{};
  array.push_back(0);
  array.push_back(1);
  array.push_back(3);

  const auto cloned = array;
  REQUIRE(cloned.size() == 3);
  REQUIRE(cloned.capacity() >= 3);
  REQUIRE(std::equal(array.begin(), array.end(), cloned.begin(), cloned.end()));
}

TEST_CASE("check assignment operator", "[CArray]")
{
  auto first = CArray<int>{};
  auto second = CArray<int>{};

  first.push_back(10);
  first.push_back(20);
  first.push_back(30);

  second = first;
  REQUIRE(second.size() == 3);
  REQUIRE(second.capacity() >= 3);
  REQUIRE(std::equal(first.begin(), first.end(), second.begin(), second.end()));
}

TEST_CASE("check append item", "[CArray]")
{
  auto array = CArray<int>{};

  array.push_back(0);
  REQUIRE(array.size() == 1);
  REQUIRE(array.capacity() >= 1);
  REQUIRE(array[0] == 0);
  
  array.push_back(1);
  REQUIRE(array.size() == 2);
  REQUIRE(array.capacity() >= 2);
  REQUIRE(array[1] == 1);

  array.push_back(3);
  REQUIRE(array.size() == 3);
  REQUIRE(array.capacity() >= 3);
  REQUIRE(array[2] == 3);
}

TEST_CASE("check insert item", "[CArray]")
{
  auto array = CArray<int>{};

  array.push_back(10);
  array.push_back(20);
  array.push_back(30);

  array.insert(1, 15);
  REQUIRE(array.size() == 4);
  REQUIRE(array.capacity() >= 4);
  CHECK(array[0] == 10);
  CHECK(array[1] == 15);
  CHECK(array[2] == 20);
  CHECK(array[3] == 30);
}

TEST_CASE("check erase item", "[CArray]")
{
  auto array = CArray<int>{};

  array.push_back(10);
  array.push_back(15);
  array.push_back(20);
  array.push_back(30);

  array.erase(1);
  REQUIRE(array.size() == 3);
  REQUIRE(array.capacity() >= 3);
  CHECK(array[0] == 10);
  CHECK(array[1] == 20);
  CHECK(array[2] == 30);

  array.erase(1);
  REQUIRE(array.size() == 2);
  REQUIRE(array.capacity() >= 2);
  CHECK(array[0] == 10);
  CHECK(array[1] == 30);

  array.erase(1);
  REQUIRE(array.size() == 1);
  REQUIRE(array.capacity() >= 1);
  CHECK(array[0] == 10);

  array.erase(0);
  REQUIRE(array.size() == 0);
  REQUIRE(array.capacity() >= 0);
}

TEST_CASE("check erase range", "[CArray]")
{
  auto array = CArray<int>{};

  array.push_back(10);
  array.push_back(20);
  array.push_back(30);
  array.push_back(40);
  array.push_back(50);

  const auto from = array.begin() + 1;
  const auto to = array.begin() + 4;
  array.erase(from, to);
  REQUIRE(array.size() == 2);
  REQUIRE(array.capacity() >= 2);
  CHECK(array[0] == 10);
  CHECK(array[1] == 50);
}

TEST_CASE("check clear array", "[CArray]")
{
  auto first = CArray<int>{};
  first.push_back(10);
  first.push_back(20);
  first.push_back(30);

  first.clear();
  REQUIRE(first.size() == 0);
  REQUIRE(first.capacity() >= 0);
}
