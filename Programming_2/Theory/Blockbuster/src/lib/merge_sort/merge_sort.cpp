#include "merge_sort.h"
#include <functional>
using std::function;

function<bool(Movie L[], Movie R[], int i, int j)> Compare[6] = {
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].duration <= R[j].duration; },
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].title <= R[j].title; },
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].director <= R[j].director; },
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].release.year <= R[j].release.year; },
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].release.month <= R[j].release.month; },
    [](Movie L[], Movie R[], int i, int j) -> bool{ return L[i].release.day <= R[j].release.day; }
};

void Merge(Movie listFrom[], int l, int m, int r, const int sortBy){
    int n1 = m - l + 1;     // amount of elements of the L subarray
    int n2 = r - m;         // amount of elements of the R subarray

    // create temp L and R arrays
    Movie L[n1];
    Movie R[n2];

    // copy data to temp arrays L[] and R[]
    for(int i = 0; i < n1; i++)
        L[i] = listFrom[l + i];
    for(int j = 0; j < n2; j++)
        R[j] = listFrom[m + 1 + j];

    // merge the temp arrays back into movies[l..r]
    int i = 0;  // initial index of first subarray
    int j = 0;  // initial index of second subarray
    int k = l;  // initial index of merged subarray

    while(i < n1 && j < n2){
        if(Compare[sortBy](L, R, i, j)){
            listFrom[k] = L[i];
            i++;
        }
        else{
            listFrom[k] = R[j];
            j++;
        }
        k++;
    }

    // copy the remaining elements of L[], if any
    while(i < n1){
        listFrom[k] = L[i];
        i++;
        k++;
    }

    // copy the remaining elements of R[], if any
    while(j < n2){
        listFrom[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort(Movie listFrom[], int l, int r, const int sortBy){
    if(l < r){
        // same as (l + r) / 2, but avoids overflow for large l and r
        int m = l + (r - l) / 2;

        // sort first and second halves
        MergeSort(listFrom, l, m, sortBy);
        MergeSort(listFrom, m + 1, r, sortBy);

        // merge the sorted halves
        Merge(listFrom, l, m, r, sortBy);
    }
}