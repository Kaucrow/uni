#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode().
#include <ctime>
#include <iomanip>
#include <boost/locale.hpp>
#include <structs.h>
#include <merge_sort.h>
#include <bin_search.h>
#include <store_movie.h>
#include <rent_movie.h>

using   std::wcout, std::wcerr, std::wcin, std::getline, std::wfstream,
        std::wifstream, std::wifstream, std::wstring;

enum { DURATION, TITLE, DIRECTOR, YEAR, MONTH, DAY };
enum { SEARCH = 1, ADD = 2, RENT = 3, EXIT = 4 };


/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

wstring GetDateTime();

/**
 * @brief Gets the number of movies in the movies.csv file.
 * @param inFile - wifstream object that contains the movies.csv file.
 * @return Number of movies in the movies.csv file.
 * @see GetNumMoviesImplem "GetNumMovies() implementation details".
 */
int GetNumMovies(wfstream& inFile);

/**
 * @brief Puts the movies in the movies.csv file into a movie list array.
 * @param movList - Movie list array to store the movies in.
 * @param inFile - wifstream object that contains the movies.csv file.
 */
void PopulateMovieList(Movie movList[], wfstream& inFile);

int main(){
    boost::locale::generator gen;               // Create the locale generator.
    std::locale loc = gen("en_US");             // Create an "en_US" locale
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // Change the STDOUT mode to use UTF-8 characters.

    wstring csvName = L"./data/movies.csv";
    wfstream csvFile(csvName.c_str());
    if(!csvFile){ wcerr << "ERR: FILE \"" << csvName << "\" COULD NOT BE OPENED."; return 1; }
    csvFile.imbue(loc);                          // Apply the locale to the movies.csv stream object.

    int totalMovies = GetNumMovies(csvFile);     // Get the number of movies in the movies.csv file.

    /*  Create lists of movies which hold 4000 movies each, where the first index of the array is unused.
        each movie list is sorted according to a different property
        (duration, title, director, release year, release month, release day). */
    Movie durList[totalMovies + 3001];
    Movie ttlList[totalMovies + 3001];
    Movie dirList[totalMovies + 3001];
    Movie yeaList[totalMovies + 3001];
    Movie monList[totalMovies + 3001];
    Movie dayList[totalMovies + 3001];
    Movie* movList[6] = { durList, ttlList, dirList, yeaList, monList, dayList };
    
    wstring username, lastName;

    int userCount = 0;
  
    wcout << "Input your full name: ";
    wcin >> username;

    userCount++; 

    wfstream binFile("./data/user_data.bin", std::ios::out | std::ios::app | std::ios::binary);

    if(binFile.is_open()){ 
        binFile.write((wchar_t*) &userCount, sizeof(userCount)); 
        binFile.write(username.c_str(), username.size() + 1);
        //file.write(lastName.c_str(), lastName.size() + 1); 
        binFile.close();
    }

    /* Populate the DURATION movie list. */
    try{ PopulateMovieList(movList[DURATION], csvFile); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    /* Copy the duration movie list elements to all of the other lists. */
    for(int i = 1; i <= totalMovies; i++){
        movList[TITLE][i]    = movList[DURATION][i];
        movList[DIRECTOR][i] = movList[DURATION][i];
        movList[YEAR][i]     = movList[DURATION][i];
        movList[MONTH][i]    = movList[DURATION][i];
        movList[DAY][i]      = movList[DURATION][i];
    }

    /* Sort each list. */
    for(int i = 0; i < 6; i++)
        MergeSort(movList[i], 1, totalMovies, i);

    /***************************
    /*  Main loop.
     **************************/
    while(true){
        ClrScr();
        int action;
        wcout   << "*** CHOOSE AN ACTION ***\n"
                << "(1) Search with filters\n"
                << "(2) Add a movie\n"
                << "(3) Rent a movie\n"
                << "(4) Exit\n"
                << "Select option: ";
        wcin >> action;
        while(action < SEARCH || action > EXIT){
            wcout << "INVALID OPTION.\nSelect option: ";
            wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            wcin >> action;
        }
        wcin.ignore(1);

        /***************************
        /*  Search with filters.
         **************************/
        if(action == SEARCH){
            ClrScr();
            Movie matches[totalMovies + 3001];      // Movie list array to stores the search matches.
            Movie toMatch;                          // Movie that holds the parameter to be matched in the corresponding movie list.
            wcout   << "*** FILTERS ***\n"
                    << "(1) Duration\n"
                    << "(2) Title\n"
                    << "(3) Director\n"
                    << "(4) Release year\n"
                    << "(5) Release month\n"
                    << "(6) Release day\n"
                    << "Select option: ";

            wcin >> action;
            while(action < (DURATION + 1) || action > (DAY + 1)){
                wcout << "INVALID OPTION.\nSelect option: ";
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                wcin >> action;
            }
            wcin.ignore(1);

            wstring search;
            ClrScr();

            /**
             * Generate the "toMatch" movie according to the selected filter,
             * perform the search, and store the matching movies in the "matches" movie list array.
             */
            switch(action){
                case DURATION + 1:
                    wcout << "Duration to search for: ";
                    getline(wcin, search);
                    toMatch.duration = stoi(search);
                    BinSearch(movList[DURATION], matches, 1, totalMovies, toMatch, DURATION);
                    break;
                case TITLE + 1:
                    wcout << "Title to search for: ";
                    getline(wcin, search);
                    toMatch.title = search;
                    BinSearch(movList[TITLE], matches, 1, totalMovies, toMatch, TITLE);
                    break;
                case DIRECTOR + 1:
                    wcout << "Director to search for: ";
                    getline(wcin, search);
                    toMatch.director = search;
                    BinSearch(movList[DIRECTOR], matches, 1, totalMovies, toMatch, DIRECTOR);
                    break;
                case YEAR + 1:
                    wcout << "Year to search for: ";
                    getline(wcin, search);
                    toMatch.release.year = stoi(search);
                    BinSearch(movList[YEAR], matches, 1, totalMovies, toMatch, YEAR);
                    break;
                case MONTH + 1:
                    wcout << "Month to search for: ";
                    getline(wcin, search);
                    while(stoi(search) < 1 || stoi(search) > 12){
                        wcout << "Please input a valid month.\n";
                        getline(wcin, search);
                    }
                    toMatch.release.month = stoi(search);
                    BinSearch(movList[MONTH], matches, 1, totalMovies, toMatch, MONTH);
                    break;
                case DAY + 1:
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
                    wcerr << "[ ERR ] THIS SHOULD NEVER EXECUTE. IT'S ONLY HERE FOR DEBUG PURPOSES.\n";     // debug
                    break;
            }

            ClrScr();
            /* Print the matching movies from the "matches" movie list array. */
            for(int i = 0; matches[i].duration != 0; i++){
                wcout << matches[i].title << '\n';
            }
            
        }
        /***************************
        /*  Add a movie.
         **************************/
        else if(action == ADD){
            ClrScr();
            Movie toStore;
            wstring storeDat;
            totalMovies++;              // Increase the count of movies.

            wcout << "*** NEW MOVIE DATA ***\n";

            /* Get each of the 6 data fields of the movie that will be added.*/
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

            /**
             * Update each of the movie lists with the added movie, while preserving the
             * sorting order in each of them.
             */
            StoreNewMovie(movList[DURATION], 1, totalMovies, toStore, DURATION);
            StoreNewMovie(movList[TITLE], 1, totalMovies, toStore, TITLE);
            StoreNewMovie(movList[DIRECTOR], 1, totalMovies, toStore, DIRECTOR);
            StoreNewMovie(movList[YEAR], 1, totalMovies, toStore, YEAR);
            StoreNewMovie(movList[MONTH], 1, totalMovies, toStore, MONTH);
            StoreNewMovie(movList[DAY], 1, totalMovies, toStore, DAY);

            wcout << "[ INFO ] THE MOVIE WAS ADDED SUCCESSFULLY.\n";
        }
        else if(action == RENT){ 
            Movie toRent[1];
            wcout << "This execs\n";
            wcout << "*** MOVIE RENT ***\n";
            wcout << "Input the name of the movie: ";
            wcin >> toRent[0].title;
            if(BinSearch(ttlList, toRent, 1, totalMovies, toRent[0], TITLE) == -1);
            else{
                wcout << toRent[0].title << '\n';
            }
            wstring currDate = GetDateTime().substr(0, 10);
            wcout << currDate << '\n';
            wcout << toRent[0].ID << '\n';

            RentMovie(csvFile, toRent[0].ID, username, currDate);

            wcin.get();
        }
        else{ wcout << "\nTerminating program...\n"; return 0; }
        wcin.get();
    }
    return 0;
}

void PopulateMovieList(Movie movList[], wfstream& inFile){
    int numMovies = GetNumMovies(inFile);
    int nextComma;              // Stores the pos of the next comma in the curr line.
    wstring wReadingLine;       // Stores the curr line.

    /* Vars for setting the movie genres. */
    int nextPipe = 0;           // Stores the pos of the next pipe in the wTempReadingLine.
    bool exitSuccess = false;   // True if no more than 6 genres were found, false otherwise.
    wstring wTempReadingLine;   // Temp copy of the curr line.
    wstring genreExc = L"ERR: FOUND TOO MANY GENRES ON MOVIE NUMBER ";  // Exc thrown when exitSuccess is false.

    inFile.seekg(0);
    getline(inFile, wReadingLine);      // Ignore the first line in the inFile.

    for(int i = 1; i <= numMovies; i++){
        getline(inFile, wReadingLine);  // Get the next line
        for(int j = 0; j < 6; j++){     // And store each of the 6 data fields.
            nextComma = wReadingLine.find(',');
            switch(j){
                /* ID */
                case 0: 
                    movList[i].ID = stoi(wReadingLine);       // Store the first number of the curr line.
                    break;
                /* Title */
                case 1:
                    /** 
                     * If the first character is a double quote, adjust the value of nextComma to actually
                     * contain the pos of the comma after the title.
                     * WARNING: Assumes the last double quote char is within the title, and nowhere else.
                     */
                    if(wReadingLine[0] == '"') nextComma = (wReadingLine.substr(1)).find_last_of('"') + 2;
                    movList[i].title = wReadingLine.substr(0, nextComma);
                    break;
                /* Genres */
                case 2:
                    /* Create a temp copy of the curr line with only the movie genres in it. */
                    wTempReadingLine = wReadingLine.substr(0, nextComma);
                    /* Store a max of 6 genres. */
                    for(int k = 0; k < 6; k++){         
                        nextPipe = wTempReadingLine.find('|');
                        /* If no pipe character was found on the temp line, assign the last genre and stop storing any more. */
                        if(nextPipe == -1){ movList[i].genres[k] = wTempReadingLine; exitSuccess = true; break; }
                        /* Otherwise, store the curr genre and remove it from the temp line. */
                        else{
                            movList[i].genres[k] = wTempReadingLine.substr(0, nextPipe);
                            wTempReadingLine = wTempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    /**
                     * Throw an exception if the loop never encountered a break statement, meaning there were
                     * more than 6 genres in the wReadingLine.
                     */
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(std::to_wstring(i)); throw genreExc; }
                /* Duration */
                case 3:
                    movList[i].duration = stoi(wReadingLine); 
                    break;
                /* Director */
                case 4:
                    movList[i].director = wReadingLine.substr(0, nextComma);
                    break;
                /* Release */
                case 5:
                    /**
                     * WARNING: Assumes that the release date is in the year-month-day format,
                     * the year is 4 characters long, and the month and day are two characters long each.
                     */
                    movList[i].release.year  = stoi(wReadingLine.substr(0, 4));
                    movList[i].release.month = stoi(wReadingLine.substr(5, 2));
                    movList[i].release.day   = stoi(wReadingLine.substr(8, 2));
                    break;
            } 
            wReadingLine = wReadingLine.substr(nextComma + 1);  // Remove the stored data field from the curr line.
        }
    }
}

/**
 * @name GetNumMoviesImplem
 * Sets the inFile pos to the start of the last line, gets the line,
 * and returns the first number in it.
 */
int GetNumMovies(wfstream& inFile){
    inFile.seekg(0, std::ios_base::end);            // Move to the EOF.
    std::wifstream::pos_type pos = inFile.tellg();  // Get the curr pos and assign to a variable.
    pos = int(pos) - 2;             // Reduce the pos by two to
    inFile.seekg(pos);              // Go back two chars.

    wchar_t ch = ' ';
    /* Executes while a newline isn't found. */
    while(ch != '\n'){
        pos = int(pos) - 1;         // Reduce the pos by one to
        inFile.seekg(pos);          // go back one char.
        inFile.get(ch);             // Get the curr char.
    }

    wstring lastLine;
    getline(inFile, lastLine);
    
    inFile.seekg(0, std::ios_base::beg);    // Put the inFile position back at the beginning.
    return stoi(lastLine);          // Return the first number in the last line.
}

wstring GetDateTime(){
    time_t rawtime;
    struct tm timeinfo;
    wchar_t buffer[20];

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    wcsftime(buffer, 20, L"%Y-%m-%d %H:%M:%S", &timeinfo);

    return buffer;
}

void ClrScr(){
    #ifdef _WIN32
        // If on Windows OS
        std::system("cls");
    #else
        // Assuming POSIX OS
        std::system("clear");
    #endif
}