// Copyright 2026 SVEGAK/Novikov R

#include <gtest/gtest.h>
#define STACK_ON_MASS_TESTS

#ifdef STACK_ON_MASS_TESTS
#include "stack_on_mass.h"

#include <string>



TEST(TStackOnMassCtor, ZeroSizeThrows) {
    EXPECT_THROW(TStackOnMass<int>(0), std::invalid_argument);
}

TEST(TStackOnMassCtor, InitialStateIsEmptyNotFull) {
    TStackOnMass<int> s(5);
    EXPECT_TRUE(s.is_empty());
    EXPECT_FALSE(s.is_full());
}

TEST(TStackOnMassCtor, SizeOneInitialState) {
    TStackOnMass<int> s(1);
    EXPECT_TRUE(s.is_empty());
    EXPECT_FALSE(s.is_full());
}

TEST(TStackOnMassPush, SinglePush) {
    TStackOnMass<int> s(5);
    s.push(10);
    EXPECT_FALSE(s.is_empty());
    EXPECT_FALSE(s.is_full());
    EXPECT_EQ(s.top(), 10);
}

TEST(TStackOnMassPush, PushUntilFull) {
    TStackOnMass<int> s(3);
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_TRUE(s.is_full());
    EXPECT_FALSE(s.is_empty());
}

TEST(TStackOnMassPush, PushOnFullThrows) {
    TStackOnMass<int> s(1);
    s.push(1);
    EXPECT_THROW(s.push(2), std::logic_error);
}

TEST(TStackOnMassPop, PopFromEmptyThrows) {
    TStackOnMass<int> s(3);
    EXPECT_THROW(s.pop(), std::logic_error);
}

TEST(TStackOnMassPop, LifoOrder) {
    TStackOnMass<int> s(3);
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.pop(), 3);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.pop(), 1);
    EXPECT_TRUE(s.is_empty());
}

TEST(TStackOnMassPop, PopAfterPartialPush) {
    TStackOnMass<int> s(4);
    s.push(1);
    s.push(2);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.top(), 1);
    s.push(3);
    EXPECT_EQ(s.top(), 3);
    EXPECT_FALSE(s.is_full());
}

TEST(TStackOnMassPop, ReuseAfterEmpty) {
    TStackOnMass<int> s(2);
    s.push(1);
    s.push(2);
    s.pop();
    s.pop();
    EXPECT_TRUE(s.is_empty());
    s.push(9);
    EXPECT_EQ(s.top(), 9);
}

TEST(TStackOnMassTop, TopOnEmptyThrows) {
    TStackOnMass<int> s(3);
    EXPECT_THROW(s.top(), std::logic_error);
}

TEST(TStackOnMassTop, TopReturnsReference) {
    TStackOnMass<int> s(3);
    s.push(5);
    s.top() = 7;
    EXPECT_EQ(s.top(), 7);
    EXPECT_EQ(s.pop(), 7);
}

TEST(TStackOnMassCopy, PreservesElements) {
    TStackOnMass<int> a(3);
    a.push(1);
    a.push(2);
    EXPECT_EQ(a.top(), 2);
    TStackOnMass<int> b(a);
    EXPECT_FALSE(b.is_empty());
    EXPECT_EQ(b.pop(), 2);
    EXPECT_EQ(b.pop(), 1);
    EXPECT_TRUE(b.is_empty());
}

TEST(TStackOnMassCopy, IndependentFromOriginal) {
    TStackOnMass<int> a(3);
    a.push(42);
    TStackOnMass<int> b(a);
    a.push(43);
    EXPECT_EQ(b.top(), 42);
    b.push(99);
    EXPECT_EQ(a.top(), 43);
}

TEST(TStackOnMassCopy, CopyOfFull) {
    TStackOnMass<int> a(2);
    a.push(1);
    a.push(2);
    TStackOnMass<int> b(a);
    EXPECT_TRUE(b.is_full());
    EXPECT_EQ(b.pop(), 2);
    EXPECT_EQ(b.pop(), 1);
}

TEST(TStackOnMassCopy, CopyOfEmpty) {
    TStackOnMass<int> a(3);
    TStackOnMass<int> b(a);
    EXPECT_TRUE(b.is_empty());
    EXPECT_FALSE(b.is_full());
    b.push(5);
    EXPECT_EQ(b.pop(), 5);
}


TEST(TStackOnMassString, BasicOps) {
    TStackOnMass<std::string> s(2);
    s.push("hello");
    s.push("world");
    EXPECT_EQ(s.top(), "world");
    EXPECT_EQ(s.pop(), "world");
    EXPECT_EQ(s.pop(), "hello");
    EXPECT_TRUE(s.is_empty());
}


TEST(TStackOnMassSizeOne, PushPopCycle) {
    TStackOnMass<int> s(1);
    EXPECT_TRUE(s.is_empty());
    EXPECT_FALSE(s.is_full());
    s.push(42);
    EXPECT_TRUE(s.is_full());
    EXPECT_EQ(s.pop(), 42);
    EXPECT_TRUE(s.is_empty());
}

#endif //STACK_ON_MASS_TESTS