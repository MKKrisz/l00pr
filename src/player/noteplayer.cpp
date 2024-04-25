#include "noteplayer.hpp"

void NotePlayer::addNote(Note& note) {
    m_notes.emplace_back(note);
    m_src->addPhase();
}
void NotePlayer::addNote(SetterNote& note) {
    std::vector phases = m_src->getPhases();
    delete m_src;
    if(note.getGen() == nullptr) {
        m_src = def_src->copy();
        m_src->setPhases(phases);
        return;
    }
    m_src = note.getGen()->copy();
    m_src->setPhases(phases);
}

NotePlayer::NotePlayer(AudioSource* src) : m_notes(), m_src(src->copy()), def_src(src->copy()){}

NotePlayer::NotePlayer(const NotePlayer& player) 
    : m_notes(player.m_notes), m_src(player.m_src->copy()), def_src(player.def_src->copy()){}

NotePlayer::~NotePlayer() {
    delete m_src;
    delete def_src;
}


float NotePlayer::getSample(double srate) {
    double sum = 0;
    for(auto it = m_notes.begin(); it != m_notes.end(); std::advance(it, 1)) {
        if(it->isComplete()) {
            m_src->removePhase(std::distance(m_notes.begin(), it));
            it = m_notes.erase(it);
            std::advance(it, -1);
            continue;
        }
        double t = it->getTime();
        AudioSource& src = *m_src;

        double sample = src(std::distance(m_notes.begin(), it), it->getDelta(srate), t, srate);
        sum += (*it).getAmplitude() * sample;
        
    }
    return sum;
}


NotePlayer& NotePlayer::operator=(const NotePlayer& player) {
    if(this == &player) {
        return *this;
    }
    m_notes = player.m_notes;
    m_src = player.m_src;
    return *this;
}
