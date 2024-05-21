#include "freq.hpp"
#include <gtest/gtest.h>

//Here we will only test frequency parsing minimally as that is done int util_test.cpp
TEST(FreqTest, cctor_test1) {
    Frequency f{};
    EXPECT_EQ(f.getName(), std::to_string(0.0));
    EXPECT_EQ(f.getFreq(), 0);
}

TEST(FreqTest, cctor_test2) {
    Frequency f("123");
    EXPECT_EQ(f.getName(), "123");
    EXPECT_EQ(f.getFreq(), 123);
}

TEST(FreqTest, cctor_test3) {
    Frequency f(123);
    EXPECT_EQ(f.getName(), std::to_string(123.0));
    EXPECT_EQ(f.getFreq(), 123);
}

TEST(FreqTest, cctor_test4) {
    std::stringstream ss;
    ss << "123";
    Frequency f(ss);
    ss.get();
    EXPECT_EQ(f.getName(), "123");
    EXPECT_EQ(f.getFreq(), 123);
    EXPECT_TRUE(ss.eof());
}

TEST(FreqTest, cctor_basic_notes) {
    std::stringstream ss;
    ss << "A4";
    Frequency f(ss);
    EXPECT_EQ(f.getName(), "A4");
    EXPECT_EQ(f.getFreq(), 440);
}

TEST(FreqTest, add) {
    Frequency f1(440);
    Frequency f2(1);
    EXPECT_FLOAT_EQ(f1.getFreq(), 440);
    f1 = f1 + f2;
    EXPECT_FLOAT_EQ(f1.getFreq(), 441);
    f1 = f1 + 2;
    EXPECT_FLOAT_EQ(f1.getFreq(), 443);
}

TEST(FreqTest, mul) {
    Frequency f1(440);
    Frequency f2(2);
    EXPECT_FLOAT_EQ(f1.getFreq(), 440);
    f1 = f1 * f2;
    EXPECT_FLOAT_EQ(f1.getFreq(), 880);
    f1 = f1 * 2;
    EXPECT_FLOAT_EQ(f1.getFreq(), 1760);
    f2 = 2 * f2;
    EXPECT_FLOAT_EQ(f2.getFreq(), 4);
}
