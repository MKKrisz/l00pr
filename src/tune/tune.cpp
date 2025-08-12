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

Tune::Tune() : m_lanes(), p_sources(), p_refs(), p_labeled() {}

Tune::Tune(NotePlayer& p, NoteStream& s) : m_lanes(), p_sources(), p_refs(), p_labeled() {
    m_lanes.push_back(Lane(p, s));
}

Tune::Tune(Lane& p) : m_lanes(), p_sources(), p_refs(), p_labeled() {
    m_lanes.push_back(p);
}

Tune::Tune(const Tune& t) : m_lanes(t.m_lanes), p_sources(), p_refs(t.p_refs), p_labeled(t.p_labeled), p_globalFilter((std::unique_ptr<Filter>&&)std::move(t.p_globalFilter->copy())), m_bpm(t.m_bpm), m_srate(t.m_srate), m_polynote(t.m_polynote) {
    for(auto& s : t.p_sources) {
        p_sources.emplace_back(s->copy());
    }
}

template <std::ranges::range T>
    requires std::same_as<std::ranges::range_value_t<T>, Lane>
Tune::Tune(T data) : m_lanes(), p_sources(), p_refs(), p_labeled() {
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
        p_sources.push_back(Source::Make(stream, m_srate));
        return;
    }
    if(!multiple)
        std::cout << "Warning: 'generator' specified, but using multiple generator syntax." << std::endl;
    stream.get();
    while((stream >> skipws).peek() != '}') {
        p_sources.push_back(Source::Make(stream, m_srate));
    }
    stream.get();
}
void Tune::SetGen(std::istream& str, Tune* t) {
    t->setGen(str);
}

Source* Tune::getSourceByName(const std::string& name, const std::string& err) {
    try {
        return Source::getByName(p_sources, name);
    } catch(const std::out_of_range& ) {
        auto ref = std::make_unique<SourceRef>(name, err);
        SourceRef* ret = ref.get();
        p_refs.emplace_back(ret);
        p_sources.push_back(std::move(ref));
        return ret;
    }
}
std::vector<Source*> Tune::getSources() {
    std::vector<Source*> ret {};
    ret.reserve(p_sources.size());
    for(const auto& s : p_sources) {
        ret.emplace_back(s.get());
    }
    return ret;
}

Source* Tune::getSource(std::istream& stream) {
    stream >> skipws;
    if(stream.peek() != '(') { return nullptr; }
    stream.get();
    if(stream.peek() == ')') { return nullptr; }
    stream >> skipws;

    Source* gen = nullptr;

    if(isdigit(stream.peek())) {
        size_t genId;
        stream >> genId;
        if(genId >= p_sources.size()) {
            auto ref = std::make_unique<SourceRef>(genId, "The ID of the player source (" + std::to_string(genId) + ") is larger than the number of loaded generators. Maybe you're trying to load a multifile project?");
            gen = ref.get();
            p_refs.emplace_back(ref.get());
            p_sources.push_back(std::move(ref));
        }
        else {
            gen = p_sources[genId].get();
        }
        stream >> expect(')');
    }
    else {
        auto pos = stream.tellg();
        try {
            auto temp = Source::Make(stream, m_srate);
            gen = temp.get();
            addSource(std::move(temp));
            stream >> expect(')');
        } catch(std::exception& err) {
            stream.seekg(pos);
            std::string name;
            std::getline(stream, name, ')');
            gen = getSourceByName(trim(name), err.what());
        }
    }
    return gen;
}

void Tune::addLane(std::istream& stream) {
    Source* gen = getSource(stream);
    if(gen == nullptr) { gen = p_sources[0].get();}


    NoteStream str;

    if((stream >> skipws).peek() == '{') {
        stream.get();
        str = NoteStream(stream, this, m_bpm, m_polynote, m_srate);
        stream >> expect('}');    
    }

    m_lanes.emplace_back(Lane(NotePlayer(gen), str));
}
void Tune::AddLane(std::istream& str, Tune* t) {
    t->addLane(str);
}

void Tune::resolveReferences() {
    // fetch all (potentially missing) references and labels
    for(auto& s : p_sources) {
        //fetch
        auto s_refs = s->getSourceRefs();
        auto s_labels = s->getLabeled();

        //insert
        p_refs.insert(p_refs.begin(), s_refs.begin(), s_refs.end());
        for(const auto& l : s_labels) {
            bool success = p_labeled.emplace(l->label(), l).second;
            if(!success) { throw std::runtime_error("Multiple sources with the same label aren't allowed! (label: " + l->label() + ")"); }
        }
    }
    auto s_refs = p_globalFilter->getSourceRefs();
    auto s_labels = p_globalFilter->getLabeled();

    //insert
    p_refs.insert(p_refs.begin(), s_refs.begin(), s_refs.end());
    for(const auto& l : s_labels) {
        bool success = p_labeled.emplace(l->label(), l).second;
        if(!success) { throw std::runtime_error("Multiple sources with the same label aren't allowed! (label: " + l->label() + ")"); }
    }
    for(auto& r : p_refs) {
        if(r->resolved()) continue;
        auto label = r->getWanted();
        if(!r->labeled()) {
            if(label.second >= p_sources.size()) {
                throw std::out_of_range(r->getError());
            }
            r->setSource(p_sources[label.second].get());
            continue;
        }
        try {
            auto src = p_labeled.at(label.first);
            r->setSource(src);
        } catch (std::out_of_range& err) {
            throw std::out_of_range(label.first);//"Unresolved reference: No source labeled as :" + label.first + ": was found!\nIf you tried to create a source inplace, here is the error for that:\n\n" + r->getError());
        }
    }
}

double Tune::getSample(double srate, bool print) {
    double sum = 0;
    bool hasNewNotes = false;
    for(auto& l : m_lanes) {
        std::vector<std::unique_ptr<Note>> newNotes = l.stream.GetStartingNotes(t);
        if(print && !newNotes.empty()) 
            hasNewNotes = true;
        for(size_t i = 0; i < newNotes.size(); i++) {
            if(print) std::cout << newNotes[i]->ToString() << "    ";
            l.player.addNote(std::move(newNotes[i]));
        }
        sum += l.player.getSample(srate);
    }
    if(print && hasNewNotes) std::cout << std::endl;

    //if(p_globalFilter != nullptr) {         // this should never happen
    p_globalFilter->addSample(sum);
    //}
    t += 1.0f/srate;
    return p_globalFilter->getSample(srate);
}

double Tune::getLen() {
    double max = 0;
    for(auto& l : m_lanes) {
        if(max < l.stream.getLen())
            max = l.stream.getLen();
    }
    return max;
}
void Tune::addSource(std::unique_ptr<Source> src) {
    p_sources.emplace_back(std::move(src));
}


void Tune::Write(std::ostream& str) const {
    str << "set bpm: 60" << std::endl;   //Hard-coded because at the point of writing everything is converted into seconds
    str << "set samplerate: " << m_srate << std::endl;

    str << "set globalfilter: ";
    p_globalFilter->Write(str);
    str << std::endl;

    str << "generators {" << std::endl;
    for(const auto& src : p_sources) {
        if(src->label() != "") {
            str << ':' << src->label() << ": ";
        }
        src->Write(str);
        str << std::endl;
    }
    str << '}' << std::endl;

    for(const auto& lane : m_lanes) {
        str << "player(";

        if(lane.player.getSrc()->label() != "") {
            str << lane.player.getSrc()->label();
        }
        else {
            lane.player.getSrc()->Write(str);
        }
        str << ") {" << std::endl;
        lane.stream.Write(str);
        str << std::endl << '}' << std::endl;
    }
}
