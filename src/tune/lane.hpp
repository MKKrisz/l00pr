#ifndef L00PR_LANE
#define L00PR_LANE

#include "notestream.hpp"
#include "../player/noteplayer.hpp"

struct Lane {
    NotePlayer player;
    NoteStream stream;
    Lane(const NotePlayer& p, const NoteStream& s) : player(p), stream(s) {}
};

#endif
