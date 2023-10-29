#pragma once
#include "../structs.h"

int BinSearch(const Movie searchArr[], Movie storeArr[], int l, int r, const Movie &toCompare, const int type, bool storeMatches = 1);