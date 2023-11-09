#include "rent_movie.h"
#include "../structs.h"
#include "../file_ops/file_ops.h"
#include "../bin_search/bin_search.h"
#include <iostream>
using std::getline;

/**
 * @brief Gets the position of the Nth comma in a line.
 * @param line - wstring object containing the line to search.
 * @param commaNum - Number of comma in the line to get the position of.
 * @return Position of the Nth comma in the line (zero-based).
 */
int GetNthCommaPos(wstring line, int commaNum){
    int commaCounter = 0, totalPos = 0, currPos = 0;
    while(commaCounter != commaNum){
        currPos = (line.find_first_of(','));
        totalPos += currPos;
        if(commaCounter > 0) totalPos += 1;
        line = line.substr(currPos + 1);
        commaCounter++;
    }
    return totalPos;
};

void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring rentDate, wstring expiryDate, bool rentOrReturn){
    std::wifstream csvFile(csvFilePath);        // Open the movies.csv file.
 
    wstring statusType;
    // Executes if the action is a RENT. //
    if(rentOrReturn == UPDATE_RENT) statusType = L"rented";
    // Executes if the action is a RETURN. //
    else if(rentOrReturn == UPDATE_RETURN) statusType = L"returned";

    int counter = -1;       // Counts the lines read.
    wstring readingLine;
    // Keep reading lines until the line to update is reached. //
    while(counter != movieID - 1){
        getline(csvFile, readingLine);
        counter++;
    }
    getline(csvFile, readingLine);          // Read the line to update.

    // Update the line, and write it to writeFile. //
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 6));
    readingLine.append(L',' + username + L',' + rentDate + L',' + statusType + L',' + expiryDate);
    csvFile.close();
    ReplaceLine(csvFilePath, readingLine, movieID + 1);
}

void UpdateUsersDataCsv(const char* usersDataFilePath, int currUser, wstring movieTtl, bool rentOrReturn){
    std::wifstream usersDataFile(usersDataFilePath);        // Open the users_data.csv file.

    wstring readingLine;
    int counter = -1;
    // Keep reading lines until the line to update is reached. //
    while(counter != currUser - 1){
        getline(usersDataFile, readingLine);
        counter++;
    }

    getline(usersDataFile, readingLine);    // Read the line to update.

    // Get the current user's movies (comma included). E.g: ",|Cleanskin|Ignition|Big Hero 6". //
    wstring currMovies = readingLine.substr(GetNthCommaPos(readingLine, 2));
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 2));

    // Executes if the action is a RENT. //
    if(rentOrReturn == UPDATE_RENT){
        readingLine.append(currMovies.append(L'|' + movieTtl));
    } 
    // Executes if the action is a RETURN. //
    else if(rentOrReturn == UPDATE_RETURN){
        wstring temp;
        // Executes if there's more than one movie. //
        if(currMovies.find_last_of('|') != 1){
            temp = currMovies.substr(0, currMovies.find(movieTtl));     // Get the movies string up to the one to return.
            temp.append(currMovies.substr(currMovies.find(movieTtl) + movieTtl.length() + 1));      // Append the substr that comes after the movie to return.
            readingLine.append(temp);       // Append the updated movies to the readingLine.
        }
        else{
            readingLine.append(L",");       // If there's only one movie, just append a comma to the readingLine.
        }
    }
    usersDataFile.close();

    // Update the line. //
    ReplaceLine(usersDataFilePath, readingLine, currUser + 1);
}

void UpdateMovieData(Movie baseList[], int movieID, wstring username, wstring rentDate, wstring expiryDate, bool rentOrReturn){
    // Executes if the action is a RETURN. //
    if(rentOrReturn == UPDATE_RETURN){ baseList[movieID].status = MOV_STATUS_RETURNED; return; }

    // Executes if the action is a RENT. //
    baseList[movieID].status = MOV_STATUS_RENTED;
    baseList[movieID].rentedTo = username;
    for(int i = 0; i < 3; i++){
        switch(i){
            case 0:
                baseList[movieID].rentedOn.year = stoi(rentDate);
                baseList[movieID].expiry.year = stoi(expiryDate);
                break;
            case 1:
                baseList[movieID].rentedOn.month = stoi(rentDate);
                baseList[movieID].expiry.month = stoi(expiryDate);
                break;
            case 2:
                baseList[movieID].rentedOn.day = stoi(rentDate);
                baseList[movieID].expiry.day = stoi(expiryDate);
                break;
        }
        rentDate = rentDate.substr(rentDate.find('-') + 1);
        expiryDate = expiryDate.substr(expiryDate.find('-') + 1);
    }
}

void UpdateUsersData(User userList[], int currUser, wstring movieTtl, bool rentOrReturn){
    // Executes if the action is a RENT. //
    if(rentOrReturn == UPDATE_RENT){ userList[currUser].movies.append(L'|' + movieTtl); return; }
    
    // Executes if the action is a RETURN. //
    wstring temp = userList[currUser].movies;
    temp = temp.substr(0, temp.find(movieTtl) - 1);     // Get the user's movies, up to the one to return.
    temp.append(userList[currUser].movies.substr(userList[currUser].movies.find(movieTtl) + movieTtl.length()));    // Get the substr that comes after the movie to return.
    userList[currUser].movies = temp;   // Update the movies.
}

int QueryMovieRent(Movie baseList[], WstrFrag ttlFrag[], int totalMovies, wstring title, int& queryMovieID){
    // Perform a search by title for the entered movie. //
    int ttlPos = BinSearch(ttlFrag, 1, totalMovies, title);

    // If not found, return. // 
    if(ttlPos == -1) return QUERY_RENT_NOTFOUND;
    
    // If found, get its ID, then return a rent or returned status. //
    int movPos = ttlFrag[ttlPos].ID;
    queryMovieID = movPos;

    if(baseList[movPos].status != MOV_STATUS_RETURNED) return QUERY_RENT_RENTED;
    else return QUERY_RENT_NOTRENTED;
}