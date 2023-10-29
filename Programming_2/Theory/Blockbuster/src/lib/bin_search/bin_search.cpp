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

void StoreMatches(Movie searchArr[], Movie storeArr[], int someMatchPos, const Movie &toCompare, const int type){
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

int BinSearch(Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // check if x is present at mid
        if (BinCompare[type](searchArr, m, toCompare, 1)){
            StoreMatches(searchArr, storeArr, m, toCompare, type);
            return -999;
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
    std::wcout << searchArr[m].title << '\n';
    return m;
}

void StoreNewMovie(Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type){
    int pivot = BinSearch(searchArr, storeArr, l, r, toCompare, type);
    if(pivot == -999){ std::wcerr << "ERR: COULD NOT STORE MOVIE OF TYPE " << type << ".\n"; return; }

    std::wcout << "BEGIN DEBUGGING\n";
    //int storeAt;

    if(pivot > 11){ pivot -= 10; }
    else{ pivot = 0; } 

    int storeAt = pivot;
    std::wcout << searchArr[storeAt].title << '\n';
    for(int i = 0; BinCompare[type](searchArr, pivot + i, toCompare, 0); i++){ storeAt++; }
    std::wcout << searchArr[storeAt].title << '\n';
    
    // debug
    std::wcout << '\n';
    if(pivot > 20){
        for(int i = pivot - 20; i < pivot + 20; i++){
            std::wcout << searchArr[i].title << '\n';
        }
    } else{
        for(int i = pivot; i < pivot + 20; i++){
            std::wcout << searchArr[i].title << '\n';
        }
    }
    std::wcout << '\n';
    std::wcout << searchArr[storeAt].title << '\n';
}