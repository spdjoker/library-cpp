#ifndef JKR_SORT_QUICKSORT_HPP
#define JKR_SORT_QUICKSORT_HPP 

#include <vector>

namespace jkr::sort {

template <typename T>
inline unsigned int partition(std::vector<T>& data, int low, int high) {
  int i = low;
  T tmp;
  
  for (int j = low; j < high; j++) {
    if (data[j] < data[high]) {
      tmp = data[j];
      data[j] = data[i];
      data[i] = tmp;
      i++;
    }
  }

  tmp = data[high];
  data[high] = data[i];
  data[i] = tmp;
  return i;
}

template <typename T>
inline void quicksort(std::vector<T>& data, int low, int high) {
  if (low < high) {
    int pi = partition(data, low, high);

    quicksort(data, low, pi-1);
    quicksort(data, pi+1, high);
  }
}

template <typename T>
inline void quicksort(std::vector<T>& data) {
  quicksort(data, 0, data.size() - 1);
}

}

#endif
