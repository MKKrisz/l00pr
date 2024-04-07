#ifndef L00PR_SAMPLESTREAM
#define L00PR_SAMPLESTREAM

#include <vector>
#include "concepts.hpp"

template <Arithmetic T>
class SampleStream {
    std::vector<T> stream;
    int limit;
public:
    void Put(T&& data) {
        stream.push_back(data);
    }
    void Put(T* data, int n) {
        for(int i = 0; i < n; i++)
            Put(data[i]);
    }

    T& Pop() {
        T& ret = *(stream.begin());
        stream.erase(stream.begin());
        return ret;
    }
    T& WriteToCArr(T* arr, int n) {
        //memcpy()
    }



};

#endif
