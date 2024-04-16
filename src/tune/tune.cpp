#include "tune.hpp"
#include "../exceptions/parse_error.hpp"

#include <iostream>
#include <iomanip>

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
    stream >> skipws;
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
            if(buf == "generator") {
                t.setGen(stream);
                break;
            }
            if(buf == "player") {
                t.addLane(stream);
                break;
            }
        }
        buf.clear();
        stream >> skipws;
    }
    
    for(int i = 0; i < t.getLaneCount(); i++) {
        NoteStream& stream = t.getLane(i).stream;
        for(size_t j = 0; j < stream.getSetterSize(); j++) {
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
    stream >> skipws;
    std::string buf;
    while(true) {
        buf += tolower(stream.get());
        if(buf == "bpm") {
            if((stream >> skipws).peek() != ':') { 
                bpm = 60; break;
            }
            stream.get();
            stream >> bpm;
            break;
        }
        if(buf == "samplerate") {
            if((stream >> skipws).peek() != ':') { 
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
        if(buf == "poly") {
            polynote = true;
            break;
        }
    }
}

void Tune::setGen(std::istream& stream) {
    bool multiple = false;

    if(stream.peek() == 's') {
        multiple = true;
        stream.get();
    }
    stream >> skipws;

    if(stream.peek() != '{') {
        if(multiple)
            std::cout << "Warning: 'generators' specified, but using single generator syntax." << std::endl;
        Generator* gen;
        stream >> &gen;
        generators.emplace_back(gen);
        return;
    }
    if(!multiple)
        std::cout << "Warning: 'generator' specified, but using multiple generator syntax." << std::endl;
    stream.get();
    while((stream >> skipws).peek() != '}') {
        Generator* gen;
        stream >> &gen;
        generators.emplace_back(gen);
    }
    stream.get();
}

void Tune::addLane(std::istream& stream) {
    stream >> skipws;
    int genId = 0;
    NoteStream str = NoteStream();
    if(stream.peek() == '(') {
        stream.get();
        stream >> genId;
        if((stream >> skipws).peek() != ')') {
            throw parse_error(stream, "No closing ')' for player generator definition");
        }
        stream.get();
    }
    if((stream >> skipws).peek() == '{') {
        stream.get();
        str.setPolynote(polynote);
        str.setBpm(bpm);
        stream >> str;
        if((stream >> skipws).peek() != '}') {
            throw parse_error(stream, "No closing '}' for note stream definition");
        }
        stream.get();
    }
    lanes.push_back(Lane(NotePlayer(generators[genId]), str));
}

double Tune::getSample(double srate) {
    double sum = 0;
    bool newNotes = false;
    for(auto& l : lanes) {
        std::vector<Note> newPNotes = l.stream.GetStartingPlayableNotes(t);
        if(!newPNotes.empty()) 
            newNotes = true;
        std::vector<SetterNote> newSNotes = l.stream.GetStartingSetterNotes(t);
        for(size_t j = 0; j < newPNotes.size(); j++) {
            std::cout << newPNotes[j] << "    ";
            l.player.addNote(newPNotes[j]);
        }
        for(size_t j = 0; j < newSNotes.size(); j++) {
            std::cout << "New Generator!!" <<std::endl;
            l.player.addNote(newSNotes[j]);
        }
        sum += l.player.getSample(srate);
    }
    if(newNotes) std::cout << std::endl;
    t += 1/srate;
    return sum;
}

double Tune::getLen() {
    double max = 0;
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
