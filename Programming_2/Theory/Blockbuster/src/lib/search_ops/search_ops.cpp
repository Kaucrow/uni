#include "search_ops.h"
#include <iostream>     // For debug.
#include <string>
#include <cmath>
using std::wstring, std::abs;

template<typename FragT, typename DataT>
void StoreMatches(const FragT searchArr[], int storeArr[], int someMatchPos, const DataT search, bool lesserMatch){
    int storeIndex = 0;     // Next index to store a match at.

    if(!lesserMatch){
        // Search down the searchArr for matches. //
        for(int offset = 0; (searchArr[someMatchPos + offset].data == search); offset--){
            storeArr[storeIndex] = searchArr[someMatchPos + offset].ID;
            storeIndex++;
        }
        
        // Search up the searchArr for matches. //
        for(int offset = 1; (searchArr[someMatchPos + offset].data == search); offset++){
            storeArr[storeIndex] = searchArr[someMatchPos + offset].ID;
            storeIndex++;
        }
    }
    else{
        // Get the topPos, which is the highest data in the searchArr that is equal or less than the search. //
        int offset = 0, topPos = 0;
        while(searchArr[someMatchPos + offset].data <= search && searchArr[someMatchPos + offset].ID != 0) offset += 10;
        while(searchArr[someMatchPos + offset].data > search && searchArr[someMatchPos + offset].ID != 0) offset--;
        topPos = someMatchPos + offset;

        // Get the ID of every element that comes before the searchArr topPos index. //
        if(searchArr[topPos].ID != 0){
            for(offset = 0; searchArr[topPos + offset].ID != 0; offset--){
                storeArr[storeIndex] = searchArr[someMatchPos + offset].ID;
                storeIndex++;
            }
        }
    }
    // Store a 0 in the array element that comes after the last match element,
    // in order to mark the end of the matches in the array.
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
void BinSearchStoreMatches(const FragT searchArr[], int storeArr[], int l, int r, const DataT search, bool lesserMatch){
    int m = BinSearch(searchArr, l, r, search, lesserMatch);
    if(m != -1){ StoreMatches(searchArr, storeArr, m, search, lesserMatch); }
    else storeArr[0] = 0;
}

void GenreSearchStoreMatches(const Movie baseList[], int storeArr[], int l, int r, const wstring search){
    int storeIndex = 0;
    for(l; l <= r; l++){
        for(int j = 0; (baseList[l].genres[j] != L""); j++){
            if(baseList[l].genres[j] == search){
                storeArr[storeIndex] = l;
                storeIndex++;
                break;
            }
        }
    }
    // Store a 0 in the array element that comes after the last match element,
    // in order to mark the end of the matches in the array.
    storeArr[storeIndex] = 0;
}

template int BinSearch<IntFrag, int>(const IntFrag searchArr[], int l, int r, const int search, bool retClosest);
template int BinSearch<WstrFrag, wstring>(const WstrFrag searchArr[], int l, int r, const wstring search, bool retClosest);
template void BinSearchStoreMatches<IntFrag, int>(const IntFrag searchArr[], int storeArr[], int l, int r, const int search, bool lesserMatch);
template void BinSearchStoreMatches<WstrFrag, wstring>(const WstrFrag searchArr[], int storeArr[], int l, int r, const wstring search, bool lesserMatch);