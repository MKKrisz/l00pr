#ifndef L00PR_INTERPOLATED_H
#define L00PR_INTERPOLATED_H


#include <ranges>
#include <utility>
#include <vector>
#include <cmath>
#include <functional>
#include <iostream>
#include <optional>

#include "concepts.hpp"
#include "exceptions/parse_error.hpp"
#include "interpolator.hpp"
#include "util.hpp"
#include "writeable.hpp"

#ifdef _WIN32
#define M_PI 3.141592653589793238
#endif

template<Arithmetic T>
class Interpolated;

template<typename T>
std::istream& operator>>(std::istream& stream, Interpolated<T>& p);

template<typename T>
struct is_interpolated : public std::false_type {};

template<typename T>
struct is_interpolated<Interpolated<T>> : public std::true_type {};


///<summary>
/// Data structures for storing and evaluating a keyframe-based interpolation
///</summary>
template <Arithmetic T>
class Interpolated : public Writeable{
protected:
    /// <summary> Internal data structure. </summary>
    std::vector<std::pair<double, T>> data;

    double lowtresh = 0;
    double hightresh = 0;
    T* lowbuf = nullptr;
    T* highbuf = nullptr;

    /// <summary> Interpolator function. </summary>
    std::function<T(T, T, double)> itp;

    /// <summary> Default interpolator for this T </summary>
    static std::function<T(T, T, double)> def_itp;

public:
    /// <summary> The T template parameter. </summary>
    typedef T value_type;

    /// <summary> Default constructor. </summary>
    Interpolated() : data(), itp(def_itp) {}

    /// <summary> Copy constructor. </summary>
    Interpolated(const Interpolated& ip) : data(ip.data), itp(ip.itp) {}

    /// <summary> Constructor for single T values </summary>
    Interpolated(const T& data) : data(), itp(def_itp){
        this->data.emplace_back(std::make_pair(0.0, data));
    }

    /// <summary> Constructor for a datapoint </summary>
    Interpolated(double t, const T& data) : data(), itp(def_itp) {
        this->data.emplace_back(std::make_pair(t, data));
    }

    /// <summary> Constructor for a datapoint in internal format </summary>
    Interpolated(std::pair<double, T>& data) : data(), itp(def_itp) {
        this->data.emplace_back(data);
    }

    /// <summary>
    /// Constructor for all types U with internal template parameter `std::pair<double, T>` that fit the std::range concept.
    /// </summary>
    template <std::ranges::range U> 
        requires std::same_as<std::ranges::range_value_t<U>, std::pair<double, T>>
    Interpolated(const U& data) : data(), itp(linearInterpolator<T>) {
        for(auto it = data.begin(); it != data.end(); std::advance(it, 1)) {
            this->data.emplace_back(*it);
        }
        sort();
    }
    
    /// <summary> Just a humble unused (deprecated?) function... </summary>
    static T basic_interpreter(const char* s, int* n = nullptr) {return T(s);}

    /// <summary> Returns the size of the internal data structure. </summary>
    size_t Size() const {return data.size();}

    /// <summary> Returns the interpolated value at 't' </summary>
    T Get(double t) {
        if(data.size() == 1) return data[0].second;

        if(lowbuf != nullptr && highbuf != nullptr && t > lowtresh && t < hightresh) {
            return itp(*lowbuf, *highbuf, (t - lowtresh) / (hightresh - lowtresh));
        }
        if(lowbuf == nullptr && highbuf != nullptr && t < hightresh) {
            return *highbuf;
        }
        if(lowbuf != nullptr && highbuf == nullptr && t > lowtresh) {
            return *lowbuf;
        }
        
        int id = getId(t);
        if(almostEQ(data[id].first, t)) return data[id].second;

        size_t aid, bid;

        if(data[id].first < t) { aid = id; bid = id + 1;}
        else { aid = id - 1; bid = id;}
        
        if(bid == 0) {
            lowbuf = nullptr;
            hightresh = data[bid].first;
            highbuf = &(data[bid].second);
            return data[bid].second;
        }
        if(bid == data.size()) {
            highbuf = nullptr;
            lowtresh = data[aid].first;
            lowbuf = &(data[aid].second);
            return data[aid].second;
        }

        auto& a = data[aid];
        lowtresh = a.first;
        lowbuf = &(a.second);

        auto& b = data[bid];
        hightresh = b.first;
        highbuf = &(b.second);

        return itp(a.second, b.second, (t - a.first) / (b.first - a.first));
    }

