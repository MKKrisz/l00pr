#include "tune.hpp"
#include "../player/noteplayer.hpp"
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

Tune::Tune() : m_lanes(), p_sources() {}

Tune::Tune(NotePlayer& p, NoteStream& s) : m_lanes() {
    m_lanes.push_back(Lane(p, s));
}

Tune::Tune(Lane& p) : m_lanes() {
    m_lanes.push_back(p);
}

Tune::Tune(const Tune& t) : m_lanes(t.m_lanes), p_sources(), p_globalFilter((std::unique_ptr<Filter>&&)std::move(t.p_globalFilter->copy())), m_bpm(t.m_bpm), m_srate(t.m_srate), m_polynote(t.m_polynote) {
    for(auto& s : t.p_sources) {
        p_sources.emplace_back(s->copy());
    }
}

template <std::ranges::range T>
    requires std::same_as<std::ranges::range_value_t<T>, Lane>
Tune::Tune(T data) : m_lanes() {
    for(auto d : data) {
        m_lanes.push_back(d);
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
        p_sources.emplace_back(AudioSource::Make(stream, m_srate));
        return;
    }
    if(!multiple)
        std::cout << "Warning: 'generator' specified, but using multiple generator syntax." << std::endl;
    stream.get();
    while((stream >> skipws).peek() != '}') {
        p_sources.emplace_back(AudioSource::Make(stream, m_srate));
    }
    stream.get();
}
void Tune::SetGen(std::istream& str, Tune* t) {
    t->setGen(str);
}

AudioSource* Tune::getSourceByName(std::string name) {
    try {
        return AudioSource::getByName(p_sources, name);
    } catch(const std::out_of_range& ) {
        return nullptr;
    }
}
std::vector<AudioSource*> Tune::getSources() {
    std::vector<AudioSource*> ret {};
    ret.reserve(p_sources.size());
    for(const auto& s : p_sources) {
        ret.emplace_back(s.get());
    }
    return ret;
}

void Tune::addLane(std::istream& stream) {
    stream >> skipws;
    AudioSource* gen = p_sources[0].get();
    bool owner = false;
    NoteStream str;
    bool hasNotes = false;
    if(stream.peek() == '(') {
        stream.get();
        int genId;
        stream >> skipws;
        if(isdigit(stream.peek())) {
            stream >> genId;
            gen = p_sources[genId].get();
            stream >> expect(')');
        }
        else {
            auto pos = stream.tellg();
            std::string name;
            std::getline(stream, name, ')');
            gen = getSourceByName(name);
            if(gen == nullptr) {
                stream.seekg(pos);
                gen = AudioSource::Make(stream, m_srate).release();
                owner = true;
                stream >> expect(')');
            }
        }
    }
    if((stream >> skipws).peek() == '{') {
        stream.get();
        str = NoteStream(stream, getSources(), m_bpm, m_polynote, m_srate);
        if(str.size() > 0) hasNotes = true;
        stream >> expect('}');    
    }
    if(!hasNotes)
        throw parse_error(stream, std::string("No notes to play.\n Player format: player(generator_id){note1 note2 ... note_n}\nNote format: <") + (m_polynote? "time" : "") +" frequency length amplitude>");

    m_lanes.emplace_back(Lane(NotePlayer(gen), str));
    if(owner) delete gen;
}
void Tune::AddLane(std::istream& str, Tune* t) {
    t->addLane(str);
}

double Tune::getSample(double srate, bool print) {
    double sum = 0;
    bool hasNewNotes = false;
    for(auto& l : m_lanes) {
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

    //if(p_globalFilter != nullptr) {         // this should never happen
    p_globalFilter->addSample(sum);
    //}
    t += 1.0f/srate;
    return p_globalFilter->calc();
}

double Tune::getLen() {
    double max = 0;
    for(auto& l : m_lanes) {
        if(max < l.stream.getLen())
            max = l.stream.getLen();
    }
    return max;
}


void Tune::Write(std::ostream& str) const {
    str << "set bpm: 60" << std::endl;   //Hard-coded because at the point of writing everything is converted into seconds
    str << "set samplerate: " << m_srate << std::endl;

    str << "set globalfilter: ";
    p_globalFilter->Write(str);
    str << std::endl;

    str << "generators {" << std::endl;
    for(const auto& src : p_sources) {
        if(src->name != "") {
            str << ':' << src->name << ": ";
        }
        src->Write(str);
        str << std::endl;
    }
    str << '}' << std::endl;

    for(const auto& lane : m_lanes) {
        str << "player(";

        if(lane.player.getSrc()->name != "") {
            str << lane.player.getSrc()->name;
        }
        else {
            lane.player.getSrc()->Write(str);
        }
        str << ") {" << std::endl;
        lane.stream.Write(str);
        str << std::endl << '}' << std::endl;
    }
}
