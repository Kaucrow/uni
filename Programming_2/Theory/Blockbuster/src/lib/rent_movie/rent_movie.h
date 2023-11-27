#pragma once
#include "../list/list.h"
#include "../structs.h"
#include "../basic_defs.h"
#include <fstream>

// Used for the rentOrReturn param. //
enum { UPDATE_RENT, UPDATE_RETURN };

/**
 * Param rentOrReturn appears in 3 functions here. This is its description:
* @param rentOrRenturn - Flag which tells the function whether the action to perform is a movie rent or return.
*/

/**
 * @brief Updates the movies.csv file when renting/returning a movie.
 * @param csvFileName - Path to the movies.csv file.
 * @param movieID - ID number of the movie to be rented/returned.
 * @param username - Username of the person renting/returning the movie.
 * @param date - pstring object containing the date of the rent.
 * @param expiry - pstring object containing the expiry date of the rent.
 */
void UpdateMoviesCsv(const char* csvFilePath, int movieID, pstring username, pstring date, pstring expiryDate, bool rentOrReturn);

/**
 * @brief Updates the base list data when renting/returning a movie.
 * @param movieID - ID of the movie to rent/return.
 * @param username - Name of the person renting/returning the movie.
 * @param rentDate - pstring object containing the date of the rent, in Y-M-d format.
 * @param expiryDate - pstring object containing the date of the rent expiry, in Y-M-d format.
*/
void UpdateMovieLiveData(List<Movie> &baseList, int movieID, pstring username, pstring rentDate, pstring expiryDate, bool rentOrReturn);

/**
 * @brief Updates the users_data.csv file and the live user list data when renting/returning a movie.
 * @param usersDataFilePath - Path to the users_data.csv file.
 * @param userList - User list.
 * @param currUser - ID of the user renting/returning the movie.
 * @param movieTtl - pstring object containing the title of the movie to rent/return.
*/
void UpdateUsersData(const char* usersDataFilePath, List<User> &userList, int currUser, pstring movieTtl, bool rentOrReturn);

/**
 * @brief Queries the base list for the status of a movie.
 * @param baseList - Base movie list to query.
 * @param ttlFrag - Title frag list.
 * @param totalMovies - Count of total movies in the the database.
 * @param title - pstring object containing the title of the movie to query.
 * @param queryMovieID - variable that should hold the ID of the queried movie after the function execution.
*/
enum { QUERY_RENT_NOTFOUND = -1, QUERY_RENT_NOTRENTED = 0, QUERY_RENT_RENTED = 1 };
int QueryMovieRent(List<Movie> &baseList, List<PStrFrag> &ttlFrag, int totalMovies, pstring title, int& queryMovieID);