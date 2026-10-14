// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_VECTOR_TMEMDATA_H_
#define LIB_VECTOR_TMEMDATA_H_

#include <initializer_list>

#define MEM_STEP 15
#define FRONT_BUFFER 5

template <typename T>
class TVector;

template <typename T>
class TMemData {
    T* _data;          // хранилище данных
    size_t _size;      // размер заполненной части хранилища
    size_t _capacity;  // вместимость хранилища

public:
    explicit TMemData(size_t size = 0);        // конструктор по размеру
    TMemData(std::initializer_list<T> list);   // конструктор по списку
    TMemData(T* data, size_t size);            // конструктор инициализации
    TMemData(const TMemData& other);           // конструктор копирования
    TMemData(TMemData&& other) noexcept;       // конструктор с move
    ~TMemData();                               // деструктор

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
        size_t placement_offset = 0,
        bool cap_calculation = true) noexcept;
    inline void clear_memory() noexcept;  // очистка памяти

    TMemData& operator=(const TMemData& other);      // присваивание
    TMemData& operator=(TMemData&& other) noexcept;  // присваивание move

    friend class TVector<T>;
};

template <typename T>
inline bool TMemData<T>::is_empty() const noexcept {
    return _size == 0 || _data == nullptr;
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
}

template <typename T>
inline size_t TMemData<T>::calculate_capacity(size_t size) {
    if ((size <= _size) && (size > 0)) {
        return size;
    }
    if (size <= MEM_STEP) {
        return MEM_STEP;
    }
    return (size + MEM_STEP);
}

template <typename T>
TMemData<T>::TMemData(size_t size) {
    _capacity = calculate_capacity(size);
    _size = 0;
    _data = new T[_capacity];
}

template <typename T>
TMemData<T>::TMemData(std::initializer_list<T> list) {
    _size = list.size();
    _capacity = _size;
    _data = new T[_capacity];
    const T* list_begin_p = list.begin();
    for (size_t i = 0; i < _size; i++) {
        _data[i] = *(list_begin_p + i);
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
TMemData<T>::TMemData(const TMemData& TMemData) {
    _size = TMemData._size;
    _capacity = TMemData._capacity;
    _data = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
        _data[i] = TMemData._data[i];
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
}

// start_index - индекс начала элементов в старом массиве
template <typename T>
void TMemData<T>::reset_memory(size_t size, size_t start_index,
    size_t placement_offset,
    bool cap_calculation) noexcept {
    T* old_data = _data;
    _capacity = (cap_calculation) ? calculate_capacity(size) : size;
    _data = new T[_capacity];
    if (size > _size) {
        for (size_t i = 0; i < _size; i++) {
            _data[i + placement_offset] = old_data[i + start_index];
        }
    }
    else {  // size <= _size
        for (size_t i = 0; i < size; i++) {
            _data[i + placement_offset] = old_data[i + start_index];
        }
    }
    _size = size;
    delete[] old_data;
    return;
}

template <typename T>
TMemData<T>& TMemData<T>::operator=(const TMemData& other) {
    if (this != &other) {
        (*this)._size = other._size;
        (*this)._capacity = other._capacity;
        (*this)._data = new T[(*this)._capacity];
        for (size_t i = 0; i < _capacity; i++) {
            (*this)._data[i] = other._data[i];
        }
    }
    return (*this);
}

template <typename T>
TMemData<T>& TMemData<T>::operator=(TMemData&& other) noexcept {
    if (this != &other) {
        (*this)._size = other._size;
        (*this)._capacity = other._capacity;
        (*this)._data = other._data;
        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
    }
    return (*this);
}

#endif  // LIB_VECTOR_TMEMDATA_H_