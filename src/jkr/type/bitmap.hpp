#ifndef JKR_TYPE_BITMAP_HPP
#define JKR_TYPE_BITMAP_HPP

namespace jkr::type {

class bitmap {
  // Represents the size in bytes of a character type on any architecture.
  constexpr static int CHAR_SIZE_BYTES = sizeof(unsigned char);
  // Represents the size in bits of a character type on any architecture.
  constexpr static int CHAR_SIZE_BITS = CHAR_SIZE_BYTES * 8;
  // Represents a full character on any architecture, 0xFF | 0xFFFF.
  constexpr static unsigned char CHAR_FULL = ~((char)0);

  // The number of bits that represent the usable size of the data.
  int m_size_bits;
  // The size of the data array, independent of architecture char size.
  int m_char_count;
  // The array containing the bitmap, with bits going right to left. 
  unsigned char *data;

public:
  /** @brief Constructs a bitmap with n bits starting as 0 or 1.
   *  
   *  Initializes a bitmap using a number of bits to calculate
   *  the byte size and therefore character count. It defaults
   *  the bit size of the bitmap to 0, while also initializing
   *  each bit to 0 unless set is changed to true by setting 
   *  entire bytes at a time. 
   *
   *  Time Complexity: O(m_char_count)
   *  
   *  @param bit_size the number of bits to initialize. 
   *  @param set the starting state of the bits. 
   */
  bitmap(int bit_size = 0, bool set = false);
  /** @brief Deallocates the bitmap's data.
   *  
   *  Time Complexity: O(m_char_count)
   */
  ~bitmap() { delete[] data; }

  /** @brief Sets the ith bit in the bitmap to 1.
   *  
   *  If the index of the bit to set is greater than the
   *  number of bits contained in the bitmap, an assertion
   *  will be thrown to make the issue known. Otherwise, the
   *  ith bit will be set to 1 regardless of what it was.
   *
   *  Time Complexity: O(1)
   *  
   *  @param i the bit to set
   */
  void set(int i); 
  /** @brief Sets all the bits within the bitmap to 1.
   *  
   *  Iterates through each byte in the bitmap, setting them
   *  equal to {CHAR_FULL}. This will affect the remaining 
   *  bits of the bitmap, obtained using {bits % CHAR_SIZE}.
   *  However, those bits do not affect the use of the 
   *  bitmap and therefore addressing this would be more
   *  time consuming than useful.
   *
   *  Time Complexity: O(m_char_count)
   */
  void set();
  /** @brief Sets the ith bit in the bitmap to 0.
   *  
   *  If the index of the bit to set is greater than the
   *  number of bits contained in the bitmap, an assertion
   *  will be thrown to make the issue known. Otherwise, the
   *  ith bit will be set to 0 regardless of what it was.
   *
   *  Time Complexity: O(1)
   *  
   *  @param i the bit to set
   */
  void clear(int i);
  /** @brief Sets all the bits within the bitmap to 0.
   *  
   *  Iterates through each byte in the bitmap, setting them
   *  equal to 0. This will affect the remaining bits of the 
   *  bitmap, obtained using {bits % CHAR_SIZE}. However, 
   *  those bits do not affect the use of the bitmap and 
   *  therefore addressing this would be more time consuming 
   *  than useful.
   *
   *  Time Complexity: O(m_char_count)
   */
  void clear();

  /** @brief Tries to set the ith bit in the bitmap to 1.
   *  
   *  If the index of the bit to set is greater than the
   *  number of bits contained in the bitmap, an assertion
   *  will be thrown to make the issue known. If the
   *  ith bit is already 1, this function will return false,
   *  otherwise it will set the bit to 1 and return true.
   *
   *  Time Complexity: O(1)
   *  
   *  @param i the bit to try to set.
   *  @return Whether or not the bit was set.
   */
  bool try_set(int i);
  /** @brief Tries to set the ith bit in the bitmap to 0.
   *  
   *  If the index of the bit to clear is greater than the
   *  number of bits contained in the bitmap, an assertion
   *  will be thrown to make the issue known. If the
   *  ith bit is already 0, this function will return false,
   *  otherwise it will set the bit to 0 and return true.
   *
   *  Time Complexity: O(1)
   *  
   *  @param i the bit to try to set.
   *  @return Whether or not the bit was set.
   */
  bool try_clear(int i);
  
  /** @brief Returns the first bit that is 0 and sets it.
   *  
   *  Iterates through the bitmap looking for an unset bit,
   *  and if one is found it will set it and return its 
   *  index. If none is found, then this function will
   *  return -1.
   *
   *  Time Complexity: O(m_char_count)
   *  
   *  @return The index of the claimed bit.
   */
  int find();

  /** @brief Adds another bit to the end of the bitmap.
   *  
   *  Increments the size of the bitmap and assigns the 
   *  new bit a value equal to set. If the bitmap has no 
   *  capacity for a new bit, the capacity of the bitmap 
   *  doubles using reserve to accomodate the extra bit.
   *  This takes inspiration from std::vector.
   *
   *  AC Time Complexity: O(1);
   *  WC Time Complexity: O(m_char_count);
   */
  void push_back(bool set);
  /** @brief Removes the last bit in the bitmap.
   *  
   *  Decrements the bit size of the bitmap by one to mask
   *  the last bit, therefore reducing the size of the 
   *  bitmap. Does nothing if the size of the bitmap is 
   *  already 0.
   *
   *  Time Complexity: O(1)
   */
  void pop_back();
  /** @brief Changes the maximum number of bits available.
   *  
   *  Creates a new bitmap of the new size and attempts to 
   *  move the current bitmap data into it. If the bitmap 
   *  already has the capacity to accomodate the new bits,
   *  then the size is just increased to fit.
   *
   *  Time Complexity: O(char_count)
   */
  void resize(int num_bits);
  /** @brief Increases the total capacity of the bitmap.
   *  
   *  Allocates n bytes if the current byte size of the 
   *  bitmap is less than the requested amount, but does
   *  not change the bit size of the bitmap.
   *
   *  Time Complexity: O(char_count)
   */
  void reserve(int bytes);

  /** @brief Returns whether the ith bit is 1 or not.
   *  
   *  If the index of the bit to clear is greater than the
   *  number of bits contained in the bitmap, an assertion
   *  will be thrown to make the issue known. If the ith bit 
   *  is 1, this function will return true, otherwise it 
   *  will return false.
   *
   *  Time Complexity: O(1)
   *  
   *  @param i the bit to test.
   *  @return Whether or not the bit is set.
   */
  bool test(int i) const;
#ifdef JKR_DEBUG_PRINT
  /** @brief Prints the entire bitmap as 1s and 0s.
   *  
   *  Iterates through the bitmap, printing each individual
   *  bit as a 0 or a 1, and any unused bits are hidden. 
   *
   *  Time Complexity: O(m_size_bits)
   */
  void print() const;
#endif

  bool front() const { 
    return m_size_bits == 0 
      ? false 
      : data[0] & 1; 
  }

  bool back() const { 
    return m_size_bits == 0 
      ? false 
      : data[(m_size_bits - 1) / CHAR_SIZE_BITS] & (1 << ((m_size_bits - 1) % CHAR_SIZE_BITS)); 
  }
  int size_bits() const { return m_size_bits; }
  int size_bytes() const { return bits_to_chars(m_size_bits) * CHAR_SIZE_BYTES; }
  int capacity_bits() const { return m_char_count * CHAR_SIZE_BITS; }
  int capacity_bytes() const { return m_char_count * CHAR_SIZE_BYTES; }

  static int bits_to_chars(int count);
};

}

#endif
