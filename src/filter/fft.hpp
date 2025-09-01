#pragma once
#include "filter.hpp"
#include "../interpolated.hpp"
#include "../util.hpp"

#include <fftw3.h>



// TODO: Add a way to change FFTW_MEASURE dynamically
// pass()
class FFT : public Filter {
public:
    FFT() : Filter(nullptr) {init_plan();}
    FFT(Source* s) : Filter(s) {init_plan();}
    FFT(const FFT& f) : Filter(f), fft_size(f.fft_size), buffer_top(f.buffer_top), fft(f.fft), output_bottom(f.output_bottom) {
        init_plan();
        for(size_t i = 0; i < fft_size; i++) {
            input_buffer[i] = f.input_buffer[i];
            output_buffer[i] = f.output_buffer[i];
        }
        for(size_t i = 0; i < (fft_size/2 + 1); i++) {
            fft[i][0] = f.fft[i][0];
            fft[i][1] = f.fft[i][1];
        }
    }
    
    FFT(size_t buffer_size, Source* s = nullptr) : Filter(s), fft_size(buffer_size) {init_plan();}

    inline double filter(double sample, double, double, int) { 
        double ret = output_buffer[output_bottom];
        output_bottom++;
        if(output_bottom == fft_size) {
            output_bottom = 0;
        }

        input_buffer[buffer_top] = sample;
        buffer_top++;
        if(buffer_top == fft_size) {
            fftw_execute(fft_plan);
            ModifyFFT(fft, fft_size/2 + 1);
            fftw_execute(ifft_plan);
            for(size_t i = 0; i < fft_size; i++) {
                output_buffer[i] *= (1.0/fft_size);
            }
            buffer_top = 0;
        }
        return ret;
    }
    //std::unique_ptr<Source> copy() {return std::make_unique<FFT>(*this); }
    inline FFT& operator=(const FFT&) = default;
    std::string ToString() const {return Filter::ToString() + "Dummy";}
    std::string GetNameAndParams() const { return "dummy"; }

    virtual ~FFT() {
        fftw_free(input_buffer);
        fftw_free(fft);
        fftw_free(output_buffer);
    }

protected:
    virtual void ModifyFFT(fftw_complex* fft, size_t n) = 0;
    void init_plan() {
        input_buffer = (double*)fftw_malloc(sizeof(double) * fft_size);
        fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fft_size/2 + 1));
        output_buffer = (double*)fftw_malloc(sizeof(double) * fft_size);
        
        fft_plan =  fftw_plan_dft_r2c_1d(fft_size, input_buffer, fft, FFTW_MEASURE);
        ifft_plan = fftw_plan_dft_c2r_1d(fft_size, fft, output_buffer, FFTW_MEASURE);
    }

    fftw_plan fft_plan;
    fftw_plan ifft_plan;

    size_t fft_size = 4096;
    double* input_buffer = nullptr;
    size_t buffer_top = 0;
    fftw_complex* fft = nullptr;
    double* output_buffer = nullptr;
    size_t output_bottom = 0;
};


class MockFFT : public FFT {
protected:
    void ModifyFFT(fftw_complex* fft, size_t size) override {
        for(size_t i = 0; i < size; i++) {
            for(int j = -40; j < fft[i][0]*2; j++) {
                std::cout << ' ';
            }
            std::cout << "[41m [m" << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
    }
public:

    MockFFT() : FFT() {/*nothing*/}
    MockFFT(const MockFFT& f) : FFT(f) {/*nothing*/}
    MockFFT(size_t resolution, Source* src) : FFT(resolution, src) {/*nothing*/}

    std::unique_ptr<Source> copy() override {return std::make_unique<MockFFT>(*this);}
    MockFFT& operator=(const MockFFT&) = default;
    std::string ToString() const override {return Filter::ToString() + "MockFFT"; }
    std::string GetNameAndParams() const override {return "mockfft(" + std::to_string((size_t)fft_size) + ")";}
    static std::unique_ptr<MockFFT> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        int c;
        std::unique_ptr<Source> src;
        str >> expect('(') >> skipws >> c >> expect(')') >> skipws;

        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
        return std::make_unique<MockFFT>((size_t)c, src.get());
    }
};


class FFTPass : public FFT {
protected:
    Interpolated<double> character = {1.0};
    std::vector<double> char_precalc = {};

    void ModifyFFT(fftw_complex* fft, size_t size) override {
        for(size_t i = 0; i < size; i++) {
            fft[i][0] *= char_precalc[i];
            fft[i][1] *= char_precalc[i];
        }
    }
public:

    FFTPass() : FFT() {/*nothing*/}
    FFTPass(const FFTPass& f) : FFT(f), character(f.character), char_precalc(f.char_precalc) {/*nothing*/}
    FFTPass(size_t resolution, Source* src) : FFT(resolution, src) {/*nothing*/}
    FFTPass(Interpolated<double>& characteristics, int srate, size_t resolution, Source* src) : FFT(resolution, src), character(characteristics) {
        size_t result_size = resolution / 2 + 1;
        char_precalc.resize(result_size);
        for(size_t i = 0; i < result_size; i++) {
            char_precalc[i] = character.Get(i*(double)srate/resolution);
        }
    }

    std::unique_ptr<Source> copy() override {return std::make_unique<FFTPass>(*this);}
    FFTPass& operator=(const FFTPass&) = default;
    std::string ToString() const override {return Filter::ToString() + "FFTPass"; }
    std::string GetNameAndParams() const override {return "mockfft(" + std::to_string((size_t)fft_size) + ")";}
    static std::unique_ptr<FFTPass> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        Interpolated<double> character;
        int fft_size = 4096;
        std::unique_ptr<Source> src;
        str >> expect('(') >> skipws >> character >> skipws;

        if(isdigit(str.peek())) { str >> fft_size; }

        str >> expect(')') >> skipws;

        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
        return std::make_unique<FFTPass>(character, srate, (size_t)fft_size, src.get());
    }
};


class AddFrequencyFilter : public FFT {
protected:
    Interpolated<double> sine_factor = {0.0};
    Interpolated<double> cosine_factor = {0.0};
    std::vector<double> sine_precalc = {};
    std::vector<double> cosine_precalc = {};

    void ModifyFFT(fftw_complex* fft, size_t size) override {
        for(size_t i = 0; i < size; i++) {
            fft[i][0] += sine_precalc[i];
            fft[i][1] += cosine_precalc[i];
        }
    }
public:

    AddFrequencyFilter() : FFT() {}
    AddFrequencyFilter(const AddFrequencyFilter& f) : FFT(f), sine_factor(f.sine_factor), cosine_factor(f.cosine_factor), sine_precalc(f.sine_precalc), cosine_precalc(f.cosine_precalc) {}
    AddFrequencyFilter(size_t resolution, Source* src) : FFT(resolution, src) {}
    AddFrequencyFilter(Interpolated<double>& sine, Interpolated<double>& cosine, int srate, size_t resolution, Source* src) : FFT(resolution, src), sine_factor(sine), cosine_factor(cosine) {
        size_t result_size = resolution / 2 + 1;
        sine_precalc.resize(result_size);
        cosine_precalc.resize(result_size);
        for(size_t i = 0; i < result_size; i++) {
            sine_precalc[i] =   sine_factor.Get(i*(double)srate/resolution)*result_size;
            cosine_precalc[i] = cosine_factor.Get(i*(double)srate/resolution)*result_size;
        }
    }

    std::unique_ptr<Source> copy() override {return std::make_unique<AddFrequencyFilter>(*this);}
    AddFrequencyFilter& operator=(const AddFrequencyFilter&) = default;
    std::string ToString() const override {return Filter::ToString() + "AddFrequencyFilter"; }
    std::string GetNameAndParams() const override {return "mockfft(" + std::to_string((size_t)fft_size) + ")";}
    static std::unique_ptr<AddFrequencyFilter> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        Interpolated<double> sine_character;
        Interpolated<double> cosine_character;
        int fft_size = 4096;
        std::unique_ptr<Source> src;
        str >> expect('(') >> skipws >> sine_character >> skipws >> cosine_character >> skipws;

        if(isdigit(str.peek())) { str >> fft_size; }

        str >> expect(')') >> skipws;

        if(str.peek() == '{'){
            str.get();
            src = Source::Make(str, srate, flags);
            str >> expect('}');
        }
        return std::make_unique<AddFrequencyFilter>(sine_character, cosine_character, srate, (size_t)fft_size, src.get());
    }
};
