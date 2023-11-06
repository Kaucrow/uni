#include "../bin_search/bin_search.h"
#include <string>
#include <functional>
using std::wstring, std::function;

/**
 * @file ../bin_search/bin_search.h
 * @see BinCompareDesc "BinCompare description"
 */
//extern function<bool(const Movie searchArr[], int pos, Movie toCompare, bool equality)> BinCompare[6];

/**
 * @name StoreNewMovieImplem
 * Firstly, performs a binary search on the movie list provided, looking for a movie with
 * a data field matching the one of the "toStore" movie. If a movie match is found,
 * its index gets set as the pivot. Otherwise, the index of the closest match gets
 * set as the pivot. Then, from the pivot, the function goes back a number of elements
 * (usually 10), and starts looking up the arr for the index where the new movie
 * should be stored at. Once it finds it, it shifts up all the movies in the arr
 * starting from the last one, until it reaches the index of the arr where the new
 * movie should be stored at, and stores it there.
 */
template<typename T, typename U>
void StoreNewFrag(T arr[], int l, int r, const U toStore){
    /* Get the pivot. */
    int pivot = BinSearch(arr, l, r - 1, toStore, true);

    /* Go back a number of elements. */
    if(pivot > 10){ pivot -= 10; }
    else{ pivot = 1; } 

    int storeAt = pivot;

    /* Get the index where the movie should be stored at. */
    for(int i = 0; arr[pivot + i].data < toStore; i++){ storeAt++; }

    /* Shift every movie up, until the storeAt index is reached. */
    for(int i = r; i > storeAt; i--){ arr[i] = arr[i - 1]; }

    /* Store the movie at the storeAt index. */
    arr[storeAt].ID = r;
    arr[storeAt].data = toStore;
}

template void StoreNewFrag<IntFrag, int>(IntFrag arr[], int l, int r, const int toStore);
template void StoreNewFrag<WstrFrag, wstring>(WstrFrag arr[], int l, int r, const wstring toStore);