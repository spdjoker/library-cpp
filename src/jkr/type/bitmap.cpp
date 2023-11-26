#include "bitmap.hpp"

#include <cassert>

#include "fmt/core.h"


namespace jkr::type {

bitmap::bitmap(int num_bits, bool set) 
  : m_size_bits(num_bits)
  , m_char_count(bits_to_chars(num_bits))
  , data(new unsigned char[m_char_count]) {
  assert(num_bits >= 0);

  if (set) this->set();
}

void bitmap::set(int i) { 
  assert(i < m_size_bits);
  data[i / CHAR_SIZE_BITS] |= 1 << (i % CHAR_SIZE_BITS);
}

void bitmap::set() {
  int char_count = bits_to_chars(m_size_bits); // Full bytes
  for (int b = 0; b < char_count; b++)
    data[b] = CHAR_FULL;
}

void bitmap::clear(int i) { 
  assert(i < m_size_bits);
  data[i / CHAR_SIZE_BITS] &= ~(1 << (i % CHAR_SIZE_BITS));
}

void bitmap::clear() {
  int char_count = bits_to_chars(m_size_bits);
  for (int b = 0; b < char_count; b++)
    data[b] = 0;
}

bool bitmap::try_set(int i) {
  assert(i < m_size_bits);
  if (data[i / CHAR_SIZE_BITS] & (1 << (i % CHAR_SIZE_BITS)))
    return false;
  data[i / CHAR_SIZE_BITS] |= 1 << (i % CHAR_SIZE_BITS);
  return true;
}

bool bitmap::try_clear(int i) {
  assert(i < m_size_bits);
  if (data[i / CHAR_SIZE_BITS] & (1 << (i % CHAR_SIZE_BITS))) {
    data[i / CHAR_SIZE_BITS] &= ~(1 << (i % CHAR_SIZE_BITS));
    return true;
  }
  return false;
}

int bitmap::find() {
  int offset = m_size_bits / CHAR_SIZE_BITS, b = 0, i;
  // Checks all bytes with CHAR_SIZE bits
  for (; b < offset; b++) {
    if (data[b] != CHAR_FULL) {
      for (i = 0; i < CHAR_SIZE_BITS; i++) {
        if (!(data[b] & (1 << i))) {
          set(b * 8 + i);
          return b * 8 + i;
        }
      }  
    }
  }
  
  // Checks last byte if it is not CHAR_SIZE bits
  offset = m_size_bits % CHAR_SIZE_BITS;
  if (offset) {
    // Exit early if the half byte is full.
    if (data[b] == CHAR_FULL) goto fail;
    for (i = 0; i < offset; i++) {
      if (!(data[b] & (1 << i))) {
        set(b * 8 + i);
        return b * 8 + i;
      }
    }  
  }

fail:
  return -1;
}

void bitmap::push_back(bool state) {
  if (m_size_bits == m_char_count * CHAR_SIZE_BITS)
    reserve(2 * m_char_count * CHAR_SIZE_BYTES);
  
  if (state)
    data[m_size_bits / CHAR_SIZE_BITS] |= (1 << m_size_bits % CHAR_SIZE_BITS);
  else
    data[m_size_bits / CHAR_SIZE_BITS] &= ~(1 << m_size_bits % CHAR_SIZE_BITS);
  m_size_bits++;
}

void bitmap::pop_back() {
  if (m_size_bits == 0) return;
  m_size_bits--;
}

void bitmap::resize(int num_bits) {
  assert(num_bits >= 0);
  int char_count = bits_to_chars(num_bits);
  unsigned char *copy = new unsigned char[char_count];

  int size = char_count < m_char_count ? char_count : m_char_count;
  m_char_count = char_count;
  m_size_bits = num_bits;

  for (int b = 0; b < size; b++)
    copy[b] = data[b];

  delete[] data;
  data = copy;
}

void bitmap::reserve(int bytes) {
  if (m_char_count * CHAR_SIZE_BYTES >= bytes) return;
  
  int char_count = bytes / CHAR_SIZE_BYTES + (bytes % CHAR_SIZE_BYTES ? 1 : 0);
  unsigned char *copy = new unsigned char[char_count] {0};

  for (int b = 0; b < m_char_count; b++)
    copy[b] = data[b];

  m_char_count = char_count;
  delete[] data;
  data = copy;
}

bool bitmap::test(int i) const { 
  assert(i < m_size_bits);
  return data[i / CHAR_SIZE_BITS] & (1 << (i % CHAR_SIZE_BITS));
}

#ifdef JKR_DEBUG_PRINT
void bitmap::print() const {
  int offset = m_size_bits / CHAR_SIZE_BITS, b = 0, i;
  fmt::print("[SIZE: {} bits, CAPACITY: {} bytes] -> {{", m_size_bits, capacity_bytes());
  for (; b < offset; b++) {
    for (i = 0; i < CHAR_SIZE_BITS; i++) {
      if (i % 8 == 0)
        fmt::print(" ");
      fmt::print("{}", (data[b] >> i) & 1);
    }
  }
  offset = m_size_bits % CHAR_SIZE_BITS;
  if (offset) {
    for (i = 0; i < offset; i++) {
      if (i % 8 == 0)
        fmt::print(" ");
      fmt::print("{}", (data[b] >> i) & 1);
    }
  }
  fmt::print(" }}\n");
}
#endif

int bitmap::bits_to_chars(int count) { return count / CHAR_SIZE_BITS + (count % CHAR_SIZE_BITS ? 1 : 0); }

}
