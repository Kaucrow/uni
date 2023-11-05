#pragma once
#include <fstream>
using std::wstring;

/**
 * @brief Updates the movies.csv file when renting a movie.
 * @param csvFileName - Path to the movies.csv file.
 * @param movieID - ID number of the movie to be rented.
 * @param username - Username of the person renting the movie.
 * @param date - wstring object containing the date of the rent.
 * @see RentMovieImplem
 */
void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring date);