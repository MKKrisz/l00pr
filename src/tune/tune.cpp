#include "tune.hpp"
#include "../exceptions/parse_error.hpp"
#include "set_kwd.hpp"

#include <iostream>
#include <string>

void Tune::Init() {
    Set::Init();
    AddMetadata(Tkwd_Metadata("set", Set::Parse));
    AddMetadata(Tkwd_Metadata("generator", Tune::SetGen));
    AddMetadata(Tkwd_Metadata("player", Tune::AddLane));
}

Tune::Tune() : lanes(), sources() {}

Tune::Tune(NotePlayer& p, NoteStream& s) : lanes() {
    lanes.push_back(Lane(p, s));
}

Tune::Tune(Lane& p) : lanes() {
    lanes.push_back(p);
}

Tune::Tune(const Tune& t) : lanes(t.lanes), sources(), globalFilter(t.globalFilter), bpm(t.bpm), srate(t.srate), polynote(t.polynote) {
    for(AudioSource* s : t.sources) {
        sources.emplace_back(s->copy());
    }
}

template <std::ranges::range T>
    requires std::same_as<std::ranges::range_value_t<T>, Lane>
Tune::Tune(T data) : lanes() {
    for(auto d : data) {
        lanes.push_back(d);
    }
}


std::istream& operator>>(std::istream& stream, Tune& t) {
    while((stream >> skipws).good()) {
        Tune::Parse(stream, &t);
    }
    return stream;
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
        AudioSource* gen = AudioSource::Make(stream, srate);
        sources.emplace_back(gen);
        return;
    }
    if(!multiple)
        std::cout << "Warning: 'generator' specified, but using multiple generator syntax." << std::endl;
    stream.get();
    while((stream >> skipws).peek() != '}') {
        AudioSource* gen = AudioSource::Make(stream, srate);
        sources.emplace_back(gen);
    }
    stream.get();
}
void Tune::SetGen(std::istream& str, Tune* t) {
    t->setGen(str);
}

AudioSource* Tune::getSourceByName(std::string name) {
    try {
        return AudioSource::getByName(sources, name);
    } catch(const std::out_of_range& ) {
        return nullptr;
    }
}

void Tune::addLane(std::istream& stream) {
    stream >> skipws;
    AudioSource* gen = sources[0];
    NoteStream str;
    bool hasNotes = false;
    if(stream.peek() == '(') {
        stream.get();
        int genId;
        stream >> skipws;
        if(isdigit(stream.peek())) {
            stream >> genId;
            gen = sources[genId];
            stream >> expect(')');
        }
        else {
            std::string name;
            std::getline(stream, name, ')');
            gen = getSourceByName(name);
            if(gen == nullptr) {
                throw parse_error(stream, "No audiosource named \"" + name + "\".");
            }
        }
    }
    if((stream >> skipws).peek() == '{') {
        stream.get();
        str = NoteStream(stream, sources, bpm, polynote, srate);
        if(str.size() > 0) hasNotes = true;
        stream >> expect('}');    
    }
    if(!hasNotes)
        throw parse_error(stream, std::string("No notes to play.\n Player format: player(generator_id){note1 note2 ... note_n}\nNote format: <") + (polynote? "time" : "") +" frequency length amplitude>");

    lanes.emplace_back(Lane(NotePlayer(gen), str));
}
void Tune::AddLane(std::istream& str, Tune* t) {
    t->addLane(str);
}

double Tune::getSample(double srate, bool print) {
    double sum = 0;
    bool hasNewNotes = false;
    for(auto& l : lanes) {
        std::vector<Note*> newNotes = l.stream.GetStartingNotes(t);
        if(print && !newNotes.empty()) 
            hasNewNotes = true;
        for(size_t i = 0; i < newNotes.size(); i++) {
            if(print) std::cout << newNotes[i]->ToString() << "    ";
            l.player.addNote(newNotes[i]);
        }
        sum += l.player.getSample(srate);
    }
    if(print && hasNewNotes) std::cout << std::endl;

    //if(globalFilter != nullptr) {         // this should never happen
    globalFilter->addSample(sum);
    //}
    t += 1.0f/srate;
    return globalFilter->calc();
}

double Tune::getLen() {
    double max = 0;
    for(auto& l : lanes) {
        if(max < l.stream.getLen())
            max = l.stream.getLen();
    }
    return max;
}

Tune::~Tune() {
    for(auto g : sources)
        delete g;
}
