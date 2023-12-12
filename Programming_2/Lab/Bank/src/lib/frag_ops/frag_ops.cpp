#include "../search_ops/search_ops.h"
#include "frag_ops.h"

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
    arr.total++;
    arr.CheckData();

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

template<typename ListT, typename DataT>
void DelFrag(ListT &arr, int l, int r, const DataT toDel, int delID){
    arr.total--;

    // Get the pos of the element to delete. //
    int delPos = BinSearch(arr, l, r + 1, toDel);

    // Shift every frag down, until the end of the list is reached. //
    for(int i = delPos; i < r + 1; i++){
        arr.data[i] = arr.data[i + 1];
        //std::cout << arr.data[i].data << '\n';
        //std::cin.get();
    }

    // Change the IDs that need to be changed. //
    for(int i = l; i < r + 1; i++)
        if(arr.data[i].ID >= delID)
            arr.data[i].ID--;
}

template void StoreNewFrag<List<IntFrag>, int>(List<IntFrag> &arr, int l, int r, const int toStore);
template void StoreNewFrag<List<StrFrag>, string>(List<StrFrag> &arr, int l, int r, const string toStore);
template void StoreNewFrag<List<LLIntFrag>, long long int>(List<LLIntFrag> &arr, int l, int r, const long long int toStore);
template void DelFrag<List<IntFrag>, int>(List<IntFrag> &arr, int l, int r, const int toDel, int delID);
template void DelFrag<List<StrFrag>, string>(List<StrFrag> &arr, int l, int r, const string toDel, int delID);
template void DelFrag<List<LLIntFrag>, long long int>(List<LLIntFrag> &arr, int l, int r, const long long int toDel, int delID);