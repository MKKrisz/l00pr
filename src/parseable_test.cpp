#include "parseable_base.hpp"
#include <gtest/gtest.h>
#include <iostream>

struct test_meta : public Metadata<void> {
    test_meta(const char* kw, std::function<void(std::istream&)> fn) : Metadata<void>(kw, fn) {}
    test_meta(const test_meta& m) : Metadata<void>(m) {}
    std::string ToString() const override { return "testmeta " + Metadata<void>::ToString(); }
};

void func1(std::istream&) {std::cout << "1" << std::endl;}
void func2(std::istream&) {std::cout << "2" << std::endl;}
void func3(std::istream&) {std::cout << "3" << std::endl;}

class test_parser : public Parseable<void, test_meta> {
public:
    static void Init() {
        AddMetadata(test_meta("1", func1));
        AddMetadata(test_meta("2", func2));
        AddMetadata(test_meta("3", func3));
    }
};


TEST(Parseable, valid_parse_single) {
    test_parser::Init();
    EXPECT_EQ(test_parser::GetLongestKeywordMeta().keyword.size(), 1);
    EXPECT_EQ(test_parser::GetKeywordCount(), 3);
    std::stringstream ss;
    ss << "1";
    testing::internal::CaptureStdout();
    test_parser::Parse(ss);
    std::string str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "1\n");
    test_parser::ClearMetadata();
    EXPECT_EQ(test_parser::GetKeywordCount(), 0);
}

TEST(Parseable, valid_parse_with_whitespace) {
    test_parser::Init();
    std::stringstream ss;
    ss << "                    \n      \t 1";
    testing::internal::CaptureStdout();
    test_parser::Parse(ss);
    std::string str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "1\n");
    test_parser::ClearMetadata();
}

TEST(Parseable, valid_parse_multi) {
    test_parser::Init();
    std::stringstream ss;
    ss << "1 2 3";
    testing::internal::CaptureStdout();
    test_parser::Parse(ss);
    test_parser::Parse(ss);
    test_parser::Parse(ss);
    std::string str = testing::internal::GetCapturedStdout();
    EXPECT_EQ(str, "1\n2\n3\n");
    EXPECT_THROW(test_parser::Parse(ss), parse_error);
    test_parser::ClearMetadata();
}

TEST(Parseable, invalid_parse) {
    test_parser::Init();
    std::stringstream ss;
    ss << "4 5 6 palacsinta";
    EXPECT_THROW(test_parser::Parse(ss), parse_error);
    EXPECT_THROW(test_parser::Parse(ss), parse_error);
    EXPECT_THROW(test_parser::Parse(ss), parse_error);
    EXPECT_THROW(test_parser::Parse(ss), parse_error);
    test_parser::ClearMetadata();
}
