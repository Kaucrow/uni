#include "merge_sort.h"
#include <iostream>     // For debugging.

// For use only by MergeSort(). //
template<typename FragT>
void Merge(FragT arr[], int l, int m, int r){
    int n1 = m - l + 1;     // Amount of elements of the L subarray.
    int n2 = r - m;         // Amount of elements of the R subarray.

    // Create temp L and R arrays.
    FragT* L{ new FragT [n1] };
    FragT* R{ new FragT [n2] };

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
        if(L[i].data <= R[j].data){
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

template<typename FragT>
void MergeSort(FragT arr[], int l, int r){
    if(l < r){
        // Same as (l + r) / 2, but avoids overflow for large l and r.
        int m = l + (r - l) / 2;

        // Sort first and second halves.
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);

        // Merge the sorted halves.
        Merge(arr, l, m, r);
    }
}

template void Merge<IntFrag>(IntFrag arr[], int l, int m, int r);
template void Merge<LLIntFrag>(LLIntFrag arr[], int l, int m, int r);
template void Merge<StrFrag>(StrFrag arr[], int l, int m, int r);
template void MergeSort<IntFrag>(IntFrag arr[], int l, int r);
template void MergeSort<LLIntFrag>(LLIntFrag arr[], int l, int r);
template void MergeSort<StrFrag>(StrFrag arr[], int l, int r);