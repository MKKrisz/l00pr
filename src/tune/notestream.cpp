#include "notestream.hpp"
#include "../util.hpp"
#include "../exceptions/parse_error.hpp"
#include "loop.hpp"

NoteStream::NoteStream(Note n) : playable(), setter(){
    playable.push_back(std::make_pair(0, n));
}

NoteStream::NoteStream(double t, Note n) : playable(), setter(){
    playable.push_back(std::make_pair(t, n));
}

NoteStream::NoteStream(std::pair<double, Note> p) : playable(), setter(){
    playable.push_back(p);
}

NoteStream::NoteStream(const NoteStream& p) : playable(p.playable), setter(p.setter){}

/// Returns the index of the keyframe with timestamp "t"
/// In case of failure the frame timestamp of playable[id] will differ from t
int NoteStream::getId(double t) {
    if(playable.size() == 0) return 0;
    int min = 0;
    int max = playable.size()-1;
    int mid = (min + max)/2;
    while(min<=max && !almostEQ(playable[mid].first, t)) {
        if(playable[mid].first < t)
            min = mid+1;
        else
            max = mid-1;
        mid = (min + max)/2;
    }
    return mid;
}

int NoteStream::getSetterId(double t) {
    if(setter.size() == 0) return 0;
    int min = 0;
    int max = setter.size()-1;
    int mid = (min + max)/2;
    while(min<=max && !almostEQ(setter[mid].first, t)) {
        if(setter[mid].first < t)
            min = mid+1;
        else
            max = mid-1;
        mid = (min + max)/2;
    }
    return mid;
}


void NoteStream::Add(std::pair<double, Note> p) {
    if(playable.empty()) { 
        playable.emplace_back(p);
        return;
    }
    int id = getId(p.first);
    if(playable[id].first < p.first) id++;
    if(id == playable.size())
        playable.emplace_back(p);
    else
        playable.insert(std::next(playable.begin(), id), p);
}

void NoteStream::Add(std::pair<double, SetterNote> p) {
    if(setter.empty()) { 
        setter.emplace_back(p);
        return;
    }
    int id = getSetterId(p.first);
    if(setter[id].first < p.first) id++;
    if(id == setter.size())
        setter.emplace_back(p);
    else
        setter.insert(std::next(setter.begin(), id), p);
}

void NoteStream::Add(std::pair<double, Loop> p) {
    if(p.second.getRepAmount() < 0) {
        //TODO: Infinite loops
        return;
    }
    double timestamp = p.first;
    Loop l = p.second;
    double len = l.getRepAmount() * l.getLen();
    int reps = 0;
    for(int i = 0; l.playable[i].first + reps*l.getLen() < len;) { 
        Add(std::make_pair(timestamp + reps*l.getLen() + l.playable[i].first, l.playable[i].second));
        i++;
        if(i >= l.playable.size()) {
            i = 0;
            reps++;
        }
    }
    for(reps = 0; reps < l.getRepAmount(); reps++) {
        for(int i = 0; i < l.setter.size(); i++) {
            Add(std::make_pair(timestamp + reps*l.getLen() + l.setter[i].first, l.setter[i].second));
        }
    }
    
}


std::vector<Note> NoteStream::GetStartingPlayableNotes(double t) {
    if(playable.size() == 0) return {};
    std::vector<Note> ret;
    auto i = playable.begin();
    for( ; i != playable.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    playable.erase(playable.begin(), i);
    return ret;
}

std::vector<SetterNote> NoteStream::GetStartingSetterNotes(double t) {
    if(setter.empty()) return {};
    std::vector<SetterNote> ret;
    auto i = setter.begin();
    for( ; i != setter.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    setter.erase(setter.begin(), i);
    return ret;
}

std::istream& operator>>(std::istream& stream, NoteStream& ns) {
    bool poly = ns.getPolynote();
    double sumlen = 0;
    while(true) {

        if((stream >> std::ws).peek() != '<') break;
        stream.get();

        double ts = sumlen;
        double len = 0;
        if(poly) {
            stream >> ts;
            ts *= 60/ns.getBpm();
        }
        if(isNote((stream >> std::ws).peek())) {
            Note n = Note(stream, ns.getBpm());
            ns.Add(std::make_pair(ts, n));
            len = n.getLen();
        }
        else {
            std::string buf = "";
            char c;
            bool parsed = false;
            stream >> std::ws;
            for(int i = 0; i < 4 && stream.get(c); i++) {
                buf += tolower(c);
                if(buf == "set") {
                    SetterNote n = SetterNote(stream);
                    ns.Add(std::make_pair(ts, n));
                    parsed = true;
                    break;
                }
                if(buf == "loop") {
                    Loop l = Loop(stream, ns.getBpm(), ns.getPolynote());
                    ns.Add(std::make_pair(ts, l));
                    parsed = true;
                    break;
                }
            }
            if(!parsed)
                throw parse_error(stream, "Could not parse special note");

        }
        if((stream >> std::ws).peek() != '>')
            throw parse_error(stream, "Missing closing '>' for note");
        stream.get();

        if(!poly)
            sumlen += len;
        if((stream >> std::ws).peek() != '<') break;
    }
    return stream;
}
