#include "util.hpp"
#include "exceptions/parse_error.hpp"

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>

double getFreq(std::istream& stream, int transpose) {
    //skips whitespace then checks if next char is a number
    if(isdigit((stream >> skipws).peek()) || stream.peek() == '-') {
        double ret;
        stream >> ret;
        return ret;
    }
    char buf;
    int octave;
    int note;
    stream >> buf;
    
    if(buf == 'H') 
        throw parse_error(stream, "Try using B in place of H. (H becomes B; B becomes Bb)");
    if(buf < 'A' || buf > 'G')
        throw parse_error(stream, "Couldn't interpret stream as a note or float");

    //same thing as above except in a tightly packed ternary-chain
    note = buf > 'E' ? (buf-'A')*2 - 2 : 
           buf > 'B' ? (buf-'A')*2 - 1 : (buf-'A')*2;

    //handling the modifier
    buf = stream.peek();
    if(buf == '#')      { note++;  stream.get(); }
    else if(buf == 'b') { note--;  stream.get(); }
    else if(buf == 'x') { note+=2; stream.get(); }

    if(isdigit(stream.peek())){
        stream >> octave;
        if(note > 2) octave--;
    }
    else throw parse_error(stream, "Couldn't interpret stream as a note or float");
    
    // A4 is 440Hz (pls don't argue, accept that it is)
    // In equal temperament the quotient of adjacent notes is 2^(1/12)
    // from that it is easy to conclude that the formula for caluclating the 
    // frequency of the given note is (considering our reference point of A4):
    //
    // 440 * 2^(octave - 4 + (note/12))
    //
    return 440 * pow(2.0f, (octave-4) + (note + transpose)/12.0f);
}


std::pair<int, int> getLineColNo(std::istream& stream) {
    // clear the bad bit
    stream.clear();
    int pos = stream.tellg();
    stream.seekg(0);
    stream >> std::noskipws;

    int lineno = 1;
    int colno = 1;
    char buf;
    while(stream.tellg() < pos && stream>>buf) {
        if(buf == '\n') {
            lineno++; 
            colno = 1;
            continue;
        } 
        colno++;
    }
    return std::make_pair(lineno, colno);
}

std::istream& skipws(std::istream& str) {
    while((str >> std::ws).peek() == '#') {
        char c;
        while(str.get(c) && c != '\n') {}
    }
    return str;
}

exp_p expect(char c) {return exp_p{c};}
std::istream& operator>>(std::istream& str, const exp_p& c) {
    str >> skipws;
    if(str.peek() != c.val)
        throw parse_error(str, std::string("Expected '") + c.val + "'");
    str.get();
    return str;
}

std::string trim(const std::string& str) {
    const std::string spaces = " \f\n\r\t\v";
    const auto strBegin = str.find_first_not_of(spaces);
    if (strBegin == std::string::npos)
        return "";

    const auto strEnd = str.find_last_not_of(spaces);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

uint32_t reverse(uint32_t x)
{
    x = ((x & 0x55555555) <<  1) | ((x & 0xAAAAAAAA) >>  1);
    x = ((x & 0x33333333) <<  2) | ((x & 0xCCCCCCCC) >>  2);
    x = ((x & 0x0F0F0F0F) <<  4) | ((x & 0xF0F0F0F0) >>  4);
    x = ((x & 0x00FF00FF) <<  8) | ((x & 0xFF00FF00) >>  8);
    x = ((x & 0x0000FFFF) << 16) | ((x & 0xFFFF0000) >> 16);
    return x;
}



// Function to calculate FT fast
// Input: src, dst: array of complex numbers ( { R1; I1; R2; I2; R3; I3; ... Rn; In } )
//        len: number of complex numbers in src (so array.size()/2)
//        stride: number of complex numbers to jump in internal loop
bool FourierTransform(const double* src, size_t len, double* dst, size_t stride) {
    if (len == 1) { dst[0] = src[0]; dst[1] = src[1]; return true; }
    if (len % 2 != 0) {return false;}
    if (len == 0) { return false; }

    FourierTransform(src, len/2, dst, 2*stride);
    FourierTransform((src + stride), len/2, (dst + len), 2*stride);

    for(size_t i = 0; i < len; i+=2) {
        double p_real = dst[i];
        double p_imag = dst[i + 1];
        double q_real = cos(2.0*M_PI*i/len) * dst[i + len]  -  (-sin(2.0*M_PI*i/len)) * dst[i + 1 + len];
        double q_imag = cos(2.0*M_PI*i/len) * dst[i + 1 + len] + (-sin(2.0*M_PI*i/len)) * dst[i + len];
        dst[i] =           p_real + q_real;
        dst[i + 1] =       p_imag + q_imag;
        dst[i + len] =     p_real - q_real;
        dst[i + 1 + len] = p_imag - q_imag;
    }
    return true;

    /*if (len % 2 != 0) { return false; }

    uint32_t log2len = log2(len);

    for (uint32_t i = 0; i < len; i++) {
        uint32_t reverse_i = reverse(i) >> (32-log2len);
        dst[2*reverse_i] = src[2*i];
        dst[2*reverse_i + 1] = src[2*i + 1];
    }
    for (uint32_t step = 1; step < log2len; step++) {
        uint32_t m = 1<<step;
        double omega_m_real = cos(-2*M_PI/m);
        double omega_m_imag = sin(-2*M_PI/m);
        for (size_t i = 0; i < len; i += m) {
            double omega_real = 1;
            double omega_imag = 0;
            for(size_t j = 0; j < m/2; j++) {
                double t_real = omega_real * dst[2*i + 2*j + m] - omega_imag * dst[2*i + 2*j + m + 1];
                double t_imag = omega_real * dst[2*i + 2*j + m + 1] + omega_imag * dst[2*i + 2*j + m];
                double u_real = dst[2*i + 2*j];
                double u_imag = dst[2*i + 2*j + 1];

                dst[2*i + 2*j] =         u_real + t_real;
                dst[2*i + 2*j + 1] =     u_imag + t_imag;
                dst[2*i + 2*j + m] =     u_real - t_real;
                dst[2*i + 2*j + m + 1] = u_imag - t_imag;

                omega_real = omega_real * omega_m_real - omega_imag * omega_m_imag;
                omega_imag = omega_real * omega_m_imag + omega_imag * omega_m_real;
            }
        }
    }
    return true;*/
}

bool InverseFourierTransform(const double* src, size_t len, double* dst, size_t stride) {
    if (len == 1) { dst[0] = src[0]; dst[1] = src[1]; return true; }
    if (len % 2 != 0) {return false;}
    if (len == 0) { return false; }

    InverseFourierTransform(src, len/2, dst, 2*stride);
    InverseFourierTransform((src + stride), len/2, (dst + len), 2*stride);

    for(size_t i = 0; i < len; i+=2) {
        double p_real = dst[i];
        double p_imag = dst[i + 1];
        double q_real = cos(2*M_PI*i/len) * dst[i + len]  -  (sin(2*M_PI*i/len)) * dst[i + 1 + len];
        double q_imag = cos(2*M_PI*i/len) * dst[i + 1 + len] + (sin(2*M_PI*i/len)) * dst[i + len];
        dst[i] =           (p_real + q_real);
        dst[i + 1] =       (p_imag + q_imag);
        dst[i + len] =     (p_real - q_real);
        dst[i + 1 + len] = (p_imag - q_imag);
    }
    
    /*if (len % 2 != 0) { return false; }

    uint32_t log2len = log2(len);

    for (uint32_t i = 0; i < len; i++) {
        uint32_t reverse_i = reverse(i) >> (32-log2len);
        dst[2*reverse_i] = src[2*i];
        dst[2*reverse_i + 1] = src[2*i + 1];
    }
    for (uint32_t step = 1; step < log2len; step++) {
        uint32_t m = 1<<step;
        double omega_m_real = cos(2*M_PI/m);
        double omega_m_imag = sin(2*M_PI/m);
        for (size_t i = 0; i < len; i += m) {
            double omega_real = 1;
            double omega_imag = 0;
            for(size_t j = 0; j < m/2; j++) {
                double t_real = omega_real * dst[2*i + 2*j + m] - omega_imag * dst[2*i + 2*j + m + 1];
                double t_imag = omega_real * dst[2*i + 2*j + m + 1] + omega_imag * dst[2*i + 2*j + m];
                double u_real = dst[2*i + 2*j];
                double u_imag = dst[2*i + 2*j + 1];

                dst[2*i + 2*j] =         u_real + t_real;
                dst[2*i + 2*j + 1] =     u_imag + t_imag;
                dst[2*i + 2*j + m] =     u_real - t_real;
                dst[2*i + 2*j + m + 1] = u_imag - t_imag;

                omega_real = omega_real * omega_m_real - omega_imag * omega_m_imag;
                omega_imag = omega_real * omega_m_imag + omega_imag * omega_m_real;
            }
        }
    }*/

    for(size_t i = 0; i < len; i++) {
        dst[i] /= len;
    }
    return true;
}
