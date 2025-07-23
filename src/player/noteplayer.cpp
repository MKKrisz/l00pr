#include "noteplayer.hpp"
#include "note.hpp"

void NotePlayer::addNote(std::unique_ptr<Note> note) {
    note->AddToPlayer(*this);
    m_src->addNote(std::move(note));
}

void NotePlayer::setSrc(Source* src) {
    if(src == nullptr)
        m_src = def_src;
    else
        m_src = src;
}

NotePlayer::NotePlayer(Source* src) : m_src(src), def_src(src){}

NotePlayer::NotePlayer(const NotePlayer& player) 
    : m_src(player.m_src), def_src(player.def_src) {}

float NotePlayer::getSample(int srate) {
    return m_src->getSample(srate);
}


NotePlayer& NotePlayer::operator=(const NotePlayer& player) {
    if(this == &player) {
        return *this;
    }
    m_src = player.m_src;
    def_src = player.def_src;
    return *this;
}
