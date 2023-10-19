#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode().
#include <boost/locale.hpp>
#include "./lib/structs.h"
#include "./lib/merge_sort/merge_sort.h"

using   std::wcout, std::wcerr, std::cin, std::string, std::getline,
        std::wifstream, std::wifstream, std::wstring;

enum { DURATION, TITLE, DIRECTOR, YEAR, MONTH, DAY };

int GetNumMovies(wifstream& inFile);            // returns the num of movies in the movies.csv file.
void PopulateMovieList(Movie movList[], wifstream& inFile);   // puts the movies in the movies.csv file into the movie list.
int main(){
    boost::locale::generator gen;               // create the locale generator.
    std::locale loc = gen("en_US");             // create an "en_US" locale.
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // change the STDOUT mode to use UTF-8 characters.

    wstring inFileName = L"./data/movies.csv";
    wifstream inFile(inFileName.c_str());
    inFile.imbue(loc);                          // apply the locale to the movies.csv file.
    if(!inFile){ wcerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }

    int totalMovies = GetNumMovies(inFile);
    Movie durList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie ttlList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie dirList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie yeaList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie monList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie dayList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    Movie* movList[6] = { durList, ttlList, dirList, yeaList, monList, dayList };
    //wcout << GetNumMovies(inFile) << '\n';      // debug.
    //wcout << sizeof(movList) << '\n';         // debug.
    
    try{ PopulateMovieList(movList[DURATION], inFile); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    for(int i = 0; i <= totalMovies; i++){
        movList[TITLE][i]    = movList[DURATION][i];
        movList[DIRECTOR][i] = movList[DURATION][i];
        movList[YEAR][i]     = movList[DURATION][i];
        movList[MONTH][i]    = movList[DURATION][i];
        movList[DAY][i]      = movList[DURATION][i];
    }

    for(int i = 0; i < 6; i++)
        MergeSort(movList[i], 1, totalMovies, i);

    wcout << "Executes" << '\n';
    wcout << movList[DAY][1000].release.day << '\n';
    wcout << movList[TITLE][1000].title << '\n';

    return 0;
}

void PopulateMovieList(Movie movList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    int nextComma;      // stores the pos of the next comma in the curr line.
    wstring wReadingLine;       // stores the curr line.

    // vars for setting the movie genres.
    int nextPipe = 0;           // stores the pos of the next pipe in the wTempReadingLine.
    bool exitSuccess = false;   // true if no more than 5 genres were found, false otherwise.
    wstring wTempReadingLine;   // temp copy of the curr line.
    wstring genreExc = L"ERR: FOUND TOO MANY GENRES ON MOVIE NUMBER ";  // exc thrown when exitSuccess is false.

    inFile.seekg(0);
    getline(inFile, wReadingLine);      // ignore the first line in the inFile.

    for(int i = 1; i <= numMovies; i++){
        getline(inFile, wReadingLine);  // get the next line
        for(int j = 0; j < 6; j++){     // and store each of the 6 data fields.
            nextComma = wReadingLine.find(',');
            switch(j){
                // ID
                case 0: 
                    movList[i].ID = stoi(wReadingLine);       // store the first number of the curr line.
                    break;
                // Title
                case 1:
                    // if the first character is a double quote, adjust the value of nextComma to actually
                    // contain the pos of the comma after the title.
                    // WARNING: assumes the last double quote char is within the title, and nowhere else
                    if(wReadingLine[0] == '"') nextComma = (wReadingLine.substr(1)).find_last_of('"') + 2;
                    movList[i].title = wReadingLine.substr(0, nextComma);
                    break;
                // Genres
                case 2:
                    // create a temp copy of the curr line with only the movie genres in it.
                    wTempReadingLine = wReadingLine.substr(0, nextComma);
                    // store a max of 6 genres.
                    for(int k = 0; k < 6; k++){         
                        nextPipe = wTempReadingLine.find('|');
                        // if no pipe character was found on the temp line, assign the last genre and stop storing any more.
                        if(nextPipe == -1){ movList[i].genres[k] = wTempReadingLine; exitSuccess = true; break; }
                        // otherwise, store the curr genre and remove it from the temp line.
                        else{
                            movList[i].genres[k] = wTempReadingLine.substr(0, nextPipe);
                            wTempReadingLine = wTempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    // throw an exception if the loop never encountered a "break", meaning there were
                    // more than 6 genres in the temp line.
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(std::to_wstring(i)); throw genreExc; }
                // Duration
                case 3:
                    wcout << "CASE3: " << i << '\n';
                    movList[i].duration = stoi(wReadingLine); 
                    break;
                // Director
                case 4:
                    movList[i].director = wReadingLine.substr(0, nextComma);
                    break;
                // Release
                case 5:
                    // WARNING: assumes that the release date is in the year-month-day format,
                    // the year is 4 characters long, and the month and day are two characters long each.
                    movList[i].release.year  = stoi(wReadingLine.substr(0, 4));
                    movList[i].release.month = stoi(wReadingLine.substr(5, 2));
                    movList[i].release.day   = stoi(wReadingLine.substr(8, 2));
                    break;
            } 
            wReadingLine = wReadingLine.substr(nextComma + 1);  // remove the stored data field from the curr line
        }
    }
    //wcout << '\n';      // debug.
    //wcout << movList[1].title << '\n';       // debug.
}

// sets the inFile pos to the start of the last line, gets the line,
// and returns the first number in it.
int GetNumMovies(wifstream& inFile){
    inFile.seekg(0, std::ios_base::end);            // move to the EOF.
    std::wifstream::pos_type pos = inFile.tellg();  // get the curr pos and assign to a variable.
    pos = int(pos) - 2;             // reduce the pos by two to
    inFile.seekg(pos);              // go back two chars.

    wchar_t ch = ' ';
    // executes while a newline isn't found.
    while(ch != '\n'){
        pos = int(pos) - 1;         // reduce the pos by one to
        inFile.seekg(pos);          // go back one char.
        inFile.get(ch);             // get the curr char.
    }

    wstring lastLine;
    getline(inFile, lastLine);
    
    inFile.seekg(0, std::ios_base::beg);    // put the inFile position back at the beginning
    return stoi(lastLine);          // return the first number in the last line.
}