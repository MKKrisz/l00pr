#include "generator/builtin.hpp"
#include <gtest/gtest.h>
#include <sstream>

// These tests are not going to be thorough. This infrastructure is very hard
// to test, as they are very integral parts of the program.

TEST(AS, simple_test1) {
    SineGenerator g1 = SineGenerator();
    std::stringstream ss; ss << "sine";
    std::unique_ptr<AudioSource> g2 = AudioSource::Make(ss, 44100);
    g1.addPhase();
    g2->addPhase();
    for(double d = 0.01; d < 1; d+=0.01) {
        g1(0, 0.01, d, 44100, 1);
        (*g2)(0, 0.01, d, 44100, 1);
        double sample1 = g1.calc();
        EXPECT_FLOAT_EQ(sin(2*M_PI*d), sample1);
        EXPECT_FLOAT_EQ(sample1, g2->calc());
    }
}

TEST(AS, parse_test_params) {
    std::stringstream ss; ss << "sine(1 0.5 0.5)";
    std::unique_ptr<AudioSource> g;
    EXPECT_NO_THROW(g = AudioSource::Make(ss, 44100));
}

TEST(AS, parse_test_invalid_name) {
    std::stringstream ss; ss << "sin(1 0.5)";
    EXPECT_THROW(AudioSource::Make(ss, 44100), parse_error);
}

TEST(AS, parse_test_invalid_param1) {
    std::stringstream ss; ss << "sine(palacsinta)";
    EXPECT_THROW(AudioSource::Make(ss, 44100), parse_error);
}

TEST(AS, parse_test_invalid_param2) {
    std::stringstream ss; ss << "sine(palacsinta";
    EXPECT_THROW(AudioSource::Make(ss, 44100), parse_error);
}

TEST(AS, parse_test_invalid_param3) {
    std::stringstream ss; ss << "sine(1, 2, 3)";
    EXPECT_THROW(AudioSource::Make(ss, 44100), parse_error);
}

TEST(AS, parse_test_invalid_param4) {
    std::stringstream ss; ss << "sine(1:)";
    EXPECT_THROW(AudioSource::Make(ss, 44100), parse_error);
}

TEST(AS, parse_filter_chain) {
    std::stringstream ss; ss << "gain(0.5) {sine}";
    std::unique_ptr<AudioSource> g = AudioSource::Make(ss, 44100);
    g->addPhase();
    for(double d = 0.01; d < 1; d+=0.01) {
        g->operator()(0, 0.01, d, 44100, 1);
        double sample1 = g->calc();
        EXPECT_FLOAT_EQ(sin(2*M_PI*d)*0.5, sample1);
    }
}
