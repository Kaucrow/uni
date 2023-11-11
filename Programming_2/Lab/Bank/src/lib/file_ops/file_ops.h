#pragma once
#include <iostream>     // For debugging.
#include <string>
#include <fstream>
#include "../structs.h"

using std::string, std::ofstream, std::ifstream, std::getline;

// ================================
//      GENERAL PURPOSE
// ================================
/**
 * @brief Appends a line to the specified file.
 * @param filePath - Path to the file to append to.
 * @param line - Line to append.
 */
void AppendLine(const char* filePath, string line);

/**
 * @brief Replaces a line with another, in the specified file.
 * @param filePath - Path to the file.
 * @param line - Line to replace with.
 * @param replaceNum - Number of line to replace.
 */
void ReplaceLine(const char* filePath, string line, int replaceNum);

/**
 * @brief Get the number of lines in a file.
 * @param filePath - Path to the file.
 * @param ignoreBlankLines - If true, the count will ignore all blank lines.
 * @return Number of lines in the file.
*/
int GetFileNumOfLines(const char* filePath, bool ignoreBlankLines = false);

// ==================================
//      FILE-SPECIFIC
// ==================================
/**
 * @brief Stores the movies from the movies.csv file into the base list array.
 * @param baseList - Base list array to store the movies in.
 * @param movFilePath - Path to the movies.csv file.
 */
void PopulateClientList(Client baseList[], const char* clientFilePath);

/**
 * @brief Stores the balance data from the client_ops.csv file into the base list array.
 * @param baseList - Base client list array to store the balance data in.
 * @param clientOpsPath - Path to the client_ops.csv file.
*/
void PopulateClientListBalance(Client baseList[], const char* clientOpsPath);