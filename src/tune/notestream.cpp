#include "notestream.hpp"
#include "../exceptions/parse_error.hpp"
#include "loop.hpp"
#include "../player/random_note.hpp"
#include "../player/setnote.hpp"
#include "../player/playablenote.hpp"

NoteStream::NoteStream(Note* n) : notes() {
    notes.emplace_back(std::make_pair(0, n));
}

NoteStream::NoteStream(double t, Note* n) : notes() {
    notes.emplace_back(std::make_pair(t, n));
}

NoteStream::NoteStream(std::pair<double, Note*> p) : notes(){
    notes.emplace_back(p);
}

NoteStream::NoteStream(const NoteStream& p) 
    : notes(), bpm(p.bpm), polynote(p.polynote), srate(p.srate) {
    for(size_t i = 0; i < p.notes.size(); i++) {
        notes.emplace_back(std::make_pair(p.notes[i].first, p.notes[i].second->copy()));
    }
}

std::vector<Note*> NoteStream::GetStartingNotes(double t) {
    if(notes.empty()) return {};
    std::vector<Note*> ret;
    auto i = notes.begin();
    for( ; i != notes.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    notes.erase(notes.begin(), i); 
    return ret;
}

NoteStream::NoteStream(std::istream& str, const std::vector<AudioSource*> srcs, double bpm, bool polynote, int srate)
    : bpm(bpm), polynote(polynote), srate(srate) {
    double sumlen = 0;
    str >> skipws;
    while(str.peek() == '<') {
        str.get();
        str >> skipws;
        double ts = sumlen;
        double len = 0;
        if(polynote) {
            str >> ts;
            ts *= 60/bpm;
        }
        if(isNote((str >> skipws).peek())) {
            PlayableNote n = PlayableNote(str, bpm);
            Add(std::make_pair(ts, n.copy()));
            len = n.GetLen();
        }
        else {
            std::string buf = "";
            char c;
            bool parsed = false;
            str >> skipws;
            for(int i = 0; i < 6 && str.get(c); i++) {
                buf += tolower(c);
                if(buf == "set") {
                    SetterNote n = SetterNote(str, srate);
                    int id = n.getId();
                    if(n.getGen() != nullptr) continue;
                    if((unsigned)id > srcs.size())
                        throw std::runtime_error(
                                "Index out of range at setter note");
                    if(id != -1)
                        n.setGen(srcs[id]);

                    Add(std::make_pair(ts, n.copy()));
                    parsed = true;
                    break;
                }
                if(buf == "loop") {
                    Loop l = Loop(str, srcs, bpm, polynote, srate);
                    Add(std::make_pair(ts, l.copy()));
                    len = l.getLen();
                    parsed = true;
                    break;
                }
                if(buf == "random") {
                    RandomNote r = RandomNote(str, srcs, bpm, polynote, srate);
                    Add(std::make_pair(ts, r.copy()));
                    len = r.GetLen();
                    parsed = true;
                    break;
                }
            }
            if(!parsed)
                throw parse_error(str, "Could not parse special note");

        }
        str >> expect('>') >> skipws;
        if(!polynote)
            sumlen += len;
    }
}
