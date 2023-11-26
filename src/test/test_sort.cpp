#include "jkr/sort.hpp"

#include <fmt/core.h>

#include <chrono>
#include <float.h>
#include <vector>

using namespace jkr::sort;

template <typename T>
using vector = std::vector<T>;
using high_resolution_clock = std::chrono::high_resolution_clock;

using sorting_algorithm = void(*)(vector<int>&);

bool failed_sort(const vector<int>& data, bool ascending) {
  if (ascending) {
    for (unsigned int i = 1; i < data.size(); i++) {
      if (data[i] < data[i - 1])
        return true;
    }
    return false;
  }

  for (unsigned int i = 1; i < data.size(); i++) {
    if (data[i] > data[i - 1])
      return true;
  }
  return false;
}

void run_test(vector<vector<int>> data, std::string algorithm_name, sorting_algorithm sorting_algo, bool ascending) {
  std::chrono::time_point<high_resolution_clock> start_time, loop_time;

  // TODO: Synchronized pool for performing sorts w/ multithreading

  unsigned int size = data.size();
  double total_sort_time_ns = 0.0;
  double min_time_ns = DBL_MAX;
  double max_time_ms = DBL_MIN;
  double time_ns;
  
  start_time = high_resolution_clock::now();
  for (unsigned int i = 0; i < size; i++) {
    loop_time = high_resolution_clock::now();
    sorting_algo(data[i]);
    time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(high_resolution_clock::now() - loop_time).count();
    total_sort_time_ns += time_ns;

    if (time_ns < min_time_ns) min_time_ns = time_ns;
    if (time_ns > max_time_ms) max_time_ms = time_ns;

    if (failed_sort(data[i], ascending)) {
      fmt::println("\e[31m{:16}\e[0m[FAILED]", algorithm_name);
      return;
    }
  }
  time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(high_resolution_clock::now() - start_time).count();

  fmt::println("\e[32m{:16}\e[0m[PASSED] -> Test: {:.2f} ms, Avg: {:.2f} ms, Range: ({:.2f}, {:.2f}) ms, Total: {:.2f} ms", 
               algorithm_name, 
               total_sort_time_ns / 1.0e6, 
               total_sort_time_ns / ((double)data.size() * 1.0e6), 
               min_time_ns / 1.0e6, 
               max_time_ms / 1.0e6, 
               time_ns / 1.0e6);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fmt::println("Invalid use of '{} <data_count> <data_size>'", argv[0]);
    return 1;
  }

  int DATA_COUNT = std::atoi(argv[1]);
  int DATA_SIZE = std::atoi(argv[2]);

  if (DATA_COUNT < 1) {
    fmt::println("Invalid use of '{}': data count must be greater than 0", argv[0]);
    return 1;
  }

  if (DATA_SIZE < 1) {
    fmt::println("Invalid use of '{}': data size must be greater than 0", argv[0]);
    return 1;
  }

  vector<vector<int>> data(DATA_COUNT);
  
  // Set the seed for random numbers to be current time
  srand(std::time(nullptr));
  
  // Generate n data arrays to run sorting algorithms on
  for (int i = 0; i < DATA_COUNT; i++) {
    for (int j = 0; j < DATA_SIZE; j++) {
      data[i].push_back(std::rand() % DATA_SIZE);
    }
  }

  vector<vector<int>> sorted_reversed(DATA_COUNT);

  run_test(data, "Quicksort", quicksort, true);
  run_test(data, "Quicksort (R)", reverse_quicksort, false);
  
  return 0;
}
