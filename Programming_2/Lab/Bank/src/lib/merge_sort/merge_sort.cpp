#include "merge_sort.h"
#include <iostream>     // For debugging.

// For use only by MergeSort(). //
template<typename FragT>
void Merge(List<FragT> &list, int l, int m, int r){
    int n1 = m - l + 1;     // Amount of elements of the L sublistay.
    int n2 = r - m;         // Amount of elements of the R sublistay.

    // Create temp L and R listays.
    FragT* L{ new FragT[n1] };
    FragT* R{ new FragT[n2] };

    // Copy data to temp listays L[] and R[].
    for(int i = 0; i < n1; i++)
        L[i] = list.data[l + i];
    for(int j = 0; j < n2; j++)
        R[j] = list.data[m + 1 + j];

    // Merge the temp listays back into movies[l..r].
    int i = 0;  // Initial index of first sublistay.
    int j = 0;  // Initial index of second sublistay.
    int k = l;  // Initial index of merged sublistay.

    while(i < n1 && j < n2){
        if(L[i].data <= R[j].data){
            list.data[k] = L[i];
            i++;
        }
        else{
            list.data[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any.
    while(i < n1){
        list.data[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any.
    while(j < n2){
        list.data[k] = R[j];
        j++;
        k++;
    }

    delete [] L; delete [] R;
}

template<typename FragT>
void MergeSort(List<FragT> &list, int l, int r){
    if(l < r){
        // Same as (l + r) / 2, but avoids overflow for large l and r.
        int m = l + (r - l) / 2;

        // Sort first and second halves.
        MergeSort(list, l, m);
        MergeSort(list, m + 1, r);

        // Merge the sorted halves.
        Merge(list, l, m, r);
    }
}

template void Merge<IntFrag>(List<IntFrag> &list, int l, int m, int r);
template void Merge<LLIntFrag>(List<LLIntFrag> &list, int l, int m, int r);
template void Merge<StrFrag>(List<StrFrag> &list, int l, int m, int r);
template void MergeSort<IntFrag>(List<IntFrag> &list, int l, int r);
template void MergeSort<LLIntFrag>(List<LLIntFrag> &list, int l, int r);
template void MergeSort<StrFrag>(List<StrFrag> &list, int l, int r);