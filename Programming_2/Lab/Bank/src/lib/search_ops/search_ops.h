#pragma once
#include "../structs.h"

/**
 * @brief Binary search function implementation.
 * @param searchArr - Frag list array to search for an element.
 * @param l - First index of the search array.
 * @param r - Last index of the search array.
 * @param search - Data field (of int or wstring type) to match.
 * @param retClosest - If true, will always return a match, otherwise the function will work as a normal bin search.
 * @return  - If a match is found, will return its index on the searchArr.
 *          - If no match is found, and retClosest is false, will return -1.
 *          - If no match is found, and retClosest is true, will return the index
 *          of the closest match on the searchArr.
 */
template<typename FragT, typename DataT>
int BinSearch(const FragT searchArr[], int l, int r, const DataT search, bool retClosest = false);

/**
 * @brief Performs a binary search and stores the ID of all matches in the storeArr.
 * All params included have the same function as the ones in BinSearch(), except for:
 * @param storeArr - int array to store the IDs of the matches in.
 * @param lesserMatch   - If false, the search will only store the ID of elements that have a data that is exactly equal to the search.
 *                      - If true, the search will store the ID of every element that has a data equal or less than the search.
 * If no match is found, a 0 will be stored in the first index of the storeArr.
*/
template<typename FragT, typename DataT>
void BinSearchStoreMatches(const FragT searchArr[], int storeArr[], int l, int r, const DataT search, bool lesserMatch = false);

/**
 * @brief Performs a linear search in the base movie list looking for movies with 
 * a genre matching the search arg, and stores the ID of all matches in the storeArr.
 * All params included have the same function as the ones in BinSearch().
 * If no match is found, a 0 will be stored in the first index of the storeArr.
*/
void GenreSearchStoreMatches(const Movie baseList[], int storeArr[], int l, int r, const wstring search);