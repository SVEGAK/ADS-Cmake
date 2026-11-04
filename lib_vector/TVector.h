// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_VECTOR_TVECTOR_H_
#define LIB_VECTOR_TVECTOR_H_

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "TMemData.h"

template <typename T>
class TVector {
    TMemData<T> _mem;  // хранилище данных + размер + вместимость
    size_t _front;     // индекс первого элемента
    size_t _back;      // индекс последнего элемента

public:
    explicit TVector(size_t size = 0);  // конструктор по размеру
    TVector(std::initializer_list<T>);  // конструктор по списку
    TVector(T* list, size_t size);      // конструктор инициализации
    TVector(const TVector& other);      // конструктор копирования
    TVector(TVector&& other) noexcept;  // конструктор с move
    ~TVector() = default;               // деструктор

    inline bool is_empty() const noexcept;  // проверка на пустоту
    inline bool is_full() const noexcept;   // проверка на переполнение

    inline void compress() noexcept;  // сжатие буфера
    inline void shrink_to_fit();      // освобождение лишней памяти
    inline size_t size() const noexcept;      // геттер размера
    inline size_t capacity() const noexcept;  // геттер вместимости
    inline T front() const;                   // геттер первого элемента
    inline T back() const;                    // геттер последнего элемента
    inline size_t front_pos() const;          // геттер индекса первого
    inline size_t back_pos() const;           // геттер индекса последнего

    inline T& set_front();  // сеттер первого элемента
    inline T& set_back();   // сеттер последнего элемента

    void push_front(T elem);        // вставка элемента в начало
    void push_back(T elem);         // вставка элемента в конец
    void insert(T elem, size_t pos);  // вставка элемента по позиции
    void push_when_empty(T elem);   // вспомогательная функция для вставки
    inline void size_decrease() noexcept;  // уменьшение size
    inline void size_increase() noexcept;  // увеличение size
    void pop_front();               // удаление элемента из начала
    void pop_back();                // удаление элемента из конца
    void erase(size_t pos);         // удаление элемента по позиции
    void shuffle() noexcept;
    void quicksort(int low, int high) noexcept;
    int partition(int low, int high) noexcept;

    TVector& operator=(const TVector& other) noexcept;  // присваивание
    TVector& operator=(TVector&& other) noexcept;  // присваивание move

    const T& operator[](size_t pos) const;  // константный operator[]
    T& operator[](size_t pos);              // operator[]

    friend std::ostream& operator<<(std::ostream& os,
        const TVector<T>& v) {  // вывод
        os << "{ ";
        for (size_t i = 0; i < v.size(); i++) {
            if (i > 0) {
                os << ", ";
            }
            os << v[i];
        }
        os << " }";
        return os;
    }

    friend std::istream& operator>>(std::istream& is,
        TVector<T>& v) {  // ввод
        size_t len = 0;
        is >> len;
        v._mem.set_memory(len);
        for (size_t i = 0; i < len; i++) {
            T temp = T();
            is >> temp;
            v.push_back(temp);
        }
        return is;
    }

    void push_back_n(const T* values, size_t n);   // вставка n в конец
    void push_front_n(const T* values, size_t n);  // вставка n в начало
    void insert_n(size_t pos, const T* values, size_t n);  // вставка n
    void erase_n(size_t pos, size_t n);  // удаление n элементов
};

template <typename T>
inline bool TVector<T>::is_empty() const noexcept {
    return _mem.is_empty();
}

template <typename T>
inline bool TVector<T>::is_full() const noexcept {
    return (_back == (_mem._capacity - 1)) || (_mem.is_full());
}

template <typename T>
inline void TVector<T>::compress() noexcept {
    if (size() <= capacity() / 2) {
        _mem.reset_memory(size(), _front, 0);
        _front = 0;
        _back = size() - 1;
    }
}

template <typename T>
inline void TVector<T>::shrink_to_fit() {
    if (size() != capacity()) {
        _mem.reset_memory(size(), _front, 0, false);
        _front = 0;
        _back = size() - 1;
    }
}

template <typename T>
inline size_t TVector<T>::size() const noexcept {
    return _mem.size();
}

template <typename T>
inline size_t TVector<T>::capacity() const noexcept {
    return _mem.capacity();
}

template <typename T>
inline size_t TVector<T>::front_pos() const {
    return _front;
}

template <typename T>
inline size_t TVector<T>::back_pos() const {
    return _back;
}

template <typename T>
inline T TVector<T>::front() const {
    if (is_empty()) {
        throw std::out_of_range("Buffer ring is empty");
    }
    return (*this)[0];
}

