// Copyright 2026 SVEGAK/Novikov R
#include "bitfield.h"

unsigned int BitField::get_mask(size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("pos out of range");
    }
    return 1u << (pos % kBlockLength);
}

size_t BitField::get_index(size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("pos out of range");
    }
    return pos / kBlockLength;
}

size_t BitField::blocks(size_t bits) const{
    return (bits + kBlockLength - 1) / kBlockLength;
}

BitField::BitField(size_t size) : _size(size) {
    _data = new unsigned int[blocks(size)]();
}

BitField::BitField(const BitField& other)
    : _size(other._size),
    _data(new unsigned int[blocks(other._size)]()) {
    std::copy(other._data, other._data + blocks(other._size), _data);
}

BitField::BitField(BitField&& other) noexcept
    : _size(other._size), _data(other._data) {
    other._size = 0;
    other._data = nullptr;
}

BitField::~BitField() {
    delete[] _data;
}

void BitField::set_bit(size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("pos out of range");
    }
    size_t index = get_index(pos);
    unsigned int mask = get_mask(pos);
    _data[index] |= mask;
}

void BitField::reset_bit(size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("pos out of range");
    }
    size_t index = get_index(pos);
    unsigned int mask = get_mask(pos);
    _data[index] &= ~mask;
}

bool BitField::get_bit(size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("pos out of range");
    }
    size_t index = get_index(pos);
    // unsigned int converts to bool implicitly (0 -> false, else true).
    return (_data[index] >> (pos % kBlockLength)) & 1u;
}

size_t BitField::size() const noexcept{
    return _size;
}

BitField& BitField::operator=(const BitField& other) {
    if (this != &other) {
        BitField tmp(other);
        std::swap(_data, tmp._data);
        std::swap(_size, tmp._size);
    }
    return *this;
}

BitField& BitField::operator=(BitField&& other) noexcept {
    if (this != &other) {
        BitField tmp(std::move(other));
        std::swap(_data, tmp._data);
        std::swap(_size, tmp._size);
    }
    return *this;
}

BitField BitField::operator<<(size_t n) const {
    if (n == 0) return *this;
    if (n >= _size) return BitField(_size);
    BitField copy(_size);
    const size_t wc = copy.blocks(_size);
    const size_t zero_blocks = n / kBlockLength; //Количество блоков которые будут сдвинуты полностью (обнулены)
    const size_t shift = n % kBlockLength; //На сколько нужно сдвинуть остальные блокиs

    for (size_t i = 0; i < wc; i++) {
        unsigned int v = 0;
        if (i >= zero_blocks) {
            v = _data[i - zero_blocks] << shift;
            if (shift && i > zero_blocks) {
                v |= _data[i - zero_blocks - 1] >> (kBlockLength - shift);
            }
        }
        copy._data[i] = v;
    }

    const size_t rem = _size % kBlockLength;
    if (rem != 0) {
        copy._data[wc - 1] &= (1u << rem) - 1u;
    }
    return copy;
}

BitField BitField::operator>>(size_t n) const {
    if (n == 0) return *this;
    if (n >= _size) return BitField(_size);
    BitField copy(_size);
    const size_t wc = copy.blocks(_size);
    const size_t zero_blocks = n / kBlockLength;
    const size_t shift = n % kBlockLength;

    for (size_t i = 0; i < wc; i++) {
        unsigned int v = 0;
        if (i + zero_blocks < wc) {
            v = _data[i + zero_blocks] >> shift;
            if (shift && i + zero_blocks + 1 < wc) {
                v |= _data[i + zero_blocks + 1] << (kBlockLength - shift);
            }
        }
        copy._data[i] = v;
    }

    const size_t rem = _size % kBlockLength;
    if (rem != 0) {
        copy._data[wc - 1] &= (1u << rem) - 1u;
    }
    return copy;
}

BitField BitField::operator&(const BitField& other) const {
    if (_size != other._size) {
        throw std::logic_error("BitField size operands must be equal.");
    }
    BitField copy(*this);
    const size_t wc = blocks(_size);
    for (size_t i = 0; i < wc; i++) {
        copy._data[i] &= other._data[i];
    }
    return copy;
}
BitField BitField::operator|(const BitField& other) const {
    if (_size != other._size) {
        throw std::logic_error("BitField size operands must be equal.");
    }
    BitField copy(*this);
    for (size_t i = 0; i < blocks(size()); i++) {
        copy._data[i] |= other._data[i];
    }
    return copy;
}

BitField BitField::operator~() const {
    BitField copy(*this);
    for (size_t i = 0; i < blocks(size()); i++) {
        copy._data[i] = ~copy._data[i];
    }
    const size_t rem = _size % kBlockLength;
    if (rem != 0) {
        copy._data[blocks(size()) - 1] &= (1u << rem) - 1u;
    }
    return copy;
}

BitField& BitField::operator&=(const BitField& other) {
    if (_size != other._size) {
        throw std::logic_error("BitField size operands must be equal.");
    }
    for (size_t i = 0; i < blocks(size()); i++) {
        _data[i] &= other._data[i];
    }
    return *this;
}

BitField& BitField::operator|=(const BitField& other) {
    if (_size != other._size) {
        throw std::logic_error("BitField size operands must be equal.");
    }
    for (size_t i = 0; i < blocks(size()); i++) {
        _data[i] |= other._data[i];
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const BitField& bf) {
    for (size_t i = 0; i < bf._size; ++i) {
        os << ((bf._data[i / kBlockLength] >> (i % kBlockLength)) & 1u ? '1' : '0');
    }
    return os;
}

std::istream& operator>>(std::istream& is, BitField& bf) {
    std::string s;
    if (!(is >> s)) return is;            // не прочитали токен

    if (s.size() != bf._size) {           // длина должна совпасть с N
        is.setstate(std::ios::failbit);
        return is;
    }
    for (size_t i = 0; i < bf._size; ++i) {
        if (s[i] == '0') {
            bf.reset_bit(i);
        }
        else if (s[i] == '1') {
            bf.set_bit(i);
        }
        else {                          // не 0/1 — ошибка ввода
            is.setstate(std::ios::failbit);
            return is;
        }
    }
    return is;
}
