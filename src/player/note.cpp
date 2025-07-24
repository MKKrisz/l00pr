#include "note.hpp"
#include "builtin.hpp"

uint32_t Note::note_id_ctr = 0;

std::string Note_Metadata::ToString() const {
    std::string ret = keyword;
    size_t maxKwdLen = Note::GetLongestKeywordMeta().keyword.size();
    for(size_t i = 0; i < (maxKwdLen - keyword.size())/8 + 1; i++) {
        ret += '\t';
    }
    ret += (maxKwdLen == keyword.size()?"":"\tSyntax: ") + syntax + "\t" + desc;
    return ret;
}

void Note::Init() {
    AddMetadata(Note_Metadata("note", PlayableNote::Create, "<frequency> <length> <amplitude>", "A playable note"));
    AddMetadata(Note_Metadata("set", SetterNote::Create, "<generator/filter chain/index>", "Changes the current generator"));
    AddMetadata(Note_Metadata("loop", Loop::Create, "(<repeat_count>) {<notes>}", "Loop"));
    AddMetadata(Note_Metadata("random", RandomNote::Create, "honestly I don't remember", "Random note"));
    default_meta = Note_Metadata("note", PlayableNote::Create, "<frequency> <length> <amplitude>", "A playable note");
}

std::unique_ptr<Note> Note::Make(std::istream& str, Tune* tune, double bpm, bool poly, int srate) {
    return Note::Parse(str, tune, bpm, poly, srate);
}


