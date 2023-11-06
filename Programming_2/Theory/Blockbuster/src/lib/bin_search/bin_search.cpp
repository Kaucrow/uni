#include "bin_search.h"
#include <iostream>     // For debug.
#include <string>
using std::wstring;

template<typename FragT, typename DataT>
void StoreMatches(const FragT searchArr[], int storeArr[], int someMatchPos, const DataT search){
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
    storeArr[storeIndex] = 0;
}

template<typename FragT, typename DataT>
int BinSearch(const FragT searchArr[], int l, int r, const DataT search, bool retClosest){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // Check if x is present at mid.
        if(searchArr[m].data == search)
            return m;
 
        // If x is greater, ignore left half.
        if(searchArr[m].data < search)
            l = m + 1;
 
        // If x is smaller, ignore right half.
        else
            r = m - 1;
    }
 
    // If we reach here, the element was not present.
    if(retClosest) return m;
    else return -1;
}

template<typename FragT, typename DataT>
int BinSearchStoreMatches(const FragT searchArr[], int storeArr[], int l, int r, const DataT search){
    int m = BinSearch(searchArr, l, r, search);
    if(m != -1){ StoreMatches(searchArr, storeArr, m, search); }
    return m;
}

template int BinSearch<IntFrag, int>(const IntFrag searchArr[], int l, int r, const int search, bool retClosest);
template int BinSearch<WstrFrag, wstring>(const WstrFrag searchArr[], int l, int r, const wstring search, bool retClosest);
template int BinSearchStoreMatches<IntFrag, int>(const IntFrag searchArr[], int storeArr[], int l, int r, const int search);
template int BinSearchStoreMatches<WstrFrag, wstring>(const WstrFrag searchArr[], int storeArr[], int l, int r, const wstring search);