    /// <summary> Modifies datapoint at 't' if exsists, adds a new datapoint otherwise </summary>
    void Set(double t, const T& data) {
        Set(std::make_pair(t, data));
    }

    /// <see cref="Set(double t, const T& data)"/>
    void Set(std::pair<double, T> p) {
        ordered_add(data, p);
    };

    /// <summary> Removes all datapoints </summary>
    void Clear() {
        data.clear();
    }

    /// <summary> Sets this object's interpolator to 'func' </summary>
    virtual void SetInterpolator(std::function<T(T, T, float)> func) {
        itp = func;
    }

    /// <summary> 
    /// Sets this type's interpolator to 'func'. 
    /// Effect only applies to objects constructed in the future. 
    /// </summary>
    static void SetDefaultInterpolator(std::function<T(T, T, float)> func) {
        def_itp = func;
    }

    /// <summary> Copy assignment operator </summary>
    Interpolated& operator=(const Interpolated& i) {
        if(this == &i) return *this;
        this->data = i.data;
        this->itp = i.itp;
        return *this;
    }

    /// <summary> Operator to use this object as a functor. Calls Get() internally. </summary>
    T operator()(double t) {return Get(t);}

    /// <summary> Returns a reference to the datapoint at index 'i' </summar/summaryy>
    std::pair<double,T>& operator[](size_t i) {return data[i];}
    const std::pair<double,T>& operator[](size_t i) const {return data[i];}

    friend std::istream& operator>> <> (std::istream& stream, Interpolated<T>& p);

protected:
    /// <summary> Sorts the internal data. May or may not actually work. Propably not even needed </summary>
    void sort(int low = 0, int high = -1) {
        if(low < 0) return;
        if(high == -1) high = data.size()-1;
        if(low >= high) return;

        int pivot = partition(low, high);
        sort(low, pivot-1);
        sort(pivot+1, high);
    }
    /// <summary> partition function for the above sort function </summary>
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
    
    /// <summary>
    /// Returns the index of the keyframe with timestamp "t"
    /// In case of failure the frame timestamp of data[id] will differ from t.
    /// </summary>
    size_t getId(double t) const { return bSearch(data, t); }

public:
    /// <summary> Returns the datapoint at 't' if it exists or an empty std::optional if it doesn't </summary>
    std::optional<std::pair<double, T>> find(double t) const {
        std::pair<float, T> ret = data[getId(t)];
        if(almostEQ(ret.first, t)) return ret;
        return std::optional<std::pair<float, T>>();
    }

    void Write(std::ostream& str) const {
        if(data.size() == 1) {
            str << data[0].second;
            return;
        }

        bool first = true;
        for(const auto& ts : data) {
            if(!first) str << " - ";
            str << ts.first << ": " << ts.second;
            first = false;
        }
    }
};

/// <summary> Parser for Interpolated values. Syntax can be: <value>; <time1>: <data1> - <time2>:<data2> - ... - <time_n>:<data_n> </summary>
/// <remarks> Does not clear 'p' </remarks>
template<typename T>
std::istream& operator>>(std::istream& stream, Interpolated<T>& p){
    stream >> skipws;
    size_t start = stream.tellg();
    int i = 0;
    while(true) {
        double t;
        T val;
        //if(!isalpha((stream >> skipws).peek())) break;
        stream >> skipws >> t;
        if((stream >> skipws).peek() != ':') {
            if(i == 0)  {
                stream.clear();
                stream.seekg(start) >> val;
                if(!stream.good()) {
                    throw parse_error(stream, "Invalid value");
                }
                p.data.emplace_back(std::make_pair(0, val));
                break;
            }
            else 
                throw parse_error(stream, "Couldn't interpret interpolated value: No ':' after timestamp value.\n Syntax for interpolated values can be: <value>; <timestamp>:<value> [ - <timestamp2>:<value2> - ... - <timestamp_n>:<value_n>]");
        }
        stream.get();
        stream >> skipws >> val;
        p.data.emplace_back(std::make_pair(t, val));

        if((stream >> skipws).peek() != '-') break;
        i++;
        stream.get();
        stream >> skipws;
        //if((stream >> skipws).peek() == '-') break;
    }
    p.sort();
    return stream;
}

/// <summary> base default interpolator for all values </summary>
template<Arithmetic T> 
std::function<T(T, T, double)> Interpolated<T>::def_itp = linearInterpolator<T>;

#endif
