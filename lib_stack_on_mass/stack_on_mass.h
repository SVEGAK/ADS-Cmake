// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_STACK_ON_MASS_STACK_ON_MASS_H_
#define LIB_STACK_ON_MASS_STACK_ON_MASS_H_

#include <exception>

template <typename T>
class TStackOnMass {
  T* _data;
  size_t _size;
  int _top;  // Индекс последнего элемента
 public:
  explicit TStackOnMass(size_t size);
  TStackOnMass(const TStackOnMass& other);
  ~TStackOnMass();

  void push(const T&);
  T pop();
  T& top();
  T& bottom();  // Для QueueOnStacks
  size_t size() const noexcept;
  const T& top() const;
  const T& bottom() const;
  inline bool is_empty() const noexcept;
  inline bool is_full() const noexcept;
};

template <class T>
inline bool TStackOnMass<T>::is_empty() const noexcept {
  return _top == -1;
}

template <class T>
inline bool TStackOnMass<T>::is_full() const noexcept {
  return _top == _size - 1;
}

template <class T>
TStackOnMass<T>::TStackOnMass(size_t size) : _size(size), _top(-1) {
  if (size == 0) {
    throw std::invalid_argument("Stack size must be greater than 0");
  }
  _data = new T[size];
}

template <typename T>
inline TStackOnMass<T>::TStackOnMass(const TStackOnMass& other)
    : _size(other._size), _top(other._top) {
  _data = new T[_size];
  for (int i = 0; i <= _top; ++i) {
    _data[i] = other._data[i];
  }
}

template <typename T>
inline TStackOnMass<T>::~TStackOnMass() {
  delete[] _data;
}

template <typename T>
inline void TStackOnMass<T>::push(const T& val) {
  if (is_full()) {
    throw std::logic_error("cannot push in full stack");
  }
  _data[++_top] = val;
}

template <typename T>
inline T TStackOnMass<T>::pop() {
  if (is_empty()) {
    throw std::logic_error("cannot pop from empty stack");
  }
  return _data[_top--];
}

template <typename T>
inline T& TStackOnMass<T>::top() {
  if (is_empty()) {
    throw std::logic_error("Stack is empty");
  }
  return _data[_top];
}

template <typename T>
inline T& TStackOnMass<T>::bottom() {
  if (is_empty()) {
    throw std::logic_error("Stack is empty");
  }
  return _data[0];
}

template <typename T>
inline const T& TStackOnMass<T>::bottom() const {
  if (is_empty()) {
    throw std::logic_error("Stack is empty");
  }
  return _data[0];
}

template <typename T>
inline size_t TStackOnMass<T>::size() const noexcept {
  return static_cast<size_t>(_top + 1);
}

template <typename T>
inline const T& TStackOnMass<T>::top() const {
  if (is_empty()) {
    throw std::logic_error("Stack is empty");
  }
  return _data[_top];
}

#endif  // LIB_STACK_ON_MASS_STACK_ON_MASS_H_
