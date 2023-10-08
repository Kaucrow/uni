#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode()
#include <boost/locale.hpp>
#include "structs.h"
using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::wifstream, std::wifstream, std::wstring, std::wcout;

int GetNumMovies(wifstream& inFile);            // returns the num of movies in the movies.csv file
void PopulateMovieList(Movie movieList[], wifstream& inFile);   // puts the movies in the movies.csv file into the movie list
int main(){
    boost::locale::generator gen;               // create the locale generator.
    std::locale loc = gen("en_US");             // create an "en_US" locale
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // change the STDOUT mode to use UTF-8 characters.

    string inFileName = "./data/movies.csv";    
    wifstream inFile(inFileName);
    inFile.imbue(loc);                          // apply the locale to the movies.csv file.
    if(!inFile){ cerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }

    Movie movieList[GetNumMovies(inFile) + 3001];       // create a list of movies for 4000 movies, where the first index is unused
    wcout << GetNumMovies(inFile) << '\n';      // debug
    wcout << sizeof(movieList) << '\n';         // debug
    PopulateMovieList(movieList, inFile);
}

void PopulateMovieList(Movie movieList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    int nextComma;
    wstring wideReadingLine;

    inFile.seekg(0);
    getline(inFile, wideReadingLine);

    for(int i = 1; i <= 100; i++){
        getline(inFile, wideReadingLine);
        wcout << "i: " << i << '\n';                // debug
        for(int j = 0; j < 6; j++){
            nextComma = wideReadingLine.find(',');
            switch(j){
                case 0: movieList[i].ID = stoi(wideReadingLine.substr(0)); break;
                case 1:
                    wcout << nextComma << '\n';     // debug
                    if(wideReadingLine[0] == '"') nextComma = wideReadingLine.find('"', 1) + 1;
                    wcout << nextComma << '\n';     // debug
                    movieList[i].title = wideReadingLine.substr(0, nextComma); break;
                case 2: movieList[i].genres = wideReadingLine.substr(0, nextComma); break;
                case 3: movieList[i].duration = stoi(wideReadingLine); break;
                case 4: movieList[i].director = wideReadingLine.substr(0, nextComma); break;
            }
            wideReadingLine = wideReadingLine.substr(nextComma + 1);
        }
    }
    wcout << movieList[47].duration << '\n';
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