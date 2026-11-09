// Copyright 2026 SVEGAK/Novikov R
#include "bitfield.h"

unsigned int BitField::get_mask(size_t pos)
{
	if (pos >= _size) {
		throw std::out_of_range("pos out of range");
	}
	unsigned int mask = 1;
	return mask << pos;
}

size_t BitField::get_index(size_t pos) const
{
	if (pos >= _size) throw std::out_of_range("pos out of range");
	return pos / sizeof(unsigned int)*CHAR_BIT;
}

size_t BitField::blocks(size_t bits)
{
	 return (bits + sizeof(unsigned int) * CHAR_BIT - 1) / (sizeof(unsigned int) * CHAR_BIT);

}

BitField::BitField(size_t size): _size(size)
{
	_data = new unsigned int[size/sizeof(unsigned int)]();
}

BitField::BitField(const BitField& other)
	: _size(other._size), _data(new unsigned int[blocks(other._size)]()) {
	std::copy(other._data, other._data + blocks(other._size), _data);
}

BitField::BitField(BitField&& other) noexcept
	: _size(other._size), _data(other._data)
{
	other._size = 0;
	other._data = nullptr;
}

BitField::~BitField()
{
	delete[] _data;
}

void BitField::set_bit(size_t pos)
{
	if (pos >= _size){
		throw std::out_of_range("pos out of range");
	}
	size_t index = get_index(pos);
	unsigned int mask = get_mask(pos);
	_data[index] |= mask;
}

void BitField::reset_bit(size_t pos)
{
	if (pos >= _size) {
		throw std::out_of_range("pos out of range");
	}
	size_t index = get_index(pos);
	unsigned int mask = get_mask(pos);
	_data[index] &= ~(mask);
}

bool BitField::get_bit(size_t pos) const
{
	if (pos >= _size) {
		throw std::out_of_range("pos out of range");
	}
	size_t index = get_index(pos);
	return (_data[index] >> (pos % sizeof(unsigned int) * CHAR_BIT))&1u; //Значение переводится в uint,а при return неявно переводится в bool 
}


size_t BitField::size()
{
	return _size;
}

