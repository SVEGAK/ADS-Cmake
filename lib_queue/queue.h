// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_QUEUE_ON_STACKS
#define LIB_QUEUE_ON_STACKS

#include "stack_on_mass.h"
#include <exception>
// Очередь реализована двумя стеками:
// _in стек куда вводятся новые элементы 
// _out стек в который переворачиваются и переносятся элементы из _in (если _out пуст)

template <typename T>
class TQueueOnStacks {
	TStackOnMass<T> _in;
	TStackOnMass<T> _out;
	void transfer() {
		if (_out.is_empty()) {
			while (!_in.is_empty()) {
				_out.push(_in.pop());
			}
		}
	}
public:
	TQueueOnStacks(size_t size) : _in(size), _out(size) {};
	TQueueOnStacks(const TQueueOnStacks& other) : _in(other._in), _out(other._out) {};
	~TQueueOnStacks() = default;

	void enqueue(const T&); //Добавление элемента в очередь
	T dequeue(); //Удаление из очереди

	const T& front() const;
	T& front();
	const T& back() const;
	T& back();
	inline size_t size() const noexcept;

	inline bool is_empty() const noexcept;
	inline bool is_full() const noexcept;
};


template<typename T>
inline void TQueueOnStacks<T>::enqueue(const T& elem){
	if (_in.is_full()) {
		transfer();
		if (_in.is_full()) {
			throw std::logic_error("Queue is full.");
		}
	}
	_in.push(elem);
}

template<typename T>
inline T TQueueOnStacks<T>::dequeue()
{
	if (_out.is_empty()) {
		transfer();
		if (_out.is_empty()) {
			throw std::logic_error("Queue is empty.");
		}
		
	}
	return _out.pop();
}

template<typename T>
inline const T& TQueueOnStacks<T>::front() const
{
	if (!_out.is_empty()) {
		return _out.top();
	}
	if (!_in.is_empty()) {
		return _in.bottom()
	}
	throw std::logic_error("Queue is empty.");
}
template<typename T>
inline T& TQueueOnStacks<T>::front()
{
	if (!_out.is_empty()) {
		return _out.top();
	}
	if (!_in.is_empty()) {
		return _in.bottom()
	}
	throw std::logic_error("Queue is empty.");
	
}
template<typename T>
inline const T& TQueueOnStacks<T>::back() const
{
	if (!_in.is_empty()) {
		return _in.top();
	}
	if (!_out.is_empty()) {
		return _out.bottom();
	}

	throw std::logic_error("Queue is empty.");
	
}
template<typename T>
inline T& TQueueOnStacks<T>::back()
{
	if (!_in.is_empty()) {
		return _in.top();
	}
	if (!_out.is_empty()) {
		return _out.bottom();
	}
	throw std::logic_error("Queue is empty.");
}
template<typename T>
inline size_t TQueueOnStacks<T>::size() const noexcept
{
	return (_in.size() + _out.size());
}

template<typename T>
inline bool TQueueOnStacks<T>::is_empty() const noexcept
{
	return (_out.is_empty() && _in.is_empty());
}

template<typename T>
inline bool TQueueOnStacks<T>::is_full() const noexcept
{
	return (!_out.is_empty() && _in.is_full());
}


#endif //LIB_QUEUE_ON_STACKS

