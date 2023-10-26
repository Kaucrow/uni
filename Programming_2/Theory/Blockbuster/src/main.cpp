#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode().
#include <boost/locale.hpp>
#include "structs.h"
using   std::wcout, std::wcerr, std::cin, std::string, std::getline,
        std::wifstream, std::wifstream, std::wstring, std::fstream, std::wcin;


int GetNumMovies(wifstream& inFile);            // returns the num of movies in the movies.csv file.
void PopulateMovieList(Movie movieList[], wifstream& inFile);   // puts the movies in the movies.csv file into the movie list.
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
    Movie movieList[totalMovies + 3001];        // create a list of movies for 4000 movies, where the first index is unused.
    wcout << GetNumMovies(inFile) << '\n';      // debug.
    wcout << sizeof(movieList) << '\n';         // debug.
    try{ PopulateMovieList(movieList, inFile); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }
    
    string name, lastName;

    int countOfUsers = 0;
  
    wcout << "Input your name: ";
    cin >> name;

    wcout << "Input your last name: "; 
    cin >> lastName;

    countOfUsers++; 

    fstream file("user_data.bin", std::ios::out | std::ios::app | std::ios::binary);

    if (file.is_open()) {
    
        file.write((char*) &countOfUsers, sizeof(countOfUsers)); 
        file.write(name.c_str(), name.size() + 1);
        file.write(lastName.c_str(), lastName.size() + 1);
    
    file.close();
    }
    else {
    wcout << "File could not be opened";
    }

    return 0;
}

void PopulateMovieList(Movie movieList[], wifstream& inFile){
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

    for(int i = 1; i <= 100; i++){
        getline(inFile, wReadingLine);  // get the next line
        for(int j = 0; j < 6; j++){     // and store each of the 6 data fields.
            nextComma = wReadingLine.find(',');
            switch(j){
                // ID
                case 0: 
                    movieList[i].ID = stoi(wReadingLine);       // store the first number of the curr line.
                    break;
                // Title
                case 1:
                    // if the first character is a double quote, adjust the value of nextComma to actually
                    // contain the pos of the comma after the title.
                    if(wReadingLine[0] == '"') nextComma = wReadingLine.find('"', 1) + 1;
                    movieList[i].title = wReadingLine.substr(0, nextComma);
                    break;
                // Genres
                case 2:
                    // create a temp copy of the curr line with only the movie genres in it.
                    wTempReadingLine = wReadingLine.substr(0, nextComma);
                    // store a max of 5 genres.
                    for(int k = 0; k < 5; k++){         
                        nextPipe = wTempReadingLine.find('|');
                        // if no pipe character was found on the temp line, assign the last genre and stop storing any more.
                        if(nextPipe == -1){ movieList[i].genres[k] = wTempReadingLine; exitSuccess = true; break; }
                        // otherwise, store the curr genre and remove it from the temp line.
                        else{
                            movieList[i].genres[k] = wTempReadingLine.substr(0, nextPipe);
                            wTempReadingLine = wTempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    // throw an exception if the loop never encountered a "break", meaning there were
                    // more than 5 genres in the temp line.
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(std::to_wstring(i)); throw genreExc; }
                // Duration
                case 3:
                    movieList[i].duration = stoi(wReadingLine); 
                    break;
                // Director
                case 4:
                    movieList[i].director = wReadingLine.substr(0, nextComma);
                    break;
                // Release
                case 5:
                    // WARNING: assumes that the release date is in the year-month-day format,
                    // the year is 4 characters long, and the month and day are two characters long each.
                    movieList[i].release.year  = stoi(wReadingLine.substr(0, 4));
                    movieList[i].release.month = stoi(wReadingLine.substr(5, 2));
                    movieList[i].release.day   = stoi(wReadingLine.substr(8, 2));
                    break;
            } 
            wReadingLine = wReadingLine.substr(nextComma + 1);  // remove the stored data field from the curr line
        }
    }
    wcout << '\n';      // debug.
    wcout << movieList[47].genres[4] << '\n';       // debug.
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