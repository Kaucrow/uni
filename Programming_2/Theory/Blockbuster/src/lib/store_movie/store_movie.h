#pragma once
#include "../structs.h"

/**
 * @brief Stores a movie in the movie list provided, while preserving its sorting order.
 * @param arr - Movie list array to store the movie in.
 * @param l - First arr index.
 * @param r - Last arr index.
 * @param toCompare - Movie struct to
 */
void StoreNewMovie(Movie arr[], int l, int r, const Movie &toStore, const int type);