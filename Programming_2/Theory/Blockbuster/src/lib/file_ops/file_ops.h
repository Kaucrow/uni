#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "../structs.h"
using std::wstring, std::wofstream, std::wifstream, std::getline;

// ================================
//      GENERAL PURPOSE
// ================================
/**
 * @brief Appends a line to the specified file.
 * @param filePath - Path to the file to append to.
 * @param line - Line to append.
 */
void AppendLine(const char* filePath, wstring line);

/**
 * @brief Replaces a line with another, in the specified file.
 * @param filePath - Path to the file.
 * @param line - Line to replace with.
 * @param replaceNum - Number of line to replace.
 */
void ReplaceLine(const char* filePath, wstring line, int replaceNum);

/**
 * @brief Gets the first number in the last line of the specified file.
 * @param filePath - Path to the file.
 * @return First number in the last line of the file.
 * WARNING: An exception will be thrown at runtime if the last line of the file does not
 * contain a number as its first character.
 */
int GetLastLineFirstNum(const char* filePath);

// ==================================
//      FILE-SPECIFIC
// ==================================
/**
 * @brief Ensures that the movies.csv file has the required fields.
 * If it doesn't have the required fields, they are added to the file.
 * @param movFilePath - Path to the movies.csv file.
 */
void CheckMoviesCsv(const char* movFilePath);

/**
 * @brief Stores the movies from the movies.csv file into the base list array.
 * @param baseList - Base list array to store the movies in.
 * @param movFilePath - Path to the movies.csv file.
 */
void PopulateMovieList(Movie baseList[], const char* movFilePath);

/**
 * @brief Stores the users data from the users_data.csv file into each element of the user list array.
 * @param userList - User list to store the users data in.
 * @param userDataFilePath - Path to the users_data.csv file.
 */
void PopulateUserList(User userList[], const char* userDataFilePath);