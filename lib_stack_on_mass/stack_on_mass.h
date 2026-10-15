// Copyright 2026 SVEGAK/Novikov R

#include <exception>
template <typename T>
class TStackOnMass {
	T* _data;
	size_t _size;
	size_t _top;
 public:
	 TStackOnMass(size_t size);
	 TStackOnMass(const TStackOnMass& other);
	 ~TStackOnMass();
	 
	 void push(const T&);
	 T pop();
	 T& top();
	 inline bool is_empty() const noexcept;
	 inline bool is_full() const noexcept;

};

template<class T>
inline bool TStackOnMass<T>::is_empty() const noexcept {
	return _top == 0;
}

template<class T>
inline bool TStackOnMass<T>::is_full() const noexcept {
	return _top == _size;
}

template<class T>
TStackOnMass<T>::TStackOnMass(size_t size) : _size(size), _top(0) {
	if (size == 0) {
		throw std::invalid_argument("Stack size must be greater than 0");
	}
	_data = new T[size];
}

template<typename T>
inline TStackOnMass<T>::TStackOnMass(const TStackOnMass& other): _size(other._size),_top(other._top)
{
	_data = new T[_size];
	for (size_t i = 0; i < _top; ++i) {
		_data[i] = other._data[i];
	}
}

template<typename T>
inline TStackOnMass<T>::~TStackOnMass()
{
	delete[] _data;
}



template<typename T>
inline void TStackOnMass<T>::push(const T& val)
{
	if (is_full()) {
		throw std::logic_error("cannot push in full stack");
	}
	_data[++_top] = val;
}

template<typename T>
inline T TStackOnMass<T>::pop()
{
	if (is_empty()) {
		throw std::logic_error("cannot pop from empty stack");
	}
	return _data[_top--];
}

template<typename T>
inline T& TStackOnMass<T>::top()
{
	if (is_empty()) {
		throw std::underflow_error("Stack is empty: no top element");
	}
	return _data[_top];
}
