#ifndef L00PR_FILTER_FIR
#define L00PR_FILTER_FIR

#include "filter.hpp"
#include "../interpolated.hpp"



class FIR : public Filter {
protected:
    std::vector<double> kern;
    std::vector<double> sbuf;
    size_t bufId;
    FIR() : kern(), sbuf(), bufId(0){}
public:
    FIR(std::vector<double>& iResponse) 
        : Filter(), kern(iResponse), sbuf(iResponse.size(), 0), bufId(0) {}
    FIR(std::vector<double>& iResponse, AudioSource* src) 
        : Filter(src), kern(iResponse), sbuf(iResponse.size(), 0), bufId(0) {}
    FIR(AudioSource* src) : Filter(src), kern(), sbuf(), bufId(0){}
    FIR(const FIR& f) = default;

    double filter(double sample, double delta, double t, double srate);
    AudioSource* copy() {return new FIR(*this); }
    FIR& operator=(const FIR& f) {
        if(this == &f) return *this;
        Filter::operator=(f);
        kern = f.kern;
        sbuf = f.sbuf;
        bufId = f.bufId;
        return *this;
    }
};

class PassFilter : public FIR {
public:
    PassFilter(std::vector<double> dp, AudioSource* src = nullptr);  
    PassFilter(const Interpolated<double>& dp, int srate, AudioSource* src = nullptr, size_t scount = 200);
    PassFilter(std::istream& str, int srate, MakeFlags& flags = MakeFlags::all);
    //AudioSource* copy() {return new PassFilter(*this);}
    PassFilter& operator=(const PassFilter&);
};

#endif
