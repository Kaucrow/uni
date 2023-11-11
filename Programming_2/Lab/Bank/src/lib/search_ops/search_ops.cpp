#include "search_ops.h"
#include <iostream>     // For debugging.
#include <string>

using std::string;

template<typename FragT, typename DataT>
int BinSearch(const FragT searchArr[], int l, int r, const DataT search){
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
    return -1;
}

template int BinSearch<IntFrag, int>(const IntFrag searchArr[], int l, int r, const int search);
template int BinSearch<LLIntFrag, long long int>(const LLIntFrag searchArr[], int l, int r, const long long int search);
template int BinSearch<StrFrag, string>(const StrFrag searchArr[], int l, int r, const string search);