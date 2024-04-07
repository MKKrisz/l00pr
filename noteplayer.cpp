#include "noteplayer.hpp"

void NotePlayer::addNote(Note& note) {
    m_notes.emplace_back(note);
    m_gen->addPhase();
}

NotePlayer::NotePlayer(Generator* gen) : m_notes() , m_gen(gen->copy()) {}

NotePlayer::NotePlayer(const NotePlayer& player) 
    : m_notes(player.m_notes), m_gen(player.m_gen->copy()){}

NotePlayer::~NotePlayer() {
    delete m_gen;
}


float NotePlayer::getSample(float srate) {
    float sum = 0;
    int i;
    for(auto it = m_notes.begin(); it != m_notes.end(); std::advance(it, 1)) {
        if(it->isComplete()) {
            m_gen->removePhase(std::distance(m_notes.begin(), it));
            it = m_notes.erase(it);
            std::advance(it, -1);
            continue;
        }
        float t = it->getTime();
        Generator& gen = *m_gen;

        float sample = gen(std::distance(m_notes.begin(), it), it->getDelta(srate), t);
        sum += (*it).getAmplitude() * sample;
        
    }
    return sum;
}


NotePlayer& NotePlayer::operator=(const NotePlayer& player) {
    if(this == &player) {
        return *this;
    }
    m_notes = player.m_notes;
    m_gen = player.m_gen;
    return *this;
}
