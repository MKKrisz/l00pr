#ifndef L00PR_LANE
#define L00PR_LANE

#include "notestream.hpp"
#include "../player/noteplayer.hpp"


/// <summary> Rudimentary structure storing only a NotePlayer and a NoteStream </summary>
struct Lane {
    NotePlayer player;
    NoteStream stream;
    Lane(const NotePlayer& p, const NoteStream& s) : player(p), stream(s) {}
};

#endif
