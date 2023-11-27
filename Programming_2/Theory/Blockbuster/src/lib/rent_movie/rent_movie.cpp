#include "rent_movie.h"
#include "../structs.h"
#include "../file_ops/file_ops.h"
#include "../search_ops/search_ops.h"
#include <iostream>
using std::getline;

/**
 * @brief Gets the position of the Nth semicolon in a line.
 * @param line - pstring object containing the line to search.
 * @param commaNum - Number of semicolon in the line to get the position of.
 * @return Position of the Nth semicolon in the line (zero-based).
 */
int GetNthDelimPos(pstring line, int delimNum){
    int delimCounter = 0, totalPos = 0, currPos = 0;
    while(delimCounter != delimNum){
        currPos = (line.find_first_of(';'));
        totalPos += currPos;
        if(delimCounter > 0) totalPos += 1;
        line = line.substr(currPos + 1);
        delimCounter++;
    }
    return totalPos;
};

void UpdateMoviesCsv(const char* csvFilePath, int movieID, pstring username, pstring rentDate, pstring expiryDate, bool rentOrReturn){
    pifstream csvFile(csvFilePath);        // Open the movies.csv file.
 
    pstring statusType;

    #ifdef _WIN32
        // Executes if the action is a RENT. //
        if(rentOrReturn == UPDATE_RENT) statusType = L"rented";
        // Executes if the action is a RETURN. //
        else if(rentOrReturn == UPDATE_RETURN) statusType = L"returned";
    #else
        // Executes if the action is a RENT. //
        if(rentOrReturn == UPDATE_RENT) statusType = "rented";
        // Executes if the action is a RETURN. //
        else if(rentOrReturn == UPDATE_RETURN) statusType = "returned";
    #endif

    int counter = -1;       // Counts the lines read.
    pstring readingLine;
    // Keep reading lines until the line to update is reached. //
    while(counter != movieID - 1){
        getline(csvFile, readingLine);
        counter++;
    }
    getline(csvFile, readingLine);          // Read the line to update.

    // Update the line, and write it to writeFile. //
    readingLine = readingLine.substr(0, GetNthDelimPos(readingLine, 7));

    #ifdef _WIN32
        readingLine.append(L';' + username + L';' + rentDate + L';' + statusType + L';' + expiryDate);
    #else
        readingLine.append(';' + username + ';' + rentDate + ';' + statusType + ';' + expiryDate);
    #endif

    csvFile.close();
    ReplaceLine(csvFilePath, readingLine, movieID + 1);
}

void UpdateUsersData(const char* usersDataFilePath, List<User> &userList, int currUser, pstring movieTtl, bool rentOrReturn){
    pifstream usersDataFile(usersDataFilePath);        // Open the users_data.csv file.

    pstring readingLine;
    int counter = -1;
    // Keep reading lines until the line to update is reached. //
    while(counter != currUser - 1){
        getline(usersDataFile, readingLine);
        counter++;
    }

    getline(usersDataFile, readingLine);    // Read the line to update.

    usersDataFile.close();

    // Get the line, up to the movies substr. E.g: readingLine will be something like "1;User1;". //
    readingLine = readingLine.substr(0, GetNthDelimPos(readingLine, 2) + 1);

    // Executes if the action is a RENT. //
    if(rentOrReturn == UPDATE_RENT){
        #ifdef _WIN32
            userList.data[currUser].movies.append(L'|' + movieTtl);
        #else
            userList.data[currUser].movies.append('|' + movieTtl);
        #endif

        readingLine.append(userList.data[currUser].movies);
    }
    // Executes if the action is a RETURN. //
    else if(rentOrReturn == UPDATE_RETURN){
        pstring temp = userList.data[currUser].movies;
        temp = temp.substr(0, temp.find(movieTtl) - 1);     // Get the user's movies, up to the one to return.
        temp.append(userList.data[currUser].movies.substr(userList.data[currUser].movies.find(movieTtl) + movieTtl.length()));    // Append the substr that comes after the movie to return.

        userList.data[currUser].movies = temp;   // Update the live movie data.

        readingLine.append(temp);           // Append the updated movies to the readingLine.
    }

    // Update the users_data.csv line. //
    ReplaceLine(usersDataFilePath, readingLine, currUser + 1);
}

void UpdateMovieLiveData(List<Movie> &baseList, int movieID, pstring username, pstring rentDate, pstring expiryDate, bool rentOrReturn){
    // Executes if the action is a RETURN. //
    if(rentOrReturn == UPDATE_RETURN){ baseList.data[movieID].status = MOV_STATUS_RETURNED; return; }

    // Executes if the action is a RENT. //
    baseList.data[movieID].status = MOV_STATUS_RENTED;
    baseList.data[movieID].rentedTo = username;
    for(int i = 0; i < 3; i++){
        switch(i){
            case 0:
                baseList.data[movieID].rentedOn.year = stoi(rentDate);
                baseList.data[movieID].expiry.year = stoi(expiryDate);
                break;
            case 1:
                baseList.data[movieID].rentedOn.month = stoi(rentDate);
                baseList.data[movieID].expiry.month = stoi(expiryDate);
                break;
            case 2:
                baseList.data[movieID].rentedOn.day = stoi(rentDate);
                baseList.data[movieID].expiry.day = stoi(expiryDate);
                break;
        }
        rentDate = rentDate.substr(rentDate.find('-') + 1);
        expiryDate = expiryDate.substr(expiryDate.find('-') + 1);
    }
}

int QueryMovieRent(List<Movie> &baseList, List<PStrFrag> &ttlFrag, int totalMovies, pstring title, int& queryMovieID){
    // Perform a search by title for the entered movie. //
    int ttlPos = BinSearch(ttlFrag, 1, totalMovies, title);

    // If not found, return. // 
    if(ttlPos == -1) return QUERY_RENT_NOTFOUND;
    
    // If found, get its ID, then return a rent or returned status. //
    int movPos = ttlFrag.data[ttlPos].ID;
    queryMovieID = movPos;

    if(baseList.data[movPos].status != MOV_STATUS_RETURNED) return QUERY_RENT_RENTED;
    else return QUERY_RENT_NOTRENTED;
}