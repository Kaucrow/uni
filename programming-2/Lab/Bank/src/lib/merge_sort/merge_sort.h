#pragma once
#include "../structs.h"
#include "../list/list.h"

/**
 * @brief Merge sort function implementation for sorting frag lists.
 * @param list - Frag list to sort.
 * @param l - First index of the array.
 * @param r - Last index of the array.
 */
template <typename FragT>
void MergeSort(List<FragT> &list, int l, int r);