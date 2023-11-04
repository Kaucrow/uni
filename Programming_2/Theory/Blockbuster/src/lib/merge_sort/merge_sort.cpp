#include "merge_sort.h"
#include <iostream>
#include <functional>
using std::function;

/**
 * @brief   Polymorphic function wrapper used by the Merge() function for sorting a movie list
 *          according to duration, title, director, release year, release month, or release day.
 * @param L - Left half of the array that is being merged.
 * @param R - Right half of the array that is being merged.
 * @param i - Index of the L array element to be compared.
 * @param j - Index of the R array element to be compared.
 * @return True if L[i].dataType <= R[j].dataType, false otherwise.
 */
function<bool(const Movie L[], const Movie R[], int i, int j)> Compare[6] = {
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].duration <= R[j].duration; },
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].title <= R[j].title; },
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].director <= R[j].director; },
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].release.year <= R[j].release.year; },
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].release.month <= R[j].release.month; },
    [](const Movie L[], const Movie R[], int i, int j) -> bool{ return L[i].release.day <= R[j].release.day; }
};

/* For use only by MergeSort() */
void Merge(Movie arr[], int l, int m, int r, const int sortBy){
    int n1 = m - l + 1;     // Amount of elements of the L subarray.
    int n2 = r - m;         // Amount of elements of the R subarray.

    // Create temp L and R arrays.
    Movie* L{ new Movie [n1] };
    Movie* R{ new Movie [n2] };

    // Copy data to temp arrays L[] and R[].
    for(int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for(int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into movies[l..r].
    int i = 0;  // Initial index of first subarray.
    int j = 0;  // Initial index of second subarray.
    int k = l;  // Initial index of merged subarray.

    while(i < n1 && j < n2){
        if(Compare[sortBy](L, R, i, j)){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any.
    while(i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any.
    while(j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }

    delete [] L; delete [] R;
}

void MergeSort(Movie arr[], int l, int r, const int sortBy){
    if(l < r){
        // Same as (l + r) / 2, but avoids overflow for large l and r.
        int m = l + (r - l) / 2;

        // Sort first and second halves.
        MergeSort(arr, l, m, sortBy);
        MergeSort(arr, m + 1, r, sortBy);

        // Merge the sorted halves.
        Merge(arr, l, m, r, sortBy);
    }
}