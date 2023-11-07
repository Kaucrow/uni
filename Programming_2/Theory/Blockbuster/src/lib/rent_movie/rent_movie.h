#pragma once
#include "../structs.h"
#include <fstream>
using std::wstring;

/**
 * @brief Updates the movies.csv file when renting a movie.
 * @param csvFileName - Path to the movies.csv file.
 * @param movieID - ID number of the movie to be rented.
 * @param username - Username of the person renting the movie.
 * @param date - wstring object containing the date of the rent.
 */
void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring date, wstring expiryDate);

void UpdateUsersDataCsv(const char* usersDataFilePath, int currUser, wstring movieTtl);

void UpdateMovieData(Movie baseList[], int movieID, wstring username, wstring rentDate, wstring expiryDate);

void UpdateUsersData(User userList[], int currUser, wstring movieTtl);

enum { QUERY_RENT_NOTFOUND = -1, QUERY_RENT_NOTRENTED = 0, QUERY_RENT_RENTED = 1 };
int QueryMovieRent(Movie baseList[], WstrFrag ttlFrag[], int totalMovies, wstring title, int& queryMovieID);