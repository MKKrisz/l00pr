#include "notestream.hpp"
#include "util.hpp"
#include "exceptions/parse_error.hpp"

NoteStream::NoteStream(Note n) : data(){
    data.push_back(std::make_pair(0, n));
}

NoteStream::NoteStream(float t, Note n) : data(){
    data.push_back(std::make_pair(t, n));
}

NoteStream::NoteStream(std::pair<float, Note> p) : data(){
    data.push_back(p);
}

NoteStream::NoteStream(const NoteStream& p) : data(p.data){}


// Adatok: 
//      float timestamp
//      Keyframe freq
//      float length
//      optional Keyframe amplitude
// <ts freq len ampl>
NoteStream::NoteStream(const char* s, int* offset) : data() {
    int i = 0;
    while(true) {
        while(isspace(s[i])) i++;
        if(s[i] != '<') break;
        i++;
        float ts = atof(&s[i]);
        while(isdigit(s[i]) || i == '.') i++; 
        while(isspace(s[i])) i++;
        int offs;
        Interpolated<float> freq = Interpolated<float>(&s[i], &offs);
        i += offs;
        float len = atof(&s[i]);
        while(isdigit(s[i]) || i == '.') i++;
        Interpolated<float> ampl = 1;
        if(s[i] != '>') {
            ampl = Interpolated<float>(&s[i], &offs);
            i += offs;
        }
        i++;
        data.push_back(std::make_pair(ts, Note(len, freq, ampl)));
        if(s[i] != '<') break;
    }
    sort();
}


void NoteStream::sort(int low, int high) {
    if(low < 0) return;
    if(high == -1) high = data.size()-1;
    if(low >= high) return;

    int pivot = partition(low, high);
    sort(low, pivot-1);
    sort(pivot+1, high);
}
int NoteStream::partition(int l, int h) {
    float pivot = data[h].first;
    int i = l-1;
    for(int j = l; j < h; j++) {
        if(data[j].first <= pivot) {
            i++;
            auto temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
    i++;
    auto temp = data[i];
    data[i] = data[h];
    data[h] = temp;
    return i;
}

/// Returns the index of the keyframe with timestamp "t"
/// In case of failure the frame timestamp of data[id] will differ from t
int NoteStream::getId(float t) {
    if(data.size() == 0) return 0;
    int min = 0;
    int max = data.size()-1;
    int mid = (min + max)/2;
    while(min<=max && !almostEQ(data[mid].first, t)) {
        if(data[mid].first < t)
            min = mid+1;
        else
            max = mid-1;
        mid = (min + max)/2;
    }
    return mid;
}


void NoteStream::Add(std::pair<float, Note>& p) {
    if(data.size() == 0) { 
        data.emplace_back(p);
        return;
    }
    int id = getId(p.first);
    if(data[id].first < p.first) id++;
    if(id == data.size())
        data.emplace_back(p);
    else
        data.insert(std::next(data.begin(), id), p);
}

 std::pair<float, Note> NoteStream::PopFirst() {
    std::pair<float, Note> ret = *data.begin();
    data.erase(data.begin());
    return ret;
}


std::vector<Note> NoteStream::GetStartingNotes(float t) {
    if(data.size() == 0) return std::vector<Note>();
    std::vector<Note> ret;
    auto i = data.begin();
    for( ; i != data.end() && i->first <= t; i++) {
        ret.emplace_back(i->second);
    }
    data.erase(data.begin(), i);
    return ret;
}

std::istream& operator>>(std::istream& stream, NoteStream& ns) {
    bool poly = ns.getPolynote();
    float sumlen = 0;
    while(true) {

        if((stream >> std::ws).peek() != '<') break;
        stream.get();

        float ts;
        if(poly) {
            stream >> ts;
            ts *= 60/ns.getBpm();
        }
        Note n = Note(stream, ns.getBpm());
        if((stream >> std::ws).peek() != '>')
            throw parse_error(stream, "Missing closing '>' for note");
        stream.get();

        std::pair<float, Note> note = std::make_pair(poly?ts:sumlen, n);
        ns.Add(note);
        if(!poly)
            sumlen += n.getLen();
        if((stream >> std::ws).peek() != '<') break;
    }
    ns.sort();
    return stream;
}
