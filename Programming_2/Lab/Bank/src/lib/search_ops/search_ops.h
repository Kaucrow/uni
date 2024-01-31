#pragma once
#include "../structs.h"
#include "../list/list.h"

/**
 * @brief Binary search function implementation.
 * @param searchArr - Frag list array to search for an element.
 * @param l - First index of the search array.
 * @param r - Last index of the search array.
 * @param search - Data field (of int or string type) to match.
 * @return  - If a match is found, will return its index on the searchArr.
 *          - If no match is found, will return -1.
 */
template<typename FragT, typename DataT>
int BinSearch(const List<FragT> &searchList, int l, int r, const DataT search, bool retClosest = false);