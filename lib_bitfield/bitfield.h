// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_BITFIELD
#define LIB_BITFIELD

#include <algorithm>
#include <climits>  //Для CHAR_BITS
#include <cstddef>
#include <iostream>
namespace {

constexpr size_t kBlockLength =
    sizeof(unsigned int) * CHAR_BIT;  // Кол-во битов в 1 блоке _data[i]

}
class BitField {
  unsigned int* _data;
  size_t _size;  // max мощность множества (количество элементов = кол-во блоков
                 // * sizeof(каждого блока)
  // Т.е. _size = sizeof(unsigned int) * CHAR_BITS *blocks

  unsigned int get_mask(size_t pos);   // Получение маски
  size_t get_index(size_t pos) const;  // Преобразование позиции в индекс
  size_t blocks(size_t bits) const;  // Динамическая функция вычисления числа
                                     // блоков (фактически длины массива)

 public:
  BitField(size_t size = 0);
  BitField(const BitField& other);
  BitField(BitField&& other) noexcept;
  ~BitField();

  void set_bit(size_t pos);  // pos по контракту это index
  void reset_bit(size_t pos);
  bool get_bit(size_t pos) const;

  size_t size() const noexcept;  // Геттер мощности множества

  BitField& operator=(const BitField& other);
  BitField& operator=(BitField&& other) noexcept;

  // побитовый сдвиг
  BitField operator<<(size_t n) const;
  BitField operator>>(size_t n) const;

  // побитовые операторы
  BitField operator&(const BitField& other) const;
  BitField operator|(const BitField& other) const;
  BitField operator~() const;
  BitField& operator&=(const BitField& other);
  BitField& operator|=(const BitField& other);

  friend std::ostream& operator<<(std::ostream& os, const BitField& bf);
  friend std::istream& operator>>(std::istream& is, BitField& bf);
};

#endif  // LIB_BITFIELD