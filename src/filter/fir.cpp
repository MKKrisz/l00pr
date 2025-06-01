#include <cmath>
#include <complex>

#include "fir.hpp"

#ifdef _WIN32
#define M_PI 3.141592653589793238
#endif

double FIR::filter(double sample, double, double, double) {
    sbuf[bufId++] = sample;
    bufId = bufId % sbuf.size();
    double ret = 0;
    for(size_t i = 0; i < kern.size(); i++) {
        ret += kern[i] * sbuf[(bufId + i) % sbuf.size()];
    }
    return ret;
}

PassFilter::PassFilter(std::vector<double> dp, AudioSource* src) : FIR(src) {
    //TODO: Implement IFFT instead of IDFT
    for(size_t i = 0; i < dp.size(); i++) {
        double sum = 0;
        for(size_t j = 0; j < dp.size(); j++) {
            //If I don't care about phase-offset, this should, in theory work
            sum += dp[j] * exp(std::complex<double>(0, -2*M_PI*i*j/dp.size())).real();
        }
        sum *= 1.0/dp.size();
        kern.emplace_back(sum);
    }
    sbuf = std::vector<double>(kern.size(), 0);
}

PassFilter::PassFilter(Interpolated<double>& dp, int srate, AudioSource* src, size_t scount) : FIR(src) {
    for(size_t i = 0; i < scount; i++) {
        double sum = 0;
        for(size_t j = 0; j < scount; j++) {
            double amplitude = dp.Get(double(j)/scount*srate);
            sum += (amplitude * exp(std::complex<double>(0, 2*M_PI*i*j/scount))).real();
        }
        sum *= 1.0/scount;
        kern.emplace_back(sum);
    }
    sbuf = std::vector<double>(kern.size(), 0);
}

PassFilter::PassFilter(std::istream& str, int srate, const MakeFlags& flags) {
    Interpolated<double> dp;
    std::unique_ptr<AudioSource> src = nullptr;
    str >> expect('(') >> dp;
    str >> skipws;
    size_t scount = 200;
    if(str.peek() != ')') {
        if(!isdigit(str.peek()))
            throw parse_error(str, "Expected digit");
        str >> scount;
    }
    str >> expect(')') >> skipws;
    if(str.peek() == '{') {
        str.get();
        src = std::move(AudioSource::Make(str, srate, flags));
        str >> expect('}');
    }
    *this = PassFilter(dp, srate, src.get(), scount);
}

PassFilter& PassFilter::operator=(const PassFilter& f) {
    if(this == &f) return *this;
    FIR::operator=(f);
    return *this;
}


std::unique_ptr<PassFilter> PassFilter::Create(std::istream& str, const int srate, const MakeFlags& flags) {
    return std::make_unique<PassFilter>(str, srate, flags);
}

