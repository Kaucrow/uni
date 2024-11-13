#include "merge_sort.h"
#include <iostream>

/* For use only by MergeSort() */
template<typename ListT>
void Merge(ListT &arr, int l, int m, int r){
    int n1 = m - l + 1;     // Amount of elements of the L subarray.
    int n2 = r - m;         // Amount of elements of the R subarray.

    // Create temp L and R arrays.
    ListT* L{ new ListT(n1) };
    ListT* R{ new ListT(n2) };

    // Copy data to temp arrays L[] and R[].
    for(int i = 0; i < n1; i++)
        L->data[i] = arr.data[l + i];
    for(int j = 0; j < n2; j++)
        R->data[j] = arr.data[m + 1 + j];

    // Merge the temp arrays back into movies[l..r].
    int i = 0;  // Initial index of first subarray.
    int j = 0;  // Initial index of second subarray.
    int k = l;  // Initial index of merged subarray.

    while(i < n1 && j < n2){
        if(L->data[i].data <= R->data[j].data){
            arr.data[k] = L->data[i];
            i++;
        }
        else{
            arr.data[k] = R->data[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any.
    while(i < n1){
        arr.data[k] = L->data[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any.
    while(j < n2){
        arr.data[k] = R->data[j];
        j++;
        k++;
    }

    delete L; delete R;
}

template<typename ListT>
void MergeSort(ListT &arr, int l, int r){
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

template void Merge<List<IntFrag>>(List<IntFrag> &arr, int l, int m, int r);
template void Merge<List<PStrFrag>>(List<PStrFrag> &arr, int l, int m, int r);
template void Merge<List<LLIntFrag>>(List<LLIntFrag> &arr, int l, int m, int r);
template void MergeSort<List<IntFrag>>(List<IntFrag> &arr, int l, int r);
template void MergeSort<List<PStrFrag>>(List<PStrFrag> &arr, int l, int r);
template void MergeSort<List<LLIntFrag>>(List<LLIntFrag> &arr, int l, int r);