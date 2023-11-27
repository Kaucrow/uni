#include "../search_ops/search_ops.h"
#include "./store_movie.h"
#include <string>

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
template<typename ListT, typename DataT>
void StoreNewFrag(ListT &arr, int l, int r, const DataT toStore){
    // Get the pivot. //
    int pivot = BinSearch(arr, l, r - 1, toStore, true);
    // Go back a number of elements. //
    if(pivot > 10){ pivot -= 10; }
    else{ pivot = 1; } 

    int storeAt = pivot;

    // Get the index where the frag should be stored at. //
    for(int i = 0; arr.data[pivot + i].data < toStore && (pivot + i) < r; i++){ storeAt++; }
    
    // Shift every frag up, until the storeAt index is reached. //
    for(int i = r; i > storeAt; i--){ arr.data[i] = arr.data[i - 1]; }

    // Store the frag at the storeAt index. //
    arr.data[storeAt].ID = r;
    arr.data[storeAt].data = toStore;
}

template void StoreNewFrag<List<IntFrag>, int>(List<IntFrag> &arr, int l, int r, const int toStore);
template void StoreNewFrag<List<PStrFrag>, pstring>(List<PStrFrag> &arr, int l, int r, const pstring toStore);