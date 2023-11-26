#include "jkr/sort/quicksort.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fmt/core.h>
#include <stdlib.h>
#include <vector>

using namespace jkr::sort;

template <typename T> using vector = std::vector<T>;
using algorithm = void(*)(vector<int>&, sort_order order);

void run_test(vector<vector<int>> data, sort_order order, std::string algorithm_name, algorithm sorting_algo) {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
  vector<double> times(data.size());
  
  for (unsigned int i = 0; i < data.size(); i++) {
    start_time = std::chrono::high_resolution_clock::now();
    sorting_algo(data[i], order);
    end_time = std::chrono::high_resolution_clock::now();
    times[i] = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
  }

  double avg_time_ms = 0.0;
  double min_time_ms = times[0];
  double max_time_ms = times[0];

  for (double ms : times) {
    avg_time_ms += ms;
    if (ms < min_time_ms) min_time_ms = ms;
    if (ms > max_time_ms) max_time_ms = ms;
  }

  avg_time_ms /= data.size();

  fmt::println("\e[32m{} ({})\e[0m\t[ Min: {:8.2f} ms | Avg: {:8.2f} ms | Max: {:8.2f} ms ]", 
               algorithm_name, order == sort_order::ASCENDING ? 'A' : 'D', 
               min_time_ms, avg_time_ms, max_time_ms);
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

  run_test(data, sort_order::ASCENDING, "Quicksort", quicksort);
  run_test(data, sort_order::DESCENDING, "Quicksort", quicksort);
  
  return 0;
}
