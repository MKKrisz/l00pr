#include "notestream.hpp"
#include "../exceptions/parse_error.hpp"
#include "loop.hpp"
#include "../player/random_note.hpp"
#include "../player/setnote.hpp"
#include "../player/playablenote.hpp"

NoteStream::NoteStream(Note* n) : notes() {
    notes.emplace_back(std::make_pair(0, n));
    calculateLen();
}

NoteStream::NoteStream(double t, Note* n) : notes() {
    notes.emplace_back(std::make_pair(t, n));
    calculateLen();
}

NoteStream::NoteStream(std::pair<double, Note*> p) : notes(){
    notes.emplace_back(p);
    calculateLen();
}

NoteStream::NoteStream(const NoteStream& p) 
    : notes(), bpm(p.bpm), polynote(p.polynote), srate(p.srate), lastNoteTs(p.lastNoteTs), len(p.len){
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

void NoteStream::calculateLen() {
    double max = 0;
    double maxTs = 0;
    for(size_t i = 0; i < notes.size(); i++) {
        if(maxTs < notes[i].first) 
            maxTs = notes[i].first;
        if(max < notes[i].first + notes[i].second->GetLen())
            max = notes[i].first + notes[i].second->GetLen();
    }
    len = max;
    lastNoteTs = maxTs;
}

double NoteStream::getLen() const {
    return len;
}

NoteStream::NoteStream(std::istream& str, const std::vector<AudioSource*> srcs, double bpm, bool polynote, int srate)
    : bpm(bpm), polynote(polynote), srate(srate) {
    double sumlen = 0;
    double prev_len = 0;
    str >> skipws;
    while(str.peek() == '<') {
        str.get();
        str >> skipws;
        double ts = sumlen;
        double len = 0;
        if(polynote) {
            std::string timestamp_word = "";
            str >> timestamp_word;
            if(timestamp_word == "a" || timestamp_word == "after") {
                
            } else if(timestamp_word == "w" || timestamp_word == "with") {
                ts -= prev_len;
            } else { 
                try {
                    ts = std::stod(timestamp_word);
                    ts *= 60/bpm;
                } catch(std::invalid_argument& err) {
                    throw parse_error(str, "Timestamp could not be parsed! Accepted keywords: a(fter), w(ith) or a number");
                }
            }
        }
        Note* note = Note::Make(str, srcs, bpm, polynote, srate);
        len = note->GetLen();
        Add(std::make_pair(ts, note));

        str >> expect('>') >> skipws;
        if(!polynote) {
            sumlen += len;
        } else {
            sumlen = ts + len;
        }
        prev_len = len;
    }
    calculateLen();
}
void NoteStream::Write(std::ostream& str) const {
    for(const auto& note : notes) {
        str << std::endl << "<" << note.first << " ";
        note.second->Write(str);
        str << '>';
    }
}
