#include "sampled.hpp"

#include <cstring>
#include <fstream>

SampledGenerator::SampledGenerator(std::string filename, 
    Interpolated<double> phase,
    Interpolated<double> amplitude,
    Interpolated<double> offset) : Generator(phase, amplitude, offset), filename(filename){
    parse_file(filename);
}

SampledGenerator::SampledGenerator(const SampledGenerator& g)
    : Generator(g), samples(g.samples), filename(g.filename), timestep(g.timestep) {}

SampledGenerator::SampledGenerator(std::istream& str) {
    char c;
    str >> expect('(');
    while((c = str.peek()) != ')' && !isspace(c)) {
        str.get();
        filename += c;
    }
    Interpolated<double> a[3] = {1, 1, 0.0f};
    shouldBeDefault = false;
    for(int i = 0; i < 3; i++) {
        if((str >> skipws).peek() == ')') {
            break;
        }
        a[i].Clear();
        str >> a[i];
    }
    if((str >> skipws).peek() == ')') {
        shouldBeDefault = true;
        str.get();
    }
    m_phasemul = a[0];
    m_gain = a[1];
    m_phaseoffset = a[2];
    parse_file(filename);
}

double SampledGenerator::getSample(double ph, double t) {
    return 0;
}

void read32(std::istream& stream, unsigned int* var) {
    *var |= stream.get();
    *var |= ((unsigned int)stream.get()) << 8;
    *var |= ((unsigned int)stream.get()) << 16;
    *var |= ((unsigned int)stream.get()) << 24;
}


void read16(std::istream& stream, unsigned int* var) {
    *var |= stream.get();
    *var |= ((unsigned int)stream.get()) << 8;
}

enum BPS {bits8 = 8, bits16 = 16, bits32 = 32};

void read(std::istream& stream, BPS bps, unsigned int* var) {
    switch(bps) {
        case bits8: *var = stream.get(); break;
        case bits16: read16(stream, var); break;
        case bits32: read32(stream, var); break;
        default: throw std::runtime_error("Unsupported bits per sample value");
    }
}

void SampledGenerator::parse_file(const std::string& filename) {
    std::fstream file(filename);
    std::cout << "Loading file: " << filename << std::endl;

    if(file.get() != 'R' || file.get() != 'I' || file.get() != 'F' || file.get() != 'F') {
        throw std::runtime_error("File provided is not a RIFF file");
    }
    
    unsigned int filesize = 0;
    read32(file, &filesize);

    if(file.get() != 'W' || file.get() != 'A' || file.get() != 'V' || file.get() != 'E'
        || file.get() != 'f'|| file.get() != 'm'|| file.get() != 't'|| file.get() != ' ') {
        throw std::runtime_error("File provided is not a WAV file");
    }

    unsigned int format_size = 0;
    read32(file, &format_size);

    if(format_size != 16) throw std::runtime_error("Non-PCM WAV files are unsupported");

    unsigned int format = 0;
    read16(file, &format);

    if(format != 1 && format != 3) throw std::runtime_error("Non-PCM WAV files are unsupported");
    bool fp = format == 3;

    unsigned int channels = 0;
    read16(file, &channels);

    //if(channels != 1) throw std::runtime_error("WAV files with more than 1 channels are unsupported");

    unsigned int sample_rate = 0;
    read32(file, &sample_rate);
    timestep = 1.0/sample_rate;

    unsigned int byte_rate = 0;
    read32(file, &byte_rate);

    unsigned int bps_calculated = 8*byte_rate/channels/sample_rate;
    unsigned int block_alignment = 0;
    read16(file, &block_alignment);

    unsigned int bps_from_file = 0;
    read16(file, &bps_from_file);

    if(bps_from_file != bps_calculated) throw std::runtime_error("Bps wrong? calculated:" + std::to_string(bps_calculated) + " file:" + std::to_string(bps_from_file));
    unsigned int bps = bps_from_file;

    char header[4] = {};
    for(int i = 0; i < 4; i++) {header[i] = file.get();}

    while(strncmp(header, "data", 4)) {
        unsigned int size = 0;
        read32(file, &size);
        file.seekp(size, std::ios_base::seekdir::_S_cur);
        for(int i = 0; i < 4; i++) {header[i] = file.get();}
    }
    
    unsigned int data_size = 0;
    read32(file, &data_size);
    size_t samples = data_size/block_alignment;

    unsigned int val_max = fp ? 1 : 0xffffffff >> (32 - bps + (bps > 8? 1 : 0));
    this->samples.reserve(samples);
    const double mid_point = (bps > 8 ? 0.0 : 0.5);;

    for(size_t i = 0; i < samples; i++) {
        double fpsample = 0;
        for(size_t j = 0; j < channels; j++) {
            unsigned int sample = 0;
            read(file, (BPS)bps, &sample);
            if(!fp && bps > 8 &&bps < 32 && (sample >> (bps-1))) { sample = (0xffffffff << bps) | sample;}

            double fpch = 0;
            
            if(fp) fpch = std::bit_cast<float>(sample);
            else fpch = (((int)sample / (float)val_max) - mid_point);
            fpsample += fpch/channels;
        }


        this->samples.emplace_back(fpsample);
        if(i != 0 && i % sample_rate == 0) std::cout << (i/(float)samples*100) << '%' << std::endl;
    }
    std::cout << "done" << std::endl;
}
