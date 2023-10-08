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
    //PopulateMovieList(movieList, inFile);
}

/*void PopulateMovieList(Movie movieList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    string readingLine;
    std::wstring wideReadingLine;
    int commaPos[6];
    auto GetCommas = [&wideReadingLine, &commaPos](){
        int nextComma, totalComma = 0;
        for(int i = 0; i < 6; i++){
            nextComma = wideReadingLine.find(',');
            if(i == 0) totalComma += nextComma;
            else totalComma += nextComma + 1;
            commaPos[i] = totalComma;
            std::wcout << wideReadingLine << '\n';
            wideReadingLine = wideReadingLine.substr(nextComma + 1);
        }
    };
    inFile.seekg(0);
    getline(inFile, wideReadingLine);

    for(int i = 1; i <= numMovies; i++){
        getline(inFile, wideReadingLine);
        if(i == 1) GetCommas();
        // debug
        if(i == 1){
            cout << commaPos[0] << '\n';
            cout << commaPos[1] << '\n';
            cout << commaPos[2] << '\n';
            cout << commaPos[3] << '\n';
            cout << commaPos[4] << '\n';
            cout << commaPos[5] << '\n';
        }
        //movieList[i].ID = stoi(readingLine.substr(0, readingLine.find(',')));
    }
}*/

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