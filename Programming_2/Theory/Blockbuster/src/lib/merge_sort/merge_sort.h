#pragma once
#include "../basic_defs.h"
#include "../structs.h"

/**
 * @brief Merge sort function implementation for sorting frag lists.
 * @param arr - Frag list array to sort.
 * @param l - First index of the array.
 * @param r - Last index of the array.
 */
template <typename FragT>
void MergeSort(FragT arr[], int l, int r);