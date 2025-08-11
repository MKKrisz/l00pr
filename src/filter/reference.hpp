#pragma once
#include "filter.hpp"

class Reference : public Filter {
public:
    Reference() = delete;
    Reference(Source* s) : Filter(s) {}
    Reference(const Reference& f) : Filter(f) {}
    Reference(std::istream& str, const int, const MakeFlags& = MakeFlags::all) : Filter() {
        brace(str, '(', ')', 1, [&](std::istream& str) {
            str >> skipws;
            if(isdigit(str.peek())) {
                int id; str >> id;
                src = std::make_unique<SourceRef>(id);
                return; 
            }
            std::string buf = "";
            std::getline(str, buf, ')');
            str.putback(')');
            src = std::make_unique<SourceRef>(trim(buf));
        });
    }
    void addNote(std::unique_ptr<Note>) {/*nothing, we should not manipulate the reference like that*/}

    inline double filter(double, double, double, int) { return 0; }
    double getSample(int) {
        return src->getLastSample();
    }
    double getSingleSample(double, double, int) {
        return src->getLastSample();
    }
    std::unique_ptr<Source> copy() {return std::make_unique<Reference>(*this); }
    inline Reference& operator=(const Reference&) = default;
    std::string ToString() const {return Filter::ToString() + "Dummy";}
    std::string GetNameAndParams() const {
        SourceRef* ref = dynamic_cast<SourceRef*>(src.get());
        std::string param = ref->labeled() ? ref->getWanted().first : std::to_string(ref->getWanted().second);
        return "reference(" + param + ")";
    }
    static std::unique_ptr<Reference> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<Reference>(str, srate, flags);
    }
};
