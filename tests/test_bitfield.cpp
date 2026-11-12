// Copyright 2026 SVEGAK/Novikov R

#include <gtest/gtest.h>
//#define BITFIELD_TESTS
#define BITFIELD_TESTS
#ifdef BITFIELD_TESTS
#include "bitfield.h"

#include <sstream>
#include <stdexcept>
#include <utility>

// --- создание / размер ---

TEST(BitField, CtorSizeAndZeroed) {
    BitField bf(38);
    EXPECT_EQ(bf.size(), 38u);
    for (size_t i = 0; i < 38; ++i) EXPECT_FALSE(bf.get_bit(i));
}

TEST(BitField, CopyCtorIsDeep) {
    BitField a(16); a.set_bit(3); a.set_bit(15);
    BitField b(a);
    EXPECT_TRUE(b.get_bit(3));
    EXPECT_TRUE(b.get_bit(15));
    b.reset_bit(3);
    EXPECT_TRUE(a.get_bit(3));   // a не изменился
}

TEST(BitField, MoveCtorEmptiesSource) {
    BitField a(16); a.set_bit(5);
    BitField b(std::move(a));
    EXPECT_TRUE(b.get_bit(5));
    EXPECT_EQ(a.size(), 0u);
}

// --- set / reset / get ---

TEST(BitField, SetResetGet) {
    BitField bf(10);
    bf.set_bit(0);
    EXPECT_TRUE(bf.get_bit(0));
    bf.set_bit(9);
    EXPECT_TRUE(bf.get_bit(9));
    bf.reset_bit(0);
    EXPECT_FALSE(bf.get_bit(0));
    EXPECT_TRUE(bf.get_bit(9));
}

TEST(BitField, OutOfRangeThrows) {
    BitField bf(8);
    EXPECT_THROW(bf.set_bit(8), std::out_of_range);
    EXPECT_THROW(bf.reset_bit(8), std::out_of_range);
    EXPECT_THROW(bf.get_bit(8), std::out_of_range);
}

// --- присваивание ---

TEST(BitField, CopyAssignIsDeep) {
    BitField a(8); a.set_bit(2);
    BitField b(8);
    b = a;
    EXPECT_TRUE(b.get_bit(2));
    b.reset_bit(2);
    EXPECT_TRUE(a.get_bit(2));
}

TEST(BitField, MoveAssign) {
    BitField a(8); a.set_bit(2);
    BitField b(8);
    b = std::move(a);
    EXPECT_TRUE(b.get_bit(2));
    EXPECT_EQ(a.size(), 0u);
}

// --- бинарные побитовые ---

TEST(BitField, And) {
    BitField a(8), b(8);
    a.set_bit(1); a.set_bit(2); a.set_bit(3);
    b.set_bit(2); b.set_bit(3); b.set_bit(4);
    BitField c = a & b;
    EXPECT_FALSE(c.get_bit(1));
    EXPECT_TRUE(c.get_bit(2));
    EXPECT_TRUE(c.get_bit(3));
    EXPECT_FALSE(c.get_bit(4));
}

TEST(BitField, Or) {
    BitField a(8), b(8);
    a.set_bit(1);
    b.set_bit(4);
    BitField c = a | b;
    EXPECT_TRUE(c.get_bit(1));
    EXPECT_TRUE(c.get_bit(4));
}

TEST(BitField, NotInvertsVisibleBits) {
    BitField bf(8);
    bf.set_bit(0); bf.set_bit(2); bf.set_bit(4); bf.set_bit(6);
    BitField inv = ~bf;
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_EQ(inv.get_bit(i), !bf.get_bit(i));
    }
}

TEST(BitField, AndAssign) {
    BitField a(8), b(8);
    a.set_bit(1); a.set_bit(2);
    b.set_bit(2); b.set_bit(3);
    a &= b;
    EXPECT_FALSE(a.get_bit(1));
    EXPECT_TRUE(a.get_bit(2));
    EXPECT_FALSE(a.get_bit(3));
}

TEST(BitField, OrAssign) {
    BitField a(8), b(8);
    a.set_bit(1);
    b.set_bit(4);
    a |= b;
    EXPECT_TRUE(a.get_bit(1));
    EXPECT_TRUE(a.get_bit(4));
}

TEST(BitField, BinaryOpSizeMismatchThrows) {
    BitField a(8), b(16);
    EXPECT_THROW(a & b, std::logic_error);
    EXPECT_THROW(a | b, std::logic_error);
    EXPECT_THROW(a &= b, std::logic_error);
    EXPECT_THROW(a |= b, std::logic_error);
}

// --- сдвиги ---

TEST(BitField, ShiftLeftBasics) {
    BitField bf(8);
    bf.set_bit(0);
    BitField s = bf << 1;
    EXPECT_FALSE(s.get_bit(0));
    EXPECT_TRUE(s.get_bit(1));
}

TEST(BitField, ShiftRightBasics) {
    BitField bf(8);
    bf.set_bit(3);
    BitField s = bf >> 2;
    EXPECT_FALSE(s.get_bit(3));
    EXPECT_TRUE(s.get_bit(1));
}

TEST(BitField, ShiftByZeroKeepsBits) {
    BitField bf(8);
    bf.set_bit(5);
    EXPECT_TRUE((bf << 0).get_bit(5));
    EXPECT_TRUE((bf >> 0).get_bit(5));
}

TEST(BitField, ShiftBySizeOrMoreIsEmpty) {
    BitField bf(8);
    bf.set_bit(0);
    BitField l = bf << 8;
    BitField r = bf >> 8;
    for (size_t i = 0; i < 8; ++i) {
        EXPECT_FALSE(l.get_bit(i));
        EXPECT_FALSE(r.get_bit(i));
    }
}

TEST(BitField, ShiftAcrossWordBoundary) {
    BitField bf(64);
    bf.set_bit(0);
    BitField l = bf << 32;
    EXPECT_FALSE(l.get_bit(0));
    EXPECT_TRUE(l.get_bit(32));

    BitField bf2(64);
    bf2.set_bit(63);
    BitField r = bf2 >> 32;
    EXPECT_FALSE(r.get_bit(63));
    EXPECT_TRUE(r.get_bit(31));
}

// --- ввод-вывод ---

TEST(BitField, OutputFormatLsbLeft) {
    BitField bf(8);
    bf.set_bit(0); bf.set_bit(2); bf.set_bit(4); bf.set_bit(6);
    std::ostringstream os;
    os << bf;
    EXPECT_EQ(os.str(), "10101010");
    EXPECT_EQ(os.str().size(), 8u);
}

TEST(BitField, InputRoundTrip) {
    BitField bf(8);
    std::istringstream is("10101010");
    is >> bf;
    EXPECT_TRUE(is);
    EXPECT_TRUE(bf.get_bit(0));
    EXPECT_FALSE(bf.get_bit(1));
    EXPECT_TRUE(bf.get_bit(2));
    EXPECT_FALSE(bf.get_bit(7));

    std::ostringstream os;
    os << bf;
    EXPECT_EQ(os.str(), "10101010");
}

TEST(BitField, InputWrongLengthFails) {
    BitField bf(8);
    std::istringstream is("101");
    is >> bf;
    EXPECT_FALSE(is);
}

TEST(BitField, InputBadCharFails) {
    BitField bf(4);
    std::istringstream is("10x1");
    is >> bf;
    EXPECT_FALSE(is);
}

#endif  // BITFIELD_TESTS