#include "jkr/sort/reverse_quicksort.hpp"
#include "jkr/sort/quicksort.hpp"
#include <fmt/core.h>
#include <vector>

int main() {
  fmt::println("Hello world!");

  std::vector<int> data = { 9,2,6,5,7,8,3,1,1,0,4 };

  jkr::sort::quicksort(data);

  for (int n : data)
    fmt::print("{}, ", n);
  fmt::println(" [DONE]");

  return 0;
}
