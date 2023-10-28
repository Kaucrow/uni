#pragma once
#include "../structs.h"

int BinSearch(Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type);
void StoreNewMovie(Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type);