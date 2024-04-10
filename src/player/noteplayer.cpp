#include "noteplayer.hpp"

void NotePlayer::addNote(Note& note) {
    m_notes.emplace_back(note);
    m_gen->addPhase();
}
void NotePlayer::addNote(SetterNote& note) {
    std::vector phases = m_gen->getPhases();
    delete m_gen;
    if(note.getGen() == nullptr) {
        m_gen = def_gen->copy();
        m_gen->setPhases(phases);
        return;
    }
    m_gen = note.getGen()->copy();
    m_gen->setPhases(phases);
}

NotePlayer::NotePlayer(Generator* gen) : m_notes(), m_gen(gen->copy()), def_gen(gen->copy()){}

NotePlayer::NotePlayer(const NotePlayer& player) 
    : m_notes(player.m_notes), m_gen(player.m_gen->copy()), def_gen(player.def_gen->copy()){}

NotePlayer::~NotePlayer() {
    delete m_gen;
    delete def_gen;
}


float NotePlayer::getSample(float srate) {
    double sum = 0;
    for(auto it = m_notes.begin(); it != m_notes.end(); std::advance(it, 1)) {
        if(it->isComplete()) {
            m_gen->removePhase(std::distance(m_notes.begin(), it));
            it = m_notes.erase(it);
            std::advance(it, -1);
            continue;
        }
        double t = it->getTime();
        Generator& gen = *m_gen;

        double sample = gen(std::distance(m_notes.begin(), it), it->getDelta(srate), t);
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
