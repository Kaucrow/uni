#include "rent_movie.h"
#include "../structs.h"
#include "../file_ops/file_ops.h"
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

/**
 * @name RentMovieImplem 
 * This function creates a write.csv file, copies the contents of movies.csv
 * to it while also updating the line of the rented movie, and lastly deletes
 * the curr movies.csv and renames write.csv to movies.csv.
*/
void UpdateMoviesCsv(const char* csvFilePath, int movieID, wstring username, wstring rentDate, wstring expiryDate){
    std::wifstream csvFile(csvFilePath);             // Open the csvFile.

    wstring readingLine;
    int counter = -1;                       // Counts the lines read.
    // Keep reading lines and writing them to writeFile //
    // until the line to update is reached.             //
    while(counter != movieID - 1){
        getline(csvFile, readingLine);
        std::wcout << readingLine << '\n';
        counter++;
    }
    
    getline(csvFile, readingLine);          // Read the line to update.
    // Update the line, and write it to writeFile. //
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 6));
    readingLine.append(L',' + username + L',' + rentDate + L",rented," + expiryDate);
    csvFile.close();
    ReplaceLine(csvFilePath, readingLine, movieID + 1);
}

void UpdateMovieData(Movie baseList[], int movieID, wstring rentDate, wstring expiryDate){
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