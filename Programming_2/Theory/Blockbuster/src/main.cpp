#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode()
#include <boost/locale.hpp>
#include "structs.h"
using   std::wcout, std::wcerr, std::cin, std::string, std::getline,
        std::wifstream, std::wifstream, std::wstring;

int GetNumMovies(wifstream& inFile);            // returns the num of movies in the movies.csv file
void PopulateMovieList(Movie movieList[], wifstream& inFile);   // puts the movies in the movies.csv file into the movie list
int main(){
    boost::locale::generator gen;               // create the locale generator.
    std::locale loc = gen("en_US");             // create an "en_US" locale
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // change the STDOUT mode to use UTF-8 characters.

    wstring inFileName = L"./data/movies.csv";
    wifstream inFile(inFileName.c_str());
    inFile.imbue(loc);                          // apply the locale to the movies.csv file.
    if(!inFile){ wcerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }

    Movie movieList[GetNumMovies(inFile) + 3001];       // create a list of movies for 4000 movies, where the first index is unused
    wcout << GetNumMovies(inFile) << '\n';      // debug
    wcout << sizeof(movieList) << '\n';         // debug
    try{ PopulateMovieList(movieList, inFile); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; } 
}

void PopulateMovieList(Movie movieList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    int nextComma;
    wstring wideReadingLine;

    // vars for setting the movie genres 
    int nextPipe = 0;
    bool exitSuccess = false;
    wstring tempReadingLine;
    wstring exc = L"ERR: FOUND TOO MANY GENRES ON MOVIE NUMBER ";

    inFile.seekg(0);
    getline(inFile, wideReadingLine);

    for(int i = 1; i <= 100; i++){
        getline(inFile, wideReadingLine);
        for(int j = 0; j < 6; j++){
            nextComma = wideReadingLine.find(',');
            switch(j){
                // ID
                case 0: 
                    movieList[i].ID = stoi(wideReadingLine);
                    break;
                // Title
                case 1:
                    if(wideReadingLine[0] == '"') nextComma = wideReadingLine.find('"', 1) + 1;
                    movieList[i].title = wideReadingLine.substr(0, nextComma); 
                    break;
                // Genres
                case 2:
                    tempReadingLine = wideReadingLine.substr(0, nextComma);
                    for(int k = 0; k < 5; k++){
                        nextPipe = tempReadingLine.find('|');
                        if(nextPipe == -1){ movieList[i].genres[k] = tempReadingLine; exitSuccess = true; break; }
                        else{
                            movieList[i].genres[k] = tempReadingLine.substr(0, nextPipe);
                            tempReadingLine = tempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ exc.append(std::to_wstring(i)); throw exc; }
                // Duration
                case 3:
                    movieList[i].duration = stoi(wideReadingLine); 
                    break;
                // Director
                case 4:
                    movieList[i].director = wideReadingLine.substr(0, nextComma);
                    break;
                // Release
                case 5:
                    movieList[i].release.year  = stoi(wideReadingLine.substr(0, 4));
                    movieList[i].release.month = stoi(wideReadingLine.substr(5, 2));
                    movieList[i].release.day   = stoi(wideReadingLine.substr(8, 2));
                    break;
            }
            wideReadingLine = wideReadingLine.substr(nextComma + 1);
        }
    }
    wcout << '\n';
    wcout << movieList[47].genres[4] << '\n';
}

int GetNumMovies(wifstream& inFile){
    inFile.seekg(0, std::ios_base::end);            // move to the EOF
    std::wifstream::pos_type pos = inFile.tellg();  // get the curr pos and assign to a variable
    pos = int(pos) - 1;
    inFile.seekg(pos);              // go back one char
    std::wcout << pos << '\n';      // debug

    wchar_t ch;
    // executes while a newline isn't found
    while(ch != '\n'){
        pos = int(pos) - 1;
        inFile.seekg(pos);          // go back one char
        inFile.get(ch);             // get the curr char
        std::wcout << ch << ' ';    // debug
    }

    wstring lastLine;
    getline(inFile, lastLine);
    wcout << lastLine << '\n';      // debug

    inFile.seekg(0, std::ios_base::beg);    // put the inFile position back to the beginning
    return stoi(lastLine);          // return the first number in the last line
}