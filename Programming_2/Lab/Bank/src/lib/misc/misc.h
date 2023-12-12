#pragma once
#include <string>
#include <filesystem>

#ifdef _WIN32
    #include <libloaderapi.h>
    #include <sys/stat.h> 
#else
    #include <libgen.h>
    #include <unistd.h>
    #include <linux/limits.h>
#endif

using std::string;

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

/**
 * @brief Gets the path to the "data" dir.
 * @return The path to the data dir, if found. Otherwise, returns an empty string.
*/
string GetDataDir();