template <typename T>
inline T TVector<T>::back() const {
    if (is_empty()) {
        throw std::out_of_range("Buffer ring is empty");
    }
    return (*this)[(*this).size() - 1];
}

template <typename T>
inline void TVector<T>::size_decrease() noexcept {
    _mem._size--;
}

template <typename T>
inline void TVector<T>::size_increase() noexcept {
    _mem._size++;
}

template <typename T>
inline T& TVector<T>::set_front() {
    if (is_empty()) {
        throw std::out_of_range(
            "Vector::front() called on empty vector");
    }
    return _mem._data[_front];
}

template <typename T>
inline T& TVector<T>::set_back() {
    if (is_empty()) {
        throw std::out_of_range(
            "Vector::back() called on empty vector");
    }
    return _mem._data[_back];
}

template <typename T>
TVector<T>::TVector(size_t size)
    : _mem(size), _front(0), _back((size > 0) ? size - 1 : 0) {
}

template <typename T>
TVector<T>::TVector(std::initializer_list<T> list) : _mem(list) {
    _front = 0;
    _back = (list.size() > 0) ? size() - 1 : 0;
}

template <typename T>
TVector<T>::TVector(T* list, size_t size) {
    TMemData<T> mem(list, size);
    _front = 0;
    _mem = mem;
    if (size == 0) {
        _back = size;
    }
    else {
        _back = size - 1;
    }
}

template <typename T>
TVector<T>::TVector(const TVector& other) {
    _front = other._front;
    _back = other._back;
    TMemData<T> mem(other._mem);
    _mem = mem;
}

template <typename T>
TVector<T>::TVector(TVector&& other) noexcept {
    _front = other._front;
    _back = other._back;
    _mem = std::move(other._mem);
    other._front = 0;
    other._back = 0;
}

template <typename T>
TVector<T>& TVector<T>::operator=(const TVector& other) noexcept {
    _front = other._front;
    _back = other._back;
    _mem = other._mem;
    return *this;
}

template <typename T>
TVector<T>& TVector<T>::operator=(TVector&& other) noexcept {
    _front = other._front;
    _back = other._back;
    _mem = std::move(other._mem);
    other._front = 0;
    other._back = 0;
    return *this;
}

template <typename T>
const T& TVector<T>::operator[](size_t pos) const {
    if (this->is_empty()) {
        throw std::logic_error(
            "Can't apply [] operator to empty vector: size = 0.");
    }
    pos = (pos + _front) % capacity();
    return _mem._data[pos];
}

template <typename T>
T& TVector<T>::operator[](size_t pos) {
    if (this->is_empty()) {
        throw std::logic_error(
            "Can't apply [] operator to empty vector: size = 0.");
    }
    pos = (pos + _front) % capacity();
    return _mem._data[pos];
}

template <typename T>
void TVector<T>::push_back(T elem) {
    if ((size() == 0) || is_empty()) {  // объект пуст или nullptr
        push_when_empty(elem);
        return;
    }
    if ((_back == (capacity() - 1)) && !(_mem.is_full())) {
        // просто сдвигаем массив влево, если есть свободное место
        _mem.reset_memory(size() + 1, _front, 0);
        _front = 0;
        _back = (_mem._size > 0) ? _front + size() - 1 : 0;
        _mem._data[_back] = elem;
        return;
    }
    else if (is_full()) {
        _mem.reset_memory(size() + 1, _front);
        _front = 0;
        _back = (_mem._size > 0) ? _front + size() - 1 : 0;
        _mem._data[_back] = elem;
        return;
    }
    _back++;
    _mem._data[_back] = elem;
    size_increase();
}

template <typename T>
void TVector<T>::insert(T elem, size_t pos) {
    if (pos > size()) {
        throw std::out_of_range(
            "Insert position must be in [0;" + std::to_string(size()) +
            "].");
    }

    if (is_empty() || size() == 0) {
        push_when_empty(elem);
        return;
    }

    if (is_full()) {
        _mem.reset_memory(size() + 1, _front);
        _front = 0;
        _back = (_mem._size > 0) ? _front + size() - 1 : 0;
    }
    else {
        size_increase();
        _back++;
    }

    size_t phys_pos = (pos + _front) % capacity();
    for (size_t i = _back; i > phys_pos; i--) {
        _mem._data[i] = _mem._data[i - 1];
    }
    _mem._data[phys_pos] = elem;
}

template <typename T>
void TVector<T>::push_front(T elem) {
    if ((size() == 0) || is_empty()) {  // объект пуст
        push_when_empty(elem);
        return;
    }
    if (is_full() || (_front == _back) || (_front == 0)) {
        // увеличиваем на 1 ячейку size + выделяем буфер
        _mem.reset_memory(size() + 1, _front, 1);
        _front = 0;
        _back = (_mem._size > 0) ? _front + size() - 1 : 0;
        _mem._data[_front] = std::move(elem);
        return;
    }
    _front--;
    _mem._data[_front] = std::move(elem);
    size_increase();
}

template <typename T>
void TVector<T>::push_when_empty(T elem) {
    _front = 0;
    _back = 0;
    _mem.set_memory(1);
    _mem._data[_front] = std::move(elem);
    size_increase();
}

template <typename T>
void TVector<T>::pop_front() {
    if ((_mem.size() == 0) || is_empty()) {  // объект пуст или nullptr
        throw std::out_of_range("Can't pop empty list.");
    }
    (*this)[0] = T();
    _front = (_front + 1) % capacity();
    size_decrease();
    compress();
}

template <typename T>
void TVector<T>::pop_back() {
    if ((size() == 0) || is_empty()) {  // объект пуст или nullptr
        throw std::out_of_range("Can't pop empty list.");
    }
    if (_back == 0) {
        _mem._data[_back] = T();
        _back = 0;
        size_decrease();
        compress();
        return;
    }
    else if (_back < 0) {
        throw std::out_of_range("back pos element out of massive.");
    }
    _mem._data[_back] = T();
    _back--;
    size_decrease();
    compress();
}

template <typename T>
void TVector<T>::erase(size_t pos) {  // pos - логический индекс
    if (pos >= size()) {  // size_t беззнаковый тип
        throw std::out_of_range(
            "Pop position must be in [" + std::to_string(0) + ';' +
            std::to_string(size() - 1) + "].");
    }
    if (is_empty()) {  // учитываем, если буфер пуст
        throw std::out_of_range("Can't pop empty list.");
    }
    if (pos == 0) {
        pop_front();
        return;
    }
    else if (pos == (_mem._size - 1)) {
        pop_back();
        return;
    }
    for (size_t i = pos; i < size() - 1; i++) {  // сдвиг влево
        (*this)[i] = (*this)[i + 1];
    }
    _mem.reset_memory(_mem._size - 1, _front, 0);
    _front = 0;
    _back = _front + size() - 1;
}

template <typename T>
void TVector<T>::shuffle() noexcept {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    // Алгоритм Фишера-Йетса
    if (is_empty()) {
        return;
    }
    T t;
    for (size_t i = size() - 1; i > 0; i--) {
        size_t j = std::rand() % (i + 1);
        t = (*this)[j];
        (*this)[j] = (*this)[i];
        (*this)[i] = t;
    }
}

template <typename T>
void TVector<T>::quicksort(int low, int high) noexcept {
    if (low < high) {
        int p = partition(high, low);
        quicksort(low, p);
        quicksort(p + 1, high);
    }
}

template <typename T>
int TVector<T>::partition(int high, int low) noexcept {
    // опорный элемент [low; high]
    int pivot = (*this)[std::rand() % (high - low + 1) + low];
    low--;
    high++;
    // чтобы начать с начала и использовать при этом do while
    while (true) {
        do {
            low++;
        } while ((*this)[low] < pivot);
        do {
            high--;
        } while ((*this)[high] > pivot);
        if (low >= high) {
            return high;
        }
        T k = (*this)[low];
        (*this)[low] = (*this)[high];
        (*this)[high] = k;
    }
}

template <typename T>
void TVector<T>::push_back_n(const T* values, size_t n) {
    if (n == 0) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        push_back(values[i]);
    }
}

template <typename T>
void TVector<T>::push_front_n(const T* values, size_t n) {
    if (n == 0) {
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (i == 0) {
            push_front(values[0]);
            continue;
        }
        insert(values[i], i);
    }
}

template <typename T>
void TVector<T>::insert_n(size_t pos, const T* values, size_t n) {
    if (n == 0) {
        return;
    }
    if (pos == 0) {
        push_front_n(values, n);
        return;
    }
    if (pos == size()) {
        push_back_n(values, n);
        return;
    }
    for (size_t i = 0; i < n; i++) {
        insert(values[i], pos + i);
    }
}

template <typename T>
void TVector<T>::erase_n(size_t pos, size_t n) {
    if (n == 0) {
        return;
    }
    if (pos + n > size()) {
        throw std::out_of_range("erase_n range out of bounds");
    }
    for (size_t i = 0; i < n; i++) {
        erase(pos);
    }
}

#endif  // LIB_VECTOR_TVECTOR_H_