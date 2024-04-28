#ifndef L00PR_FILTER_FIR
#define L00PR_FILTER_FIR

#include "filter.hpp"
#include "../interpolated.hpp"


/// <summary> 
/// Finite Impulse Response filter implementation using discrete convolution
/// </summary>
/// <remarks> Not directly definable </remarks>
class FIR : public Filter {
protected:
    /// <summary> The quantized impulse response function of the filter </summary>
    std::vector<double> kern;

    /// <sumamry> 'Rotating' sample buffer. Same size as 'kern' </summary>
    std::vector<double> sbuf;

    /// <summary> Current position in `sbuf` </summary>
    size_t bufId;

    // cctors
    FIR() : kern(), sbuf(), bufId(0){}
public:
    FIR(std::vector<double>& iResponse) 
        : Filter(), kern(iResponse), sbuf(iResponse.size(), 0), bufId(0) {}
    FIR(std::vector<double>& iResponse, AudioSource* src) 
        : Filter(src), kern(iResponse), sbuf(iResponse.size(), 0), bufId(0) {}
    FIR(AudioSource* src) : Filter(src), kern(), sbuf(), bufId(0){}
    FIR(const FIR& f) = default;

    /// <summary> Adds `sample` to the rotating buffer, then executes the convolution operation </summary>
    double filter(double sample, double delta, double t, double srate);
    
    AudioSource* copy() {return new FIR(*this); }

    // copy assignment operator
    FIR& operator=(const FIR& f) {
        if(this == &f) return *this;
        Filter::operator=(f);
        kern = f.kern;
        sbuf = f.sbuf;
        bufId = f.bufId;
        return *this;
    }
};

/// <summary> 
/// Arbitrary pass filter using the FIR implementation above 
/// Syntax: pass(<frequency_response> [samples]) {[source]}
/// Where frequency_response is an Interpolated<double> describing the desired amplitude at any frequency between 0 and 20k (Hz)
/// and samples is the number of filter samples to be used (default = 200)
/// </summary>
class PassFilter : public FIR {
public:
    PassFilter(std::vector<double> dp, AudioSource* src = nullptr);  
    PassFilter(const Interpolated<double>& dp, int srate, AudioSource* src = nullptr, size_t scount = 200);
    PassFilter(std::istream& str, const int srate, const MakeFlags& flags = MakeFlags::all);
    //AudioSource* copy() {return new PassFilter(*this);}
    PassFilter& operator=(const PassFilter&);
};

#endif
