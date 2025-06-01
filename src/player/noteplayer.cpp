#include "noteplayer.hpp"
#include "note.hpp"

void NotePlayer::addNote(Note* note) {
    note->AddToPlayer(*this);
    m_notes.emplace_back(note);
    m_src->addPhase();
}

void NotePlayer::setSrc(AudioSource* src) {
    std::vector<double> phases = m_src->getPhases();
    if(src == nullptr)
        m_src = std::move(def_src->copy());
    else
        m_src = std::move(src->copy());
    m_src->setPhases(phases);

    //hack to fix mismatch of phases
    for(size_t i = m_src->getPhases().size(); i < m_notes.size(); i++) {
        m_src->addPhase();
    }
}

NotePlayer::NotePlayer(AudioSource* src) : m_notes(), m_src(std::move(src->copy())), def_src(src){}

NotePlayer::NotePlayer(const NotePlayer& player) 
    : m_notes(player.m_notes), m_src(std::move(player.m_src->copy())), def_src(player.def_src){}

float NotePlayer::getSample(double srate) {
    for(size_t i = 0; i < m_notes.size(); i++) {
        if(m_notes[i]->IsComplete()) {
            m_src->removePhase(i);
            delete m_notes[i];
            m_notes.erase(m_notes.begin() + int(i));
            //i--;
            continue;
        }
        m_notes[i]->AddSample(*this, i, srate);
    }
    float res = m_src->calc();
    return res;
}


NotePlayer& NotePlayer::operator=(const NotePlayer& player) {
    if(this == &player) {
        return *this;
    }
    m_notes = player.m_notes;
    m_src = player.m_src->copy();
    def_src = player.def_src;
    return *this;
}
