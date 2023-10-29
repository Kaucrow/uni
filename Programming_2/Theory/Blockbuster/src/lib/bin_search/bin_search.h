#pragma once
#include "../structs.h"

int BinSearch(Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type, bool getMatches = 1);
void StoreNewMovie(Movie arr[], int l, int r, const Movie &toCompare, const int type);