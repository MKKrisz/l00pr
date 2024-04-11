#include "random_note.hpp"
#include "../exceptions/parse_error.hpp"

RandomNote::RandomNote(std::istream& str, double bpm = 60) : 
    bpm(bpm), frequencies(), lengths(), notes(){
    //random|( (A4, C5, E5))
    notes.setBpm(bpm);
    notes.setPolynote(false);

    if((str >> skipws).peek() == '(') {
        str.get();                      //random(| (A4, C5, E5))
        str >> skipws;                  //random( |(A4, C5, E5))
        int pos = str.tellg();
        if(str.peek() == '(') {
            str.get();
            Interpolated<double> val;
            str >> val;
            frequencies.emplace_back(val);
            if((str >> skipws).peek() == '-') {
                str.get();
                val.Clear();
                str >> skipws >> val;
                frequencies.emplace_back(val);
            }
            else {
                fixFreq = true;
                while(isalpha((str >> skipws).peek())) {
                    val.Clear();
                    str >> val;
                    frequencies.emplace_back(val);
                }
            }
            if((str >> skipws).peek() != ')')
                throw parse_error(str, "Expected ')'");
            str.get();
            if((str >> skipws).peek() == ')') {
                throw parse_error(str, "Sequence length cannot be a range");
            }
        }
        else {
            Interpolated<double> val;
            str >> val;
            if((str >> skipws).peek() == ')') {
                if(val.Size() > 1) {
                    throw parse_error(str, "Sequence length cannot be interpolated");
                }
                len = val.Get(0);
                noteBased = true;
                str.get();
            }
            else {
                fixFreq = true;
                frequencies.emplace_back(val);
            }
        }
        str >> skipws;
        if(!noteBased) {

            str >> len;

            if((str >> skipws).peek() == '(') {
                str.get();
                double val;
                str >> val;
                lengths.emplace_back(val);
                if((str >> skipws).peek() == '-') {
                    str.get();
                    str.get();
                    str >> val;
                    lengths.emplace_back(val);
                }
                else {
                    fixLen = true;
                    while(isdigit((str >> skipws).peek())) {
                        str >> val;
                        lengths.emplace_back(val);
                    }
                }
                if((str >> skipws).peek() != ')')
                    throw parse_error(str, "Expected ')'");
                str.get();
            }
            else {
                double val;
                str >> val;
                fixLen = true;
                lengths.emplace_back(val);
            }

            str >> skipws;
            if(str.peek() != ')') {
                amplitudes.clear();
                if((str >> skipws).peek() == '(') {
                    str.get();
                    Interpolated<double> val;
                    str >> val;
                    amplitudes.emplace_back(val);
                    if((str >> skipws).peek() == '-') {
                        str.get();
                        val.Clear();
                        str >> val;
                        amplitudes.emplace_back(val);
                    }
                    else {
                        fixAmpl = true;
                        while(isdigit((str >> skipws).peek())) {
                            val.Clear();
                            str >> val;
                            amplitudes.emplace_back(val);
                        }
                    }
                    if((str >> skipws).peek() != ')')
                        throw parse_error(str, "Expected ')'");
                    str.get();
                }
                else {
                    Interpolated<double> val;
                    str >> val;
                    fixAmpl = true;
                    amplitudes.emplace_back(val);
                }
            }
            if((str >> skipws).peek() != ')')
                throw parse_error(str, "Expected ')'");
            str.get();
        }
        else {
            if((str >> skipws).peek() != '{')
                throw parse_error(str, "No notes specified for random note sequence to choose from");
            str >> notes;
            if((str >> skipws).peek() != '}')
                throw parse_error(str, "Expected '}'");
            str.get();
        }
    }
}

std::vector<std::pair<double, Note>> RandomNote::Serialize(double start) {
    std::vector<std::pair<double, Note>> ret = {};
    double t = 0;
    if(noteBased) {
        while(t < len) {
            size_t id = rand() % notes.getPlayableSize();
            Note n = notes.getPlayableNote(id);
            double len = n.getLen();
            ret.emplace_back(start + t, n);
            t += len;
        }
    }
    else {
        while(t < len) {
            Interpolated<double> freq;
            double length;
            Interpolated<double> ampl;
            int id;
            if(fixFreq) {
                id = rand() % frequencies.size();
                freq = frequencies[id];
            }
            else 
                freq = lerp(frequencies[0][0].second, frequencies[1][0].second, double(rand())/RAND_MAX);

            if(fixLen) {
                id = rand() % lengths.size();
                length = lengths[id];
            }
            else 
                length = lerp(lengths[0], lengths[1], double(rand())/RAND_MAX);

            if(fixFreq) {
                id = rand() % amplitudes.size();
                ampl = amplitudes[id];
            }
            else 
                ampl = lerp(amplitudes[0][0].second, amplitudes[1][0].second, double(rand())/RAND_MAX);
            ret.emplace_back(t + start, Note(length, freq, ampl));
            t += length;
        }
    }
    return ret;
}
