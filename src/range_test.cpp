#include "range.hpp"
#include "freq.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(RangeTest, test_integral) {
    Range<int> r = std::vector<int>{1, 2, 3, 4, 5, 6, 7 , 8, 9, 10};
    EXPECT_TRUE(r.isFix());
    EXPECT_EQ(r.size(), 10);
    r.add(11);
    EXPECT_EQ(r.size(), 11);
    for(size_t i = 0; i < r.size(); i++) {
        EXPECT_EQ(r.get(double(i)/r.size()), r[i]);
    }
    r.setFix(false);
    EXPECT_FALSE(r.isFix());
    for(double i = 0; i < 1; i += 0.01) {
        EXPECT_EQ(int(i + 1), r.get(i));
    }
}

TEST(RangeTest, test_fpoint_itp1) {
    Range<double> r = std::vector<double>{1, 2, 3, 4, 5, 6, 7 , 8, 9, 10};
    r.setFix(false);
    for(double i = 0; i < 1; i += 0.01) {
        EXPECT_FLOAT_EQ(i + 1, r.get(i));
    }
}

TEST(RangeTest, test_fpoint_itp2) {
    Range<double> r(0, 10);
    for(double i = 0; i < 1; i += 0.01) {
        EXPECT_FLOAT_EQ(i*10, r.get(i));
    }
}

TEST(RangeTest, cctor_test1) {
    Range<int> r1;
    EXPECT_EQ(r1.size(), 0);
    EXPECT_THROW(r1.get(0), std::out_of_range);
    r1.add(0);
    EXPECT_EQ(r1.size(), 1);
    EXPECT_NO_THROW(r1.get(0));
    EXPECT_EQ(r1.get(0), 0);
    Range<int> r2(0);
    EXPECT_EQ(r1.size(), 1);
    EXPECT_NO_THROW(r2.get(0));
    EXPECT_EQ(r2.get(0), 0);

}

TEST(RangeTest, cctor_test2) {
    Range<double> r1(0, 100);
    EXPECT_EQ(r1.size(), 2);
    for(double i = 0; i < 1; i += 0.01) {
        EXPECT_EQ(100*i, r1.get(i));
    }
}

TEST(RangeTest, parse_valid1) {
    Range<double> r;
    std::stringstream ss;
    ss << "123";
    ss >> r;
    EXPECT_EQ(r.size(), 1);
    EXPECT_EQ(r[0], 123);
}

TEST(RangeTest, parse_valid2) {
    Range<double> r;
    std::stringstream ss;
    ss << "(0 - 100)";
    ss >> r;
    EXPECT_EQ(r.size(), 2);
    EXPECT_FALSE(r.isFix());
    for(double i = 0; i < 1; i += 0.01) {
        EXPECT_EQ(100*i, r.get(i));
    }
}

TEST(RangeTest, parse_valid3) {
    Range<double> r;
    std::stringstream ss;
    ss << "(1 2 3 4 5 6 7 8 9 10)";
    ss >> r;
    EXPECT_TRUE(r.isFix());
    EXPECT_EQ(r.size(), 10);
    for(size_t i = 0; i < r.size(); i++) {
        EXPECT_EQ(i + 1, r[i]);
    }
}

TEST(RangeTest, parse_valid4) {
    Range<double> r;
    std::stringstream ss;
    ss << "(1)";
    ss >> r;
    EXPECT_TRUE(r.isFix());
    EXPECT_EQ(r.size(), 1);
    for(size_t i = 0; i < r.size(); i++) {
        EXPECT_EQ(i + 1, r[i]);
    }
}

//We can only test for semantics
TEST(RangeTest, parse_invalid1) {
    Range<double> r;
    std::stringstream ss;
    ss << "(1";
    EXPECT_THROW(ss >> r, parse_error);
}

TEST(RangeTest, parse_invalid2) {
    Range<double> r;
    std::stringstream ss;
    ss << "(1(";
    EXPECT_THROW(ss >> r, parse_error);
}
TEST(RangeTest, parse_invalid3) {
    Range<double> r;
    std::stringstream ss;
    ss << "(-)";
    EXPECT_THROW(ss >> r, parse_error);
}

TEST(RangeTest, parse_invalid4) {
    Range<double> r;
    std::stringstream ss;
    ss << "(1, 2, 3)";
    EXPECT_THROW(ss >> r, parse_error);
}

TEST(RangeTest, parse_freq) {
    Range<Frequency> r;
    std::stringstream ss;
    ss << "(A4 - A5)";
    ss >> r;
    EXPECT_EQ(r.size(), 2);
    EXPECT_FLOAT_EQ(r[0].getFreq(), 440);
    EXPECT_FLOAT_EQ(r[1].getFreq(), 880);
    EXPECT_FALSE(r.isFix());
}
