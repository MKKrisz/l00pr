#include "interpolated.hpp"
#include "interpolator.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(Interpolated, time_constant_test)  {
    Interpolated<double> ip = 1;
    for(double d = 0; d < 100; d += 0.1) {
        EXPECT_EQ(ip(d), 1);
    }
}

TEST(Interpolated, basic_itp_test1) {
    Interpolated<double> ip;
    ip.Set(0, 0);
    ip.Set(1, 2);
    EXPECT_EQ(ip.Size(), 2);
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(0, ip(d-1));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2*d, ip(d));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2, ip(d+1));
    }

}

TEST(Interpolated, basic_itp_test2) {
    Interpolated<double> ip;
    ip.Set(0, 0);
    ip.Set(1, 2);
    ip.Set(2, 0);
    EXPECT_EQ(ip.Size(), 3);
    for(double d = 0.01; d <= 1; d += 0.01) {
        EXPECT_FLOAT_EQ(0, ip(d-1));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2*d, ip(d));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2 - 2*d, ip(1 + d));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(0, ip(d+2));
    }
}

TEST(Interpolated, basic_itp_test3) {
    Interpolated<double> ip;
    ip.Set(0, 0);
    ip.Set(1, 2);
    ip.Set(2, 0);
    EXPECT_EQ(ip.Size(), 3); 
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2 - 2*d, ip(1 + d));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(2*d, ip(d));
    }
    for(double d = 0; d < 1; d += 0.01) {
        EXPECT_FLOAT_EQ(0, ip(d+2));
    }
    for(double d = 0.01; d <= 1; d += 0.01) {
        EXPECT_FLOAT_EQ(0, ip(d-1));
    }
}

TEST(Interpolated, parse1) {
    Interpolated<double> ip;
    std::stringstream ss;
    ss << "0: 0 - 2: 0 - 1: 2 - -1:0";
    ss >> ip;
    EXPECT_EQ(ip.Size(), 4);
    EXPECT_EQ(ip[0].first, -1);
    EXPECT_EQ(ip[1].first, 0);
    EXPECT_EQ(ip[2].first, 1);
    EXPECT_EQ(ip[3].first, 2);
    EXPECT_EQ(ip[0].second, 0);
    EXPECT_EQ(ip[1].second, 0);
    EXPECT_EQ(ip[2].second, 2);
    EXPECT_EQ(ip[3].second, 0);
}
TEST(Interpolated, invalid_parse1) {
    Interpolated<double> ip;
    std::stringstream ss;
    ss << "palacsinta:1";
    EXPECT_THROW(ss >> ip, parse_error);
}
