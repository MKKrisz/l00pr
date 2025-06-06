#include "filter.hpp"
#include "../exceptions/parse_error.hpp"
#include "builtin.hpp"

std::string Filter_Metadata::ToString() const {
    std::string ret = keyword;
    size_t maxKwdLen = Filter::GetLongestKeywordMeta().keyword.size();
    for(size_t i = 0; i < (maxKwdLen - keyword.size())/8 + 1; i++) {
        ret += '\t';
    }
    ret += (maxKwdLen == keyword.size()?"":"\tSyntax: ") + syntax + "\t" + desc;
    return ret;
}

void Filter::Init() {
    AddMetadata(Filter_Metadata("delay", DelayFilter::Create, "pass(<freq_resp_curve> [samples]) [{src}]", "Arbitrary pass filter (freq_resp_curve: Interpolated<double>)"));
    AddMetadata(Filter_Metadata("feedback", Feedback::Create, "feedback(<depth> [fb_filter]) [{src}]", "Sends back signals in the chain by `depth`(uint) after being processed by `fb_filter`(Filterchain)"));
    AddMetadata(Filter_Metadata("pass", PassFilter::Create, "pass(<freq_resp_curve> [samples]) [{src}]", "Arbitrary pass filter (freq_resp_curve: Interpolated<double>)"));
    AddMetadata(Filter_Metadata("gain", GainFilter::Create, "gain(<amount>) [{src}]", "Multiplies the samples coming from src by `amount`(double)"));
    AddMetadata(Filter_Metadata("quantize", QuantizeFilter::Create, "quantize(<bits>) [{src}]", "Quantizes the signal coming from `src` to be `bits`(uint)"));
    AddMetadata(Filter_Metadata("split", Splitter::Create, "split([f_1] [f_2] ... [f_n]) [{src}]", "Splits the signal into different paths, then combines them. (f_k: Filterchain)"));
    AddMetadata(Filter_Metadata("dummy", DummyFilter::Create, "", "Does nothing"));
}

void Filter::operator()(size_t noteId, double delta, double t, double srate, double extmul) {
    (*src)(noteId, delta, t, srate, extmul);
}

double Filter::calc() {
    return filter(src == nullptr? getAccumulator() + feedback : src->calc(), 0, 0, 0) + (src == nullptr ? 0 : feedback);
}

std::string Filter::getFormattedMetadata() {
    std::string ret = "";
    for(const Filter_Metadata& f : meta) {
        ret += f.keyword + "\t" + (f.keyword.size() < 8? "\t" : "") + f.syntax + "\t" + f.desc + "\n";
    }
    return ret;
}
void Filter::Write(std::ostream& str) const {
    str << GetNameAndParams();
    if(src != nullptr) {
        str << " {";
        src->Write(str);
        str << "}";
    }
    WriteLengthBounds(str);
} 
