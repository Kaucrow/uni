#include "rent_movie.h"
#include <iostream>
using std::getline;

void RentMovie(wstring csvFileName, int movieID, wstring username, wstring date){
    auto GetNthCommaPos = [](wstring line, int commaNum) -> int{
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

    const char writeFileName[] = "./data/write.csv";
    std::wfstream csvFile(csvFileName.c_str());
    std::wofstream writeFile(writeFileName);
    //csvFile.seekg(0);
    wstring readingLine;
    int counter = 0;
    getline(csvFile, readingLine);
    writeFile << readingLine << '\n';
    while(counter != movieID - 1){
        getline(csvFile, readingLine);
        std::wcout << readingLine << '\n';
        writeFile << readingLine << '\n';
        counter++;
    }
    getline(csvFile, readingLine);
    readingLine = readingLine.substr(0, GetNthCommaPos(readingLine, 6));
    readingLine.append(L',' + username + L',' + date + L',' + L"rented");
    writeFile << readingLine << '\n';
    while(getline(csvFile, readingLine)){
        writeFile << readingLine << '\n';
    }
    csvFile.close();
    writeFile.close(); 
    char csvBuffer[32];
    wcstombs(csvBuffer, csvFileName.c_str(), sizeof(csvBuffer));
    std::wcout << csvBuffer << '\n';
    remove(csvBuffer);
    rename(writeFileName, csvBuffer);
}