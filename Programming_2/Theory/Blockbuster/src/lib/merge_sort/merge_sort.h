#pragma once
#include "../structs.h"

/**
 * @brief Merge sort function implementation.
 * @param arr - Array to sort.
 * @param l - First index of the array.
 * @param r - Last index of the array.
 * @param sortBy - Type of data field to sort by (DURATION, TITLE, DIRECTOR, YEAR, MONTH, DAY).
*/
void MergeSort(Movie arr[], int l, int r, const int sortBy);

template <typename T>
void MergeSortAlt(T arr[], int l, int r);