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

void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring rentDate, wstring expiryDate){
    std::wifstream csvFile(csvFilePath);             // Open the csvFile.

    wstring readingLine;
    int counter = -1;                       // Counts the lines read.
    // Keep reading lines and writing them to writeFile //
    // until the line to update is reached.             //
    while(counter != movieID - 1){
        getline(csvFile, readingLine);
        counter++;
    }
    
    getline(csvFile, readingLine);          // Read the line to update.
    // Update the line, and write it to writeFile. //
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 6));
    readingLine.append(L',' + username + L',' + rentDate + L",rented," + expiryDate);
    csvFile.close();
    ReplaceLine(csvFilePath, readingLine, movieID + 1);
}

void UpdateUsersDataCsv(const char* usersDataFilePath, int currUser, wstring movieTtl){
    std::wifstream usersDataFile(usersDataFilePath);

    wstring readingLine;
    int counter = -1;
    while(counter != currUser - 1){
        getline(usersDataFile, readingLine);
        counter++;
    }

    getline(usersDataFile, readingLine);

    wstring currMovies = readingLine.substr(GetNthCommaPos(readingLine, 2));
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 2));
    readingLine.append(currMovies.append(L'|' + movieTtl));
    usersDataFile.close();
    ReplaceLine(usersDataFilePath, readingLine, currUser + 1);
}

void UpdateMovieData(Movie baseList[], int movieID, wstring username, wstring rentDate, wstring expiryDate){
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

void UpdateUsersData(User userList[], int currUser, wstring movieTtl){
    userList[currUser].movies.append(L'|' + movieTtl);
}

int QueryMovieRent(Movie baseList[], WstrFrag ttlFrag[], int totalMovies, wstring title, int& queryMovieID){
    // Perform a search by title for the entered movie. //
    int ttlPos = BinSearch(ttlFrag, 1, totalMovies, title);
    
    if(ttlPos == -1) return QUERY_RENT_NOTFOUND;

    int movPos = ttlFrag[ttlPos].ID;
    queryMovieID = movPos;

    if(baseList[movPos].rentedTo != L"") return QUERY_RENT_RENTED;
    else return QUERY_RENT_NOTRENTED;
}