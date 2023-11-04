#pragma once
#include "../structs.h"

/**
 * @brief Binary search function implementation.
 * @param searchArr - Movie list array to search for an element.
 * @param storeArr - Movie list array to store matching movies in.
 * @param l - First index of the search array.
 * @param r - Last index of the search array.
 * @param search - Movie struct to match.
 * @param type - Data field type that is being searched for.
 * @param storeMatches - If true, the matching movies will get stored in the storeArr, otherwise, they won't.
 * @return  If a matching movie is found, will return its index on the searchArr.
 *          If no match is found, and storeMatches is true, will return -1.
 *          If no match is found, and storeMatches is false, will return the index of the movie that matches the search the closest.
*/
int BinSearch(const Movie searchArr[], Movie storeArr[], int l, int r, const Movie &search, const int type, bool storeMatches = 1);