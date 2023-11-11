#include "../search_ops/search_ops.h"
#include <string>
using std::wstring;

/**
 * @name StoreNewFragImplem
 * Firstly, performs a binary search on the frag list provided, looking for a frag with
 * a data field matching the one of the "toStore" data. If a frag match is found,
 * its index gets set as the pivot. Otherwise, the index of the closest match gets
 * set as the pivot. Then, from the pivot, the function goes back a number of elements
 * (usually 10), and starts looking up the arr for the index where the new frag
 * should be stored at. Once it finds it, it shifts up all the movies in the arr
 * starting from the last one, until it reaches the index of the arr where the new
 * frag should be stored at, and stores it there.
 */
template<typename FragT, typename DataT>
void StoreNewFrag(FragT arr[], int l, int r, const DataT toStore){
    // Get the pivot. //
    int pivot = BinSearch(arr, l, r - 1, toStore, true);

    // Go back a number of elements. //
    if(pivot > 10){ pivot -= 10; }
    else{ pivot = 1; } 

    int storeAt = pivot;

    // Get the index where the frag should be stored at. //
    for(int i = 0; arr[pivot + i].data < toStore; i++){ storeAt++; }

    // Shift every frag up, until the storeAt index is reached. //
    for(int i = r; i > storeAt; i--){ arr[i] = arr[i - 1]; }

    // Store the frag at the storeAt index. //
    arr[storeAt].ID = r;
    arr[storeAt].data = toStore;
}

template void StoreNewFrag<IntFrag, int>(IntFrag arr[], int l, int r, const int toStore);
template void StoreNewFrag<WstrFrag, wstring>(WstrFrag arr[], int l, int r, const wstring toStore);