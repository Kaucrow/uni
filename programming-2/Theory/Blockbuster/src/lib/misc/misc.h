#pragma once

#ifdef _WIN32
    #include <libloaderapi.h>
    #include <sys/stat.h> 
#else
    #include <libgen.h>
    #include <unistd.h>
    #include <linux/limits.h>
#endif
#include <filesystem>

#include <iostream>
#include <string>
#include "../basic_defs.h"
using std::string;

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

/**
 * @brief Gets the path to the "data" dir.
 * @return The path to the data dir, if found. Otherwise, returns an empty pstring.
*/
string GetDataDir();

/**
 * @brief Gets the "action" input and validate it.
 * @param action - Var to store the action in.
 * @param lowBound - Lowest "action" input value accepted.
 * @param highBound - Highest "action" input value accepted.
*/
void GetAction(int& action, int lowBound, int highBound);

/**
 * @brief Gets the current date, or the current date plus 14 days.
 * @param add14Days - Depending on its value, 14 days are added to the current date or not.
 * @return Returns a pstring object with the date in "Y-M-d" format.
*/
pstring GetDate(bool add14Days = false);