#pragma once
#include <fstream>
using std::wstring;

void RentMovie(std::wfstream &csvFile, int movieID, wstring username, wstring date);