#include <gtest/gtest.h>
#include "util.hpp"
#include "exceptions/parse_error.hpp"

TEST(UtilTest, getFreqInt) {
    std::stringstream ss1;
    ss1 << "440";
    EXPECT_FLOAT_EQ(440, getFreq(ss1)) << "Cannot parse int!";
}

TEST(UtilTest, getFreqDouble1) {
    std::stringstream ss1;
    ss1 << "440.5397";
    EXPECT_FLOAT_EQ(440.5397, getFreq(ss1)) << "Cannot parse double!";
}

TEST(UtilTest, getFreqDouble2) {
    std::stringstream ss1;
    for(int i = 0; i < 400; i++) {
        ss1 = {};
        ss1 << i/10.0;
        EXPECT_FLOAT_EQ(i/10.0, getFreq(ss1));
    }
}

TEST(UtilTest, getFreq) {
    const char* notes1[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};       // With sharps
    const char* notes2[] = {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab"};       // With flats
    std::stringstream ss1;
    std::stringstream ss2;

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 12; j++) {
            ss1 = {};
            ss2 = {};
            ss1 << notes1[j] << (j > 2? i + 1: i);
            ss2 << notes2[j] << (j > 2? i + 1: i);
            EXPECT_FLOAT_EQ(27.5*std::pow(2, i + j/12.0), getFreq(ss1)) << "Cannot parse '" << ss1.str() << "'!";
            EXPECT_FLOAT_EQ(27.5*std::pow(2, i + j/12.0), getFreq(ss2)) << "Cannot parse '" << ss2.str() << "'!";
            //EXPECT_FLOAT_EQ(getFreq(ss2), getFreq(ss1)) << ss1.str() << " != " << ss2.str();
        }
    }

}

TEST(UtilTest, getFreqEquivalent1) {
    const char* notes1[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};       // With sharps
    const char* notes2[] = {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab"};       // With flats
    std::stringstream ss1;
    std::stringstream ss2;

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 12; j++) {
            ss1 = {};
            ss2 = {};
            ss1 << notes1[j] << (j > 2? i + 1: i);
            ss2 << notes2[j] << (j > 2? i + 1: i);
            //EXPECT_FLOAT_EQ(27.5*std::pow(2, i + j/12.0), getFreq(ss1)) << "Cannot parse '" << ss1.str() << "'!";
            //EXPECT_FLOAT_EQ(27.5*std::pow(2, i + j/12.0), getFreq(ss2)) << "Cannot parse '" << ss2.str() << "'!";
            EXPECT_FLOAT_EQ(getFreq(ss2), getFreq(ss1)) << ss1.str() << " != " << ss2.str();
        }
    }

}

TEST(UtilTest, getFreqEquivalent2) {
    std::stringstream ss1;
    std::stringstream ss2;
    ss1 << "A4";
    ss2 << "Gx4";
    EXPECT_EQ(getFreq(ss1), getFreq(ss2)) << ss1.str() << " != " << ss2.str();
}

TEST(UtilTest, getFreqEmpty) {
    std::stringstream ss1;
    ss1 << "";
    EXPECT_THROW(getFreq(ss1), parse_error) << "Did not throw for empty string!";
}

TEST(UtilTest, getFreqHNotation) {
    std::stringstream ss1;
    ss1 << "H1";
    EXPECT_THROW(getFreq(ss1), parse_error) << "Did not throw for H!";
}

TEST(UtilTest, getFreqStartsWithSpace) {
    std::stringstream ss1;
    ss1 << " G1";
    EXPECT_NO_THROW(getFreq(ss1)) << "Thrown for starting space";
}
TEST(UtilTest, getFreqMiddleSpace) {
    std::stringstream ss1;
    ss1 << "G 1";
    EXPECT_ANY_THROW(getFreq(ss1)) << "Did not throw for middle space";
}


TEST(UtilTest, getLineNo) {
    std::stringstream ss;
    ss << "This is a string made of one singular line";
    int n = rand() % ss.str().size();
    for(int i = 0; i < n; i++) {
        ss.get();
    }
    EXPECT_EQ(getLineColNo(ss).first, 1) << "Hallucinated a \\n";
}

TEST(UtilTest, getColNo) {
    std::stringstream ss;
    ss << "This is a string made of one singular line";
    int n = rand() % ss.str().size();
    for(int i = 0; i < n; i++) {
        ss.get();
    }
    EXPECT_EQ(getLineColNo(ss).second, n+1) << "Miscalculated char pos";
}

TEST(UtilTest, getLineNo2) {
    std::stringstream ss;
    ss << "This\nis\na\nstring\nwhere\neach\nword\nis\non\na\ndifferent\nline";
    int n = 13;
    for(int i = 0; i < n; i++) {
        ss.get();
    }
    EXPECT_EQ(getLineColNo(ss).first, 4) << "Miscaluclated line no";
}

TEST(UtilTest, getColNo2) {
    std::stringstream ss;
    ss << "This\nis\na\nstring\nwhere\neach\nword\nis\non\na\ndifferent\nline";
    int n = 13;
    for(int i = 0; i < n; i++) {
        ss.get();
    }
    EXPECT_EQ(getLineColNo(ss).second, 4) << "Miscaluclated line no";
}

TEST(UtilTest, getColNo3) {
    std::stringstream ss;
    ss << " This\nis\na\nstring\nthat\nstarts\nwith\na\nspace\n";
    int n = 13;
    for(int i = 0; i < n; i++) {
        ss.get();
    }
    EXPECT_EQ(getLineColNo(ss).second, 3) << "Miscaluclated line no";
}

TEST(UtilTest, Lerp_Equality) {
    for(double i = 0; i < 1; i+=0.001) {
        EXPECT_FLOAT_EQ(i, lerp(0.0, 1.0, i));
    }
}
TEST(UtilTest, Lerp_Desc) {
    for(double i = 1; i > 0; i-=0.001) {
        EXPECT_FLOAT_EQ(i, lerp(0.0, 1.0, i));
    }
}

TEST(UtilTest, isNote1) {
    for(char c = 'A'; c <= 'G'; c++) {
        EXPECT_TRUE(isNote(c));
    }
}
TEST(UtilTest, isNote2) {
    for(char c = '0'; c <= '9'; c++) {
        EXPECT_TRUE(isNote(c));
    }
}
TEST(UtilTest, isNote3) {
    for(char c = 0; c < 127; c++) {
        if(c >= '0' && c <= '9')
            continue;
        if(c >= 'A' && c <= 'G')
            continue;
        EXPECT_FALSE(isNote(c));
    }
}

TEST(UtilTest, bSearch_EmptyArr) {
    std::vector<std::pair<double, double>> vec = {};
    EXPECT_EQ(bSearch(vec, 5.0), 0);
}
TEST(UtilTest, bSearch_filled_arr) {
    std::vector<std::pair<double, double>> vec = {{1, 2}, {2, 5}, {3, 7}, {5, 9}};
    EXPECT_EQ(bSearch(vec, -20.0), 0);
    EXPECT_EQ(bSearch(vec, 1.0), 0);
    EXPECT_EQ(bSearch(vec, 1.5), 0);    // Gets incremented in ordered_add
    EXPECT_EQ(bSearch(vec, 2.0), 1);
    EXPECT_EQ(bSearch(vec, 3.0), 2);
    EXPECT_EQ(bSearch(vec, 3.5), 2);    // Gets incremented in ordered_add
    EXPECT_EQ(bSearch(vec, 5.0), 3);
    EXPECT_EQ(bSearch(vec, 200.0), 3);
}

TEST(UtilTest, binary_insert) {
    std::vector<std::pair<double, double>> vec = {{1, 2}, {2, 5}, {3, 7}, {5, 9}};
    std::pair<double, double> p = {2.1, 7};
    EXPECT_FLOAT_EQ(vec[1].first, 2);
    EXPECT_EQ(vec[2].first, 3);
    ordered_add(vec, p);
    EXPECT_FLOAT_EQ(vec[1].first, 2);
    EXPECT_FLOAT_EQ(vec[2].first, 2.1);
    EXPECT_FLOAT_EQ(vec[3].first, 3);
}
