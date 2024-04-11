#ifndef L00PR_INTERPOLATED_H
#define L00PR_INTERPOLATED_H


#include <ranges>
#include <utility>
#include <vector>
#include <cmath>
#include <functional>
#include <istream>

#include "concepts.hpp"
#include "exceptions/parse_error.hpp"
#include "interpolator.hpp"
#include "util.hpp"


template<Arithmetic T>
class Interpolated;

template<typename T>
std::istream& operator>>(std::istream& stream, Interpolated<T>& p);


///<summary>
/// Data structures for storing and evaluating a keyframe-based interpolation
///</summary>
template <Arithmetic T>
class Interpolated {
protected:

    ///<summary>
    /// Internal data structure = map ;)
    ///</summary>
    std::vector<std::pair<float, T>> data;
    std::function<T(T, T, float)> itp;

    static std::function<T(T, T, float)> defitp;

public:
    
    ///<summary> Default constructor </Summary>
    Interpolated() : data(), itp(defitp) {}
    Interpolated(const Interpolated& ip) : data(ip.data), itp(ip.itp){}

    Interpolated(const T& data) : data(), itp(defitp){
        this->data.emplace_back(std::make_pair(0.0f, data));
    }

    Interpolated(float t, const T& data) : data(), itp(defitp) {
        this->data.emplace_back(std::make_pair(t, data));
    }

    Interpolated(std::pair<float, T>& data) : data(), itp(defitp) {
        this->data.emplace_back(data);
    }

    template <std::ranges::range U> 
        requires std::same_as<std::ranges::range_value_t<U>, std::pair<float, T>>
    Interpolated(U& data) : data(), itp(linearInterpolator<T>) {
        for(auto it = data.begin(); it != data.end(); std::advance(it, 1)) {
            this->data.emplace_back(*it);
        }
        sort();
    }
    
    static T basic_interpreter(const char* s, int* n = nullptr) {return T(s);}


    Interpolated(const char* s, 
                 int* offset = nullptr,
                 std::function<T(const char*, int*)> interpreter = basic_interpreter) 
        : data(), itp(linearInterpolator<T>) {

        int i = 0;
        int iterc = 0;
        while(true) {
            float t = 0;
            T val = interpreter(&s[i], nullptr);
            while(isspace(s[i])) i++;
            for( ; s[i] != '\0'; i++) {
                if(!isdigit(s[i])) break;
                t*=10;
                t+=s[i]-'0';
            } 
            if(s[i] == '.') {
                i++;
                for(int j = 0; s[i + j] != '\0'; j++) {
                    if(!isdigit(s[i+j])) {i += j; break;}
                    t += (s[i+j]-'0')/pow(10.0f, j+1);
                }
            }
            if(s[i++] != ':') {
                if(iterc == 0) {
                    data.emplace_back(std::make_pair(0, val));
                }
                return;
            }
            while(isspace(s[i])) i++;
            int offset;
            val = interpreter(&s[i], &offset);
            i += offset;
            data.emplace_back(std::make_pair(t, val));
            if(s[i] != '-') break;
            i++;
            while(isspace(s[i])) i++;
            iterc++;
        }
        sort();
        if(offset != nullptr) *offset = i;
    }

    size_t Size() {return data.size();}

    T Get(float t) {
        int id = getId(t);
        if(almostEQ(data[id].first, t)) return data[id].second;

        int aid, bid;
        if(data[id].first < t) { aid = id; bid = id + 1;}
        else { aid = id - 1; bid = id;}
        
        if(bid == 0) 
            return data[bid].second;
        if(bid == data.size()) {
            return data[aid].second;
        }
        auto a = data[aid];
        auto b = data[bid];

        return itp(a.second, b.second, (t - a.first) / (b.first - a.first));
    }
    
    void Set(float t, T& data) {
        Set(std::make_pair(t, data));
    }
    void Set(std::pair<float, T> p) {
        int id = getId(p.first);
        if(almostEQ(data[id].first, p.first)) {
            data[id].second = p.second;
            return;
        }
        data.insert(id, p);
    };

    void Clear() {
        data.clear();
    }

    virtual void SetInterpolator(std::function<T(T, T, float)> func) {
        itp = func;
    }
    static void SetDefaultInterpolator(std::function<T(T, T, float)> func) {
        defitp = func;
    }

    Interpolated& operator=(const Interpolated& i) {
        if(this == &i) return *this;
        this->data = i.data;
        this->itp = i.itp;
        return *this;
    }
    T operator()(float t) {return Get(t);}

    std::pair<float,T>& operator[](int i) {return data[i];}

    friend std::istream& operator>> <> (std::istream& stream, Interpolated<T>& p);

protected:
    void sort(int low = 0, int high = -1) {
        if(low < 0) return;
        if(high == -1) high = data.size()-1;
        if(low >= high) return;

        int pivot = partition(low, high);
        sort(low, pivot-1);
        sort(pivot+1, high);
    }
    int partition(int l, int h) {
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
    int getId(float t) {
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
        if(min<=max) return mid;
        //if(data[mid].first < t) return mid+1;
        return mid;
    }

    std::optional<std::pair<float, T>> find(float t) {
        std::pair<float, T> ret = data[getId(t)];
        if(almostEQ(ret.first, t)) return ret;
        return std::optional<std::pair<float, T>>();
    }
};

template<typename T>
std::istream& operator>>(std::istream& stream, Interpolated<T>& p){
    size_t start = stream.tellg();
    int i = 0;
    while(true) {
        float t;
        T val;
        if(!isalpha((stream >> skipws).peek())) break;
        stream >> t;
        if((stream >> skipws).peek() != ':') {
            if(i == 0)  {
                stream.seekg(start) >> val;
                p.data.emplace_back(std::make_pair(0, val));
            }
            else 
                throw parse_error(stream, "Couldn't interpret interpolated value: No ':' after timestamp value");
        }
        stream.get();
        stream >> val;
        p.data.emplace_back(std::make_pair(t, val));
        if((stream >> skipws).peek() != '-') break;
        i++;
        stream.get();
    }
    p.sort();
    return stream;
}

template<Arithmetic T> 
std::function<T(T, T, float)> Interpolated<T>::defitp = linearInterpolator<T>;

template<>
float Interpolated<float>::basic_interpreter(const char* s, int* n);

template<>
std::istream& operator>> (std::istream& stream, Interpolated<double>& p);

#endif
