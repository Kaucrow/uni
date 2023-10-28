#include "bin_search.h"
#include <iostream>
#include <functional>
using std::function;

template <typename T>
bool CompareFunc(T searchArr, T toCompare, bool equality){
    return equality ? searchArr == toCompare : searchArr < toCompare;
}

// polymorphic function wrapper used by the Merge() function for sorting a movie list
// according to duration, title, director, release year, release month, or release day.
function<bool(Movie searchArr[], int pos, Movie toCompare, bool equality)> BinCompare[6] = {
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].duration, toCompare.duration, equality); },
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].title, toCompare.title, equality); },
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].director, toCompare.director, equality); },
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.year, toCompare.release.year, equality); },
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.month, toCompare.release.month, equality); },
    [](Movie searchArr[], int pos, Movie toCompare, bool equality) -> bool{ 
        return CompareFunc(searchArr[pos].release.day, toCompare.release.day, equality); }
};

void StoreMatches(Movie searchArr[], Movie storeArr[], int someMatchPos, Movie toCompare, const int type){
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

void BinSearch(Movie searchArr[], Movie storeArr[], int l, int r, Movie toCompare, const int type){
    while (l <= r) {
        int m = l + (r - l) / 2;
 
        // check if x is present at mid
        if (BinCompare[type](searchArr, m, toCompare, 1)){
            StoreMatches(searchArr, storeArr, m, toCompare, type);
            return;
        }
 
        // if x is greater, ignore left half
        if (BinCompare[type](searchArr, m, toCompare, 0))
            l = m + 1;
 
        // if x is smaller, ignore right half
        else
            r = m - 1;
    }
 
    // if we reach here, the element was not present
    std::wcout << "NOT PRESENT\n";
    return;
}