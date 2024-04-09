#include "notestream.hpp"
#include "../exceptions/parse_error.hpp"
#include "loop.hpp"

NoteStream::NoteStream(Note n) : playable(), setter(), loops(){
    playable.push_back(std::make_pair(0, n));
}

NoteStream::NoteStream(double t, Note n) : playable(), setter(), loops(){
    playable.push_back(std::make_pair(t, n));
}

NoteStream::NoteStream(std::pair<double, Note> p) : playable(), setter(), loops(){
    playable.push_back(p);
}

NoteStream::NoteStream(const NoteStream& p) : 
    playable(p.playable), setter(p.setter), loops(p.loops){}

void NoteStream::Add(std::pair<double, Loop> p) {
    if(p.second.getRepAmount() < 0) {
        ordered_add(loops, std::make_pair(p.first, p.second.getBaseStream()));
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
    if(playable.empty() && loops.empty()) return {};
    std::vector<Note> ret;
    auto i = playable.begin();
    for( ; i != playable.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    playable.erase(playable.begin(), i);
    for(int j = 0; j < loops.size(); j++) {
        const double& ts = loops[j].first;
        NoteStream& l = loops[j].second;
        std::vector<Note> lsNotes = l.GetStartingPlayableNotes(t - ts);
        //hack
        for(int k = 0; k < lsNotes.size(); k++) {
            l.playable.emplace_back(t + l.getLen(), lsNotes[k]);
        }
        //end of //hack
        ret.insert(ret.end(), lsNotes.begin(), lsNotes.end());
    }
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
