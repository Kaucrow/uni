#include "rent_movie.h"
#include <iostream>
using std::getline;

void RentMovie(std::wfstream &csvFile, int movieID, wstring username, wstring date){
    csvFile.seekg(0);
    wstring readingLine;
    int counter = 0;
    getline(csvFile, readingLine);
    while(counter != movieID){
        getline(csvFile, readingLine);
        counter++;
    }
    std::wcout << readingLine << '\n';
}