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

template<typename T, typename U>
void StoreMatches(const T searchArr[], int storeArr[], int someMatchPos, const U search){
    int storeIndex = 0;
    for(int offset = 0; (searchArr[someMatchPos + offset].data == search); offset--){
        storeArr[storeIndex] = searchArr[someMatchPos + offset].ID;
        storeIndex++;
        std::wcout << "FOUNDL\n";   // debug
    }
    std::wcout << "REACHED\n";      // debug
    std::wcout << searchArr[someMatchPos - storeIndex].data << '\n';        // debug
    for(int offset = 1; (searchArr[someMatchPos + offset].data == search); offset++){
        storeArr[storeIndex] = searchArr[someMatchPos + offset].ID;
        storeIndex++;
        std::wcout << "FOUNDR\n";   // debug
    }
}

template<typename T, typename U>
int BinSearch(const T searchArr[], int storeArr[], int l, int r, const U search, bool storeMatches){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // Check if x is present at mid.
        if(searchArr[m].data == search){
            if(storeMatches) StoreMatches(searchArr, storeArr, m, search);
            return m;
        }
 
        // If x is greater, ignore left half.
        if(searchArr[m].data < search)
            l = m + 1;
 
        // If x is smaller, ignore right half.
        else
            r = m - 1;
    }
 
    // If we reach here, the element was not present.
    if(storeMatches) return -1;
    else return m;
}

template int BinSearch<IntFrag, int>(const IntFrag searchArr[], int storeArr[], int l, int r, const int search, bool storeMatches);