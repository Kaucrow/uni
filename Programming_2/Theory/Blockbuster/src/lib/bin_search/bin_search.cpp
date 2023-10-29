#include "bin_search.h"
#include <iostream>
#include <functional>
using std::function;

template <typename T>
bool CompareFunc(T searchArrDat, T toCompareDat, bool equality){
    return equality ? searchArrDat == toCompareDat : searchArrDat < toCompareDat;
}

// polymorphic function wrapper used by the Merge() function for sorting a movie list
// according to duration, title, director, release year, release month, or release day.
function<bool(const Movie searchArr[], int pos, Movie toCompare, bool equality)> BinCompare[6] = {
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].duration, toCompare.duration, equality); },
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].title, toCompare.title, equality); },
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].director, toCompare.director, equality); },
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.year, toCompare.release.year, equality); },
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.month, toCompare.release.month, equality); },
    [](const Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.day, toCompare.release.day, equality); }
};

void StoreMatches(const Movie searchArr[], Movie storeArr[], int someMatchPos, const Movie &toCompare, const int type){
    int storeIndex = 0;
    for(int offset = 0; BinCompare[type](searchArr, someMatchPos + offset, toCompare, 1); offset--){
        storeArr[storeIndex] = searchArr[someMatchPos + offset];
        storeIndex++;
        std::wcout << "FOUNDL\n";   // debug
    }
    std::wcout << "REACHED\n";      // debug
    std::wcout << searchArr[someMatchPos - storeIndex].duration << '\n';        // debug
    for(int offset = 1; BinCompare[type](searchArr, someMatchPos + offset, toCompare, 1); offset++){
        storeArr[storeIndex] = searchArr[someMatchPos + offset];
        storeIndex++;
        std::wcout << "FOUNDR\n";   // debug
    }
    //std::wcout << searchArr[someMatchPos + storeIndex].duration << '\n';      // debug
}

int BinSearch(const Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type, bool storeMatches){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // check if x is present at mid
        if(BinCompare[type](searchArr, m, toCompare, 1)){
            if(storeMatches) StoreMatches(searchArr, storeArr, m, toCompare, type);
            return m;
        }
 
        // if x is greater, ignore left half
        if(BinCompare[type](searchArr, m, toCompare, 0))
            l = m + 1;
 
        // if x is smaller, ignore right half
        else
            r = m - 1;
    }
 
    // if we reach here, the element was not present
    if(storeMatches) return -1;
    else return m;
}
