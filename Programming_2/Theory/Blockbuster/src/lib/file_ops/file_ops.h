#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../structs.h"
using std::wstring, std::wofstream, std::wifstream, std::getline;

void AppendLine(const char* filePath, wstring line);
void ReplaceLine(const char* filePath, wstring line, int lineNum);
int GetLastLineFirstNum(const char* filePath);

/**
 * @brief Puts the movies in the movies.csv file into a movie list array.
 * @param movList - Movie list array to store the movies in.
 * @param inFile - wifstream object that contains the movies.csv file.
 */
void PopulateMovieList(Movie movList[], int movieNum, const char* movFilePath);