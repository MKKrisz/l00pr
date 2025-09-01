#include "note_count.hpp"
#include "../player/note.hpp"

NoteCount::NoteCount() : Generator() {/*nothing*/}

NoteCount::NoteCount(const NoteCount& other) : Generator(other) {/*nothing*/}


double NoteCount::getSample(int srate) {
    for(size_t i = 0; i < playing_notes.size(); i++) {
        if(playing_notes[i]->IsComplete()) {
            playing_notes[i]->RemoveFromSource(this);
            playing_notes.erase(playing_notes.begin() + int(i));
            i--;
            continue;
        }
        playing_notes[i]->AddSample(this, srate);       // we still need notes to progress
    }
    m_accumulator = playing_notes.size();
    return getAccumulator();
}
std::unique_ptr<NoteCount> NoteCount::Create(std::istream&, const int, const MakeFlags&) {
    return std::make_unique<NoteCount>(); // no arguments means peace for the mind....
}
