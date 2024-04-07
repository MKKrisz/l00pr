#include "tune.hpp"
#include "../exceptions/parse_error.hpp"

#include <iostream>

Tune::Tune() : lanes(), generators() {}

Tune::Tune(NotePlayer& p, NoteStream& s) : lanes() {
    lanes.push_back(Lane(p, s));
}

Tune::Tune(Lane& p) : lanes() {
    lanes.push_back(p);
}

template <std::ranges::range T>
    requires std::same_as<std::ranges::range_value_t<T>, Lane>
Tune::Tune(T data) : lanes() {
    for(auto d : data) {
        lanes.push_back(d);
    }
}


std::istream& operator>>(std::istream& stream, Tune& t) {
    stream >> std::ws;
    while(stream.good()) {
        std::string buf = "";
        while(true) {
            if(stream.peek() == 0) 
                throw parse_error(stream, "Something is not good...");
            buf += tolower(stream.get());
            if(buf == "set") {
                t.setEnv(stream);
                break;
            }
            if(buf == "generators") {
                t.setGen(stream);
                break;
            }
            if(buf == "player") {
                t.addLane(stream);
                break;
            }
        }
        buf.clear();
        stream >> std::ws;
    }
    
    for(int i = 0; i < t.getLaneCount(); i++) {
        NoteStream& stream = t.getLane(i).stream;
        for(int j = 0; j < stream.getSetterSize(); j++) {
            SetterNote& note = stream.getSetterNote(j);
            int id = note.getId();
            if(note.getGen() != nullptr) continue;
            if(id < -1 || id > t.getGenCount()) {
                throw std::runtime_error(
                        "Index out of range at setter note no. " 
                        + std::to_string(j));
            }
            if(id != -1)
                note.setGen(t.getGenerator(id));
        }
    }

    return stream;
}


// syntax: set bpm: 120    -- sets bpm to 120
// syntax: set bpm         -- sets bpm to 60
void Tune::setEnv(std::istream& stream) {
    stream >> std::ws;
    std::string buf;
    while(true) {
        buf += tolower(stream.get());
        if(buf == "bpm") {
            if((stream >> std::ws).peek() != ':') { 
                bpm = 60; break;
            }
            stream.get();
            stream >> bpm;
            break;
        }
        if(buf == "samplerate") {
            if((stream >> std::ws).peek() != ':') { 
                srate = 48000; break;
            }
            stream.get();
            stream >> srate;
            break;
        }
        if(buf == "nopoly") {
            polynote = false;
            break;
        }
    }
}

void Tune::setGen(std::istream& stream) {
    stream >> std::ws;
    if(stream.peek() != '{') {
        Generator* gen;
        stream >> &gen;
        generators.emplace_back(gen);
        return;
    }
    stream.get();
    while((stream >> std::ws).peek() != '}') {
        Generator* gen;
        stream >> &gen;
        generators.emplace_back(gen);
    }
    stream.get();
}

void Tune::addLane(std::istream& stream) {
    stream >> std::ws;
    int genId = 0;
    NoteStream str = NoteStream();
    if(stream.peek() == '(') {
        stream.get();
        stream >> genId;
        if((stream >> std::ws).peek() != ')') {
            throw parse_error(stream, "No closing ')' for player generator definition");
        }
        stream.get();
    }
    if((stream >> std::ws).peek() == '{') {
        stream.get();
        str.setPolynote(polynote);
        str.setBpm(bpm);
        stream >> str;
        if((stream >> std::ws).peek() != '}') {
            throw parse_error(stream, "No closing '}' for note stream definition");
        }
        stream.get();
    }
    lanes.push_back(Lane(NotePlayer(generators[genId]), str));
}

float Tune::getSample(float srate) {
    float sum = 0;

    for(auto& l : lanes) {
        std::vector<Note> newPNotes = l.stream.GetStartingPlayableNotes(t);
        std::vector<SetterNote> newSNotes = l.stream.GetStartingSetterNotes(t);
        for(int j = 0; j < newPNotes.size(); j++) {
            std::cout << newPNotes[j].getFreq() << ' ' << newPNotes[j].getLen() <<std::endl;
            l.player.addNote(newPNotes[j]);
        }
        for(int j = 0; j < newSNotes.size(); j++) {
            std::cout << "New Generator!!" <<std::endl;
            l.player.addNote(newSNotes[j]);
        }
        sum += l.player.getSample(srate);
    }
    t += 1/srate;
    return sum;
}

float Tune::getLen() {
    float max = 0;
    for(auto l : lanes) {
        if(max < l.stream.getLen())
            max = l.stream.getLen();
    }
    return max;
}

Tune::~Tune() {
    for(auto g : generators)
        delete g;
}
