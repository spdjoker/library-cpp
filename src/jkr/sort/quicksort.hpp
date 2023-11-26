#ifndef JKR_SORT_QUICKSORT_HPP
#define JKR_SORT_QUICKSORT_HPP 

#include <vector>
#include <fmt/core.h>

namespace jkr::sort {

enum class sort_order { ASCENDING, DESCENDING };

template <typename T>
using COMPARATOR = bool (*)(const T&, const T&);

template <typename T>
inline unsigned int partition(std::vector<T>& data, int low, int high, sort_order order) {
  int i = low;
  T tmp;
  
  if (order == sort_order::ASCENDING) {
    for (int j = low; j < high; j++) {
      if (data[j] < data[high]) {
        tmp = data[j];
        data[j] = data[i];
        data[i] = tmp;
        i++;
      }
    }
  } else {
    for (int j = low; j < high; j++) {
      if (data[j] > data[high]) {
        tmp = data[j];
        data[j] = data[i];
        data[i] = tmp;
        i++;
      }
    }
  }

  tmp = data[high];
  data[high] = data[i];
  data[i] = tmp;
  return i;
}

template <typename T>
inline void quicksort(std::vector<T>& data, int low, int high, sort_order order) {
  if (low < high) {
    int pi = partition(data, low, high, order);

    quicksort(data, low, pi-1, order);
    quicksort(data, pi+1, high, order);
  }
}

template <typename T>
inline void quicksort(std::vector<T>& data, sort_order order) {
  quicksort(data, 0, data.size() - 1, order);
}

}

#endif
