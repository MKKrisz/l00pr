#include "random_note.hpp"
#include "playablenote.hpp"
#include "../exceptions/parse_error.hpp"

RandomNote::RandomNote(std::istream& str, const std::vector<AudioSource*>& srcs, double bpm, bool poly, int srate) : 
    frequencies(), lengths(), notes(), bpm(bpm) {
    //random|( (A4, C5, E5))
    notes.setBpm(bpm);
    notes.setPolynote(false);

    if((str >> skipws).peek() == '(') {
        str.get();                      //random(| (A4, C5, E5))
        str >> skipws >> frequencies >> skipws;                  //random( |(A4, C5, E5))
        
        if((str).peek() == ')') {
            if(frequencies[0].Size() > 1) {
                throw parse_error(str, "Sequence length cannot be interpolated");
            }
            len = frequencies[0][0].second.getFreq();
            frequencies = {};
            len *= 60/bpm;
            noteBased = true;
            str.get();
        }
        str >> skipws;
        if(!noteBased) {

            str >> len >> skipws;
            len *= 60/bpm;

            str >> lengths >> skipws; 
            for(size_t i = 0; i < lengths.size(); i++) {
                lengths[i] *= 60/bpm;
            }
            if(str.peek() != ')') {
                amplitudes = {};
                str >> amplitudes >> skipws;
            }
            if((str >> skipws).peek() != ')')
                throw parse_error(str, "Expected ')'");
            str.get();
        }
        else {
            if((str >> skipws).peek() != '{')
                throw parse_error(str, "No notes specified for random note sequence to choose from");
            notes = NoteStream(str, srcs, bpm, poly, srate);
            if((str >> skipws).peek() != '}')
                throw parse_error(str, "Expected '}'");
            str.get();
        }
    }
}

std::vector<std::pair<double, Note*>> RandomNote::Serialize(double start) {
    std::vector<std::pair<double, Note*>> ret = {};
    double t = 0;
    if(noteBased) {
        while(t < len) {
            size_t id = rand() % notes.size();
            Note* n = notes.getNote(id);
            double len = n->GetLen();
            ret.emplace_back(start + t, n->copy());
            t += len;
        }
    }
    else {
        while(t < len) {
            double r = double(rand())/RAND_MAX;
            Interpolated<Frequency> freq = frequencies.get(r);
            r = double(rand())/RAND_MAX;
            double length = lengths.get(r);
            r = double(rand())/RAND_MAX;
            Interpolated<double> ampl = amplitudes.get(r);

            ret.emplace_back(t + start, PlayableNote(length, freq, ampl).copy());
            t += length;
        }
    }
    return ret;
}
