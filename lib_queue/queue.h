// Copyright 2026 SVEGAK/Novikov R

#ifndef LIB_TStackOnMass_H
#define LIB_TStackOnMass_H

#include "stack_on_mass.h"
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
	TQueueOnStacks(const TStackOnMass& other) : _in(other._in), _out(other._out) {};
	~TQueueOnStacks() = default;

	void push(const T&);
	T pop();

	T& front();
	const T& front() const;
	T& back();//Наверно не нужен
	const T& back() const;

	size_t size();

	inline bool is_empty() const noexcept;
	inline bool is_full() const noexcept;

};

#endif //LIB_STACK_ON_MASS_Hs

