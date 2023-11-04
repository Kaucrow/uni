#include "bin_search.h"
#include <iostream>
#include <functional>
using std::function;

template <typename T>
bool CompareFunc(T searchArrDat, T toCompareDat, bool equality){
    return equality ? searchArrDat == toCompareDat : searchArrDat < toCompareDat;
}

/**
 * @brief   Polymorphic function wrapper used to compare the data fields of two movies,
 *          either by greatness or equality.
 * @param searchArr - Movie list array which contains the movie to be compared against.
 * @param pos - Index of the movie to be compared against.
 * @param toCompare - Movie to be compared from.
 * @param equality - If true, performs a comparison based on equality. If false,
 *                   performs a comparison based on greatness.
 * @return If equality is true, will return searchArr[pos].dataType == toCompare.dataType,
 *         if equality is false, will return searchArr[pos].dataType < toCompare.dataType.
*/
function<bool(const Movie searchArr[], int pos, const Movie &toCompare, bool equality)> BinCompare[6] = {
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].duration, toCompare.duration, equality); },
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].title, toCompare.title, equality); },
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].director, toCompare.director, equality); },
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.year, toCompare.release.year, equality); },
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.month, toCompare.release.month, equality); },
    [](const Movie searchArr[], int pos, const Movie &toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.day, toCompare.release.day, equality); }
};

void StoreMatches(const Movie searchArr[], Movie storeArr[], int someMatchPos, const Movie &search, const int type){
    int storeIndex = 0;
    for(int offset = 0; BinCompare[type](searchArr, someMatchPos + offset, search, 1); offset--){
        storeArr[storeIndex] = searchArr[someMatchPos + offset];
        storeIndex++;
        std::wcout << "FOUNDL\n";   // debug
    }
    std::wcout << "REACHED\n";      // debug
    std::wcout << searchArr[someMatchPos - storeIndex].duration << '\n';        // debug
    for(int offset = 1; BinCompare[type](searchArr, someMatchPos + offset, search, 1); offset++){
        storeArr[storeIndex] = searchArr[someMatchPos + offset];
        storeIndex++;
        std::wcout << "FOUNDR\n";   // debug
    }
}

int BinSearch(const Movie searchArr[], Movie storeArr[], int l, int r, const Movie &search, const int type, bool storeMatches){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // Check if x is present at mid.
        if(BinCompare[type](searchArr, m, search, 1)){
            if(storeMatches) StoreMatches(searchArr, storeArr, m, search, type);
            return m;
        }
 
        // If x is greater, ignore left half.
        if(BinCompare[type](searchArr, m, search, 0))
            l = m + 1;
 
        // If x is smaller, ignore right half.
        else
            r = m - 1;
    }
 
    // If we reach here, the element was not present.
    if(storeMatches) return -1;
    else return m;
}