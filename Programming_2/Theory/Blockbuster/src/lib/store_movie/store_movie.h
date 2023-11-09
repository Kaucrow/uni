#pragma once
#include "../structs.h"

/**
 * @brief Stores a movie frag in the frag list provided, while preserving its sorting order.
 * @param arr - Frag list array to store the movie frag in.
 * @param l - First arr index.
 * @param r - Last arr index.
 * @param toStore - Data to store in the frag list.
 * @see StoreNewMovieImplem "StoreNewMovie() implementation details".
 * This function uses the value of param 'r' both as the last arr index to check
 * for the position to store the frag at and the ID of the frag stored.
 */
template<typename FragT, typename DataT>
void StoreNewFrag(FragT arr[], int l, int r, const DataT toStore);