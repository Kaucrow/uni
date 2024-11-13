#include "search_ops.h"
#include <iostream>     // For debugging.
#include <string>

using std::string;

template<typename FragT, typename DataT>
int BinSearch(const List<FragT> &searchList, int l, int r, const DataT search, bool retClosest){
    int m;
    while (l <= r) {
        m = l + (r - l) / 2;
        // Check if x is present at mid.
        if(searchList.data[m].data == search)
            return m;
 
        // If x is greater, ignore left half.
        if(searchList.data[m].data < search)
            l = m + 1;
 
        // If x is smaller, ignore right half.
        else
            r = m - 1;
    }
 
    // If we reach here, the element was not present.
    if(retClosest) return m;
    return -1;
}

template int BinSearch<IntFrag, int>(const List<IntFrag> &searchList, int l, int r, const int search, bool retClosest);
template int BinSearch<LLIntFrag, long long int>(const List<LLIntFrag> &searchList, int l, int r, const long long int search, bool retClosest);
template int BinSearch<StrFrag, string>(const List<StrFrag> &searchList, int l, int r, const string search, bool retClosest);