#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode().
#include <boost/locale.hpp>
#include <structs.h>
#include <merge_sort.h>
#include <bin_search.h>
#include <store_movie.h>

using   std::wcout, std::wcerr, std::wcin, std::getline, std::wfstream,
        std::wifstream, std::wifstream, std::wstring;

enum { DURATION, TITLE, DIRECTOR, YEAR, MONTH, DAY };

void ClrScr();
int GetNumMovies(wifstream& inFile);            // returns the num of movies in the movies.csv file.
void PopulateMovieList(Movie movList[], wifstream& inFile);   // puts the movies in the movies.csv file into the movie list.
int main(){

    boost::locale::generator gen;               // create the locale generator.
    std::locale loc = gen("en_US");             // create an "en_US" locale.
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // change the STDOUT mode to use UTF-8 characters.

    wstring inFileName = L"./data/movies.csv";
    wifstream inFile(inFileName.c_str());
    inFile.imbue(loc);                          // apply the locale to the movies.csv stream object.
    if(!inFile){ wcerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }

    int totalMovies = GetNumMovies(inFile);     // get the number of movies in the movies.csv file

    // create lists of movies which hold 4000 movies each, where the first index of the array is unused.
    // each movie list is sorted according to a different property (duration, title, director, release year, release month, release day).
    Movie durList[totalMovies + 3001];
    Movie ttlList[totalMovies + 3001];
    Movie dirList[totalMovies + 3001];
    Movie yeaList[totalMovies + 3001];
    Movie monList[totalMovies + 3001];
    Movie dayList[totalMovies + 3001];
    Movie* movList[6] = { durList, ttlList, dirList, yeaList, monList, dayList };
    
    wstring name, lastName;

    int countOfUsers = 0;
  
    wcout << "Input your name: ";
    wcin >> name;

    wcout << "Input your last name: "; 
    wcin >> lastName;

    countOfUsers++; 

    wfstream file("./data/user_data.bin", std::ios::out | std::ios::app | std::ios::binary);

    if (file.is_open()) {
    
        file.write((wchar_t*) &countOfUsers, sizeof(countOfUsers)); 
        file.write(name.c_str(), name.size() + 1);
        file.write(lastName.c_str(), lastName.size() + 1);
    
    file.close();
    }
    else {
    wcout << "File could not be opened";
    }

    // populate the duration movie list
    try{ PopulateMovieList(movList[DURATION], inFile); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    for(int i = 1; i <= totalMovies; i++){
        movList[TITLE][i]    = movList[DURATION][i];
        movList[DIRECTOR][i] = movList[DURATION][i];
        movList[YEAR][i]     = movList[DURATION][i];
        movList[MONTH][i]    = movList[DURATION][i];
        movList[DAY][i]      = movList[DURATION][i];
    }

    for(int i = 0; i < 6; i++)
        MergeSort(movList[i], 1, totalMovies, i);


    while(true){
        ClrScr();
        int action;
        wcout   << "*** CHOOSE AN ACTION ***\n"
                << "(1) Search with filters\n"
                << "(2) Add a movie\n"
                << "Select option: ";
        wcin >> action;
        while(action < 1 || action > 2){
            wcout << "INVALID OPTION.\nSelect option: ";
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            wcin >> action;
        }
        wcin.ignore(1);

        if(action == 1){
            ClrScr();
            Movie matches[totalMovies + 3001];
            Movie toMatch;
            wcout   << "*** FILTERS ***\n"
                    << "(1) Duration\n"
                    << "(2) Title\n"
                    << "(3) Director\n"
                    << "(4) Release year\n"
                    << "(5) Release month\n"
                    << "(6) Release day\n"
                    << "Select option: ";

            wcin >> action;
            while(action < 1 || action > 6){
                wcout << "INVALID OPTION.\nSelect option: ";
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                wcin >> action;
            }
            wcin.ignore(1);

            wstring search;
            ClrScr();
            switch(action){
                case 1:
                    wcout << "Duration to search for: ";
                    getline(wcin, search);
                    toMatch.duration = stoi(search);
                    BinSearch(movList[DURATION], matches, 1, totalMovies, toMatch, DURATION);
                    break;
                case 2:
                    wcout << "Title to search for: ";
                    getline(wcin, search);
                    toMatch.title = search;
                    BinSearch(movList[TITLE], matches, 1, totalMovies, toMatch, TITLE);
                    break;
                case 3:
                    wcout << "Director to search for: ";
                    getline(wcin, search);
                    toMatch.director = search;
                    BinSearch(movList[DIRECTOR], matches, 1, totalMovies, toMatch, DIRECTOR);
                    break;
                case 4:
                    wcout << "Year to search for: ";
                    getline(wcin, search);
                    toMatch.release.year = stoi(search);
                    BinSearch(movList[YEAR], matches, 1, totalMovies, toMatch, YEAR);
                    break;
                case 5:
                    wcout << "Month to search for: ";
                    getline(wcin, search);
                    while(stoi(search) < 1 || stoi(search) > 12){
                        wcout << "Please input a valid month.\n";
                    getline(wcin, search);
                }
                toMatch.release.month = stoi(search);
                BinSearch(movList[MONTH], matches, 1, totalMovies, toMatch, MONTH);
                break;
            case 6:
                wcout << "Day to search for: ";
                getline(wcin, search);
                while(stoi(search) < 1 || stoi(search) > 31){
                    wcout << "Please input a valid day.\n";
                    getline(wcin, search);
                }
                toMatch.release.day = stoi(search);
                BinSearch(movList[DAY], matches, 1, totalMovies, toMatch, DAY);
                break;
            default:
                wcerr << "ERR: INVALID FILTER.\n";
                break;
            }

            //ClrScr();
            for(int i = 0; matches[i].duration != 0; i++){
                wcout << matches[i].title << '\n';
            }
        }
        else{
            ClrScr();
            Movie toStore;
            wstring storeDat;
            totalMovies++;

            wcout << "*** NEW MOVIE DATA ***\n";

            wcout << "-> Duration: ";
            getline(wcin, storeDat);
            while(stoi(storeDat) <= 0){
                wcerr << "[ ERR ] The movie cannot have a duration less than or equal to 0.\n-> Duration:";
                getline(wcin, storeDat);
            }
            toStore.duration = stoi(storeDat);

            wcout << "-> Title: ";
            getline(wcin, storeDat);
            storeDat[0] = toupper(storeDat[0]);
            toStore.title = storeDat;

            wcout << "-> Genres\n";
            for(int i = 0; i < 6; i++){
                wcout << "   * " << i + 1 << ": ";
                getline(wcin, storeDat);
                toStore.genres[i] = storeDat;
            }


            wcout << "-> Director: ";
            getline(wcin, storeDat);
            toStore.director = storeDat;

            wcout << "-> Release year: ";
            getline(wcin, storeDat);
            toStore.release.year = stoi(storeDat);
            
            wcout << "-> Release month: ";
            getline(wcin, storeDat);
            toStore.release.month = stoi(storeDat);
            
            wcout << "-> Release day: ";
            getline(wcin, storeDat);
            toStore.release.day = stoi(storeDat);

            StoreNewMovie(movList[DURATION], 1, totalMovies, toStore, DURATION);
            StoreNewMovie(movList[TITLE], 1, totalMovies, toStore, TITLE);
            StoreNewMovie(movList[DIRECTOR], 1, totalMovies, toStore, DIRECTOR);
            StoreNewMovie(movList[YEAR], 1, totalMovies, toStore, YEAR);
            StoreNewMovie(movList[MONTH], 1, totalMovies, toStore, MONTH);
            StoreNewMovie(movList[DAY], 1, totalMovies, toStore, DAY);

            wcout << "[ INFO ] THE MOVIE WAS ADDED SUCCESSFULLY.\n";
        }
        wcin.get();
    }
    return 0;
}

void PopulateMovieList(Movie movList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    int nextComma;              // stores the pos of the next comma in the curr line.
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
                    //wcout << "CASE3: " << i << '\n';    // debug
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
            wReadingLine = wReadingLine.substr(nextComma + 1);  // remove the stored data field from the curr line.
        }
    }
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

void ClrScr(){
    #ifdef _WIN32
        // if on Windows OS
        std::system("cls");
    #else
        // assuming POSIX OS
        std::system("clear");
    #endif
}