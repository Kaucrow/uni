#pragma once
#include "../structs.h"
#include <fstream>
using std::wstring;

// Used for the rentOrReturn param. //
enum { UPDATE_RENT, UPDATE_RETURN };

/**
 * Param rentOrReturn appears in 4 functions here. This is its description:
* @param rentOrRenturn - Flag which tells the function whether the action to perform is a movie rent or return.
*/

/**
 * @brief Updates the movies.csv file when renting/returning a movie.
 * @param csvFileName - Path to the movies.csv file.
 * @param movieID - ID number of the movie to be rented/returned.
 * @param username - Username of the person renting/returning the movie.
 * @param date - wstring object containing the date of the rent.
 * @param expiry - wstring object containing the expiry date of the rent.
 */
void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring date, wstring expiryDate, bool rentOrReturn);

/**
 * @brief Updates the users_data.csv file when renting/returning a movie.
 * @param usersDataFilePath - Path to the users_data.csv file.
 * @param currUser - ID of the user renting/returning the movie.
 * @param movieTtl - wstring object containing the title of the movie to rent/return.
*/
void UpdateUsersDataCsv(const char* usersDataFilePath, int currUser, wstring movieTtl, bool rentOrReturn);

/**
 * @brief Updates the base list data when renting/returning a movie.
 * @param movieID - ID of the movie to rent/return.
 * @param username - Name of the person renting/returning the movie.
 * @param rentDate - wstring object containing the date of the rent, in Y-M-d format.
 * @param expiryDate - wstring object containing the date of the rent expiry, in Y-M-d format.
*/
void UpdateMovieData(Movie baseList[], int movieID, wstring username, wstring rentDate, wstring expiryDate, bool rentOrReturn);

/**
 * @brief Updates the user list data when renting/returning a movie.
 * @param currUser - ID of the user renting/returning the movie.
 * @param movieTtl - wstring object containing the title of the movie to rent/return.
*/
void UpdateUsersData(User userList[], int currUser, wstring movieTtl, bool rentOrReturn);

/**
 * @brief Queries the base list for the status of a movie.
 * @param baseList - Base movie list to query.
 * @param ttlFrag - Title frag list.
 * @param totalMovies - Count of total movies in the the database.
 * @param title - wstring object containing the title of the movie to query.
 * @param queryMovieID - variable that should hold the ID of the queried movie after the function execution.
*/
enum { QUERY_RENT_NOTFOUND = -1, QUERY_RENT_NOTRENTED = 0, QUERY_RENT_RENTED = 1 };
int QueryMovieRent(Movie baseList[], WstrFrag ttlFrag[], int totalMovies, wstring title, int& queryMovieID);