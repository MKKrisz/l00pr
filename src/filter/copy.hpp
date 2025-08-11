#pragma once
#include "filter.hpp"

class CopyReference : public Filter {
    std::unique_ptr<SourceRef> ref;
public:
    CopyReference() = delete;
    CopyReference(Source* s) : Filter(s) {}
    CopyReference(const CopyReference& f) : Filter(f), ref(std::make_unique<SourceRef>(*f.ref)) {}
    CopyReference(std::istream& str, const int, const MakeFlags& = MakeFlags::all) : Filter() {
        brace(str, '(', ')', 1, [&](std::istream& str) {
            str >> skipws;
            if(isdigit(str.peek())) {
                int id; str >> id;
                ref = std::make_unique<SourceRef>(id);
                return; 
            }
            std::string buf = "";
            std::getline(str, buf, ')');
            str.putback(')');
            ref = std::make_unique<SourceRef>(trim(buf));
        });
    }
    std::vector<SourceRef*> getSourceRefs() override {
        return {ref.get()};     // we lie to the program :)
    }
    void addNote(std::unique_ptr<Note> note) override {
        if(src == nullptr) {
            if(!ref->resolved()) {throw std::runtime_error("Unresolved copy reference: " + (ref->labeled() ? ref->getWanted().first : std::to_string(ref->getWanted().second)));}
            src = ref->getSource()->copy();
        }
        src->addNote(std::move(note));
    }

    inline double filter(double sample, double, double, int) override { return sample; }
    std::unique_ptr<Source> copy() override {return std::make_unique<CopyReference>(*this); }
    std::string ToString() const override {return Filter::ToString() + "Dummy";}
    std::string GetNameAndParams() const override {
        SourceRef* ref = dynamic_cast<SourceRef*>(src.get());
        std::string param = ref->labeled() ? ref->getWanted().first : std::to_string(ref->getWanted().second);
        return "reference(" + param + ")";
    }
    static std::unique_ptr<CopyReference> Create(std::istream& str, const int srate, const MakeFlags& flags) {
        return std::make_unique<CopyReference>(str, srate, flags);
    }
};
