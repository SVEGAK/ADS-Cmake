// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_VECTOR_TMEMDATA_H_
#define LIB_VECTOR_TMEMDATA_H_

#include <cstddef>
#include <initializer_list>

#define MEM_STEP 15

template <typename T>
class TVector;

template <typename T>
class TMemData {
  T* _data;          // хранилище данных
  size_t _size;      // размер заполненной части хранилища
  size_t _capacity;  // вместимость хранилища

 public:
  TMemData(size_t size = 0);                // конструктор по размеру
  TMemData(std::initializer_list<T> list);  // конструктор по списку
  TMemData(T* data, size_t size);           // конструктор инициализации
  TMemData(const TMemData& other);          // конструктор копирования
  TMemData(TMemData&& other) noexcept;      // конструктор с move
  ~TMemData();                              // деструктор

  inline bool is_empty() const noexcept;  // проверка на пустоту
  inline bool is_full() const noexcept;   // проверка на переполнение
  inline size_t calculate_capacity(size_t size);

  inline size_t size() const noexcept;          // геттер размера
  inline size_t capacity() const noexcept;      // геттер вместимости
  inline const T* const data() const noexcept;  // геттер хранилища

  // установка памяти без сохранения данных
  void set_memory(size_t size) noexcept;
  // перевыделение памяти с сохранением данных
  void reset_memory(size_t size, size_t start_index = 0,
                    size_t placement_offset = 0, bool cap_calculation = true);
  inline void clear_memory() noexcept;  // очистка памяти

  TMemData& operator=(const TMemData& other);      // присваивание
  TMemData& operator=(TMemData&& other) noexcept;  // присваивание move

  friend class TVector<T>;
};

template <typename T>
inline bool TMemData<T>::is_empty() const noexcept {
  return _size == 0;
}

template <typename T>
inline bool TMemData<T>::is_full() const noexcept {
  return _size == _capacity && _size > 0;
}

template <typename T>
inline size_t TMemData<T>::size() const noexcept {
  return _size;
}

template <typename T>
inline size_t TMemData<T>::capacity() const noexcept {
  return _capacity;
}

template <typename T>
inline const T* const TMemData<T>::data() const noexcept {
  return _data;
}

// решил сделать inline, т.к. функция достаточно маленькая
template <typename T>
inline void TMemData<T>::clear_memory() noexcept {
  delete[] _data;
  _data = nullptr;
  _size = 0;
  _capacity = 0;
}

template <typename T>
inline size_t TMemData<T>::calculate_capacity(size_t size) {
  if ((size <= _size) && (size > 0)) {
    return size;
  }
  if (size <= MEM_STEP) {
    return MEM_STEP;
  }
  return size + MEM_STEP;
}

template <typename T>
TMemData<T>::TMemData(size_t size) {
  _size = size;
  _capacity = calculate_capacity(size);
  _data = new T[_capacity];
  for (size_t i = 0; i < _capacity; ++i) {
    _data[i] = T();  // значение по умолчанию
  }
}

template <typename T>
TMemData<T>::TMemData(std::initializer_list<T> list) {
  _size = list.size();
  _capacity = _size;
  _data = new T[_size];
  const T* list_begin_p = list.begin();
  for (size_t i = 0; i < _size; i++) {
    _data[i] = list_begin_p[i];
  }
}

template <typename T>
TMemData<T>::TMemData(T* data, size_t size) {
  _size = size;
  _capacity = calculate_capacity(_size);
  _data = new T[_capacity];
  for (size_t i = 0; i < size; i++) {
    _data[i] = data[i];
  }
}

template <typename T>
TMemData<T>::TMemData(const TMemData& other) {
  _size = other._size;
  _capacity = other._capacity;
  _data = new T[_capacity];
  for (size_t i = 0; i < _size; i++) {
    _data[i] = other._data[i];
  }
}

template <typename T>
TMemData<T>::TMemData(TMemData&& other) noexcept {
  _size = other._size;
  _capacity = other._capacity;
  _data = other._data;
  other._data = nullptr;
  other._size = 0;
  other._capacity = 0;
}

template <typename T>
TMemData<T>::~TMemData() {
  if (_data) {
    delete[] _data;
  }
}

template <typename T>
void TMemData<T>::set_memory(size_t size) noexcept {
  _size = 0;
  _capacity = calculate_capacity(size);
  delete[] _data;
  _data = new T[_capacity];
  for (size_t i = 0; i < _capacity; i++) {
    _data[i] = T();
  }
}

template <typename T>
void TMemData<T>::reset_memory(size_t size, size_t start_index,
                               size_t placement_offset, bool cap_calculation) {
  T* old_data = _data;
  size_t old_capacity = _capacity;
  size_t old_size = _size;

  _capacity = cap_calculation ? calculate_capacity(size) : size;
  _data = new T[_capacity];
  for (size_t i = 0; i < _capacity; ++i) {
    _data[i] = T();
  }

  size_t count = (size > old_size) ? old_size : size;
  if (count > 0 && old_capacity > 0) {
    for (size_t i = 0; i < count; ++i) {
      size_t src = (start_index + i) % old_capacity;  // обход кольца
      _data[i + placement_offset] = old_data[src];
    }
  }

  _size = size;
  delete[] old_data;
}

template <typename T>
TMemData<T>& TMemData<T>::operator=(const TMemData& other) {
  if (this != &other) {
    delete[] _data;
    _size = other._size;
    _capacity = other._capacity;
    _data = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
      _data[i] = other._data[i];
    }
  }
  return *this;
}

template <typename T>
TMemData<T>& TMemData<T>::operator=(TMemData&& other) noexcept {
  if (this != &other) {
    _size = other._size;
    _capacity = other._capacity;
    _data = other._data;
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
  }
  return *this;
}

#endif  // LIB_VECTOR_TMEMDATA_H_
