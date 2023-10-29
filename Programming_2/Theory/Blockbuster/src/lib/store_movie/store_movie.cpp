#include "../bin_search/bin_search.h"
#include <functional>
using std::function;

extern function<bool(const Movie searchArr[], int pos, Movie toCompare, bool equality)> BinCompare[6];

void StoreNewMovie(Movie arr[], int l, int r, const Movie &toCompare, const int type){
    int pivot = BinSearch(arr, arr, l, r - 1, toCompare, type, 0);

    if(pivot > 11){ pivot -= 10; }
    else{ pivot = 1; } 

    int storeAt = pivot;

    for(int i = 0; BinCompare[type](arr, pivot + i, toCompare, 0); i++){ storeAt++; }
    for(int i = r; i > storeAt; i--){ arr[i] = arr[i - 1]; }
    arr[storeAt] = toCompare;